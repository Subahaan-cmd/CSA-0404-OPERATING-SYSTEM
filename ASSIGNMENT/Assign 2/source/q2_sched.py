"""Q2: Gantt chart, waiting, turnaround and response times for FCFS, SJF, SRTF, Priority and Round Robin,
plus a multilevel-feedback-queue simulation and an ageing demonstration."""
import collections

JOBS = [("P1", 0, 8, 2), ("P2", 1, 4, 4), ("P3", 2, 9, 1), ("P4", 3, 5, 3)]   # name, arrival, burst, priority (1 = highest)

def compress(tl):
    segs = []
    for t, n in enumerate(tl):
        if n is None: continue
        if segs and segs[-1][0] == n and segs[-1][2] == t: segs[-1][2] = t + 1
        else: segs.append([n, t, t + 1])
    return segs

def metrics(jobs, tl):
    res = {}
    for n, a, b, p in jobs:
        fin = max(t for t, x in enumerate(tl) if x == n) + 1
        first = min(t for t, x in enumerate(tl) if x == n)
        tat = fin - a
        res[n] = (fin, tat, tat - b, first - a)
    return res

def sim(jobs, key, preemptive):
    rem = {n: b for n, a, b, p in jobs}; arr = {n: a for n, a, b, p in jobs}; pr = {n: p for n, a, b, p in jobs}
    order = {n: i for i, (n, a, b, p) in enumerate(jobs)}
    tl = []; t = 0; cur = None
    while any(rem.values()):
        ready = [n for n in rem if rem[n] > 0 and arr[n] <= t]
        if not ready: tl.append(None); t += 1; continue
        if preemptive or cur is None or rem[cur] == 0:
            cur = min(ready, key=lambda n: (key(n, rem, arr, pr), arr[n], order[n]))
        tl.append(cur); rem[cur] -= 1; t += 1
    return tl

def rr(jobs, q):
    rem = {n: b for n, a, b, p in jobs}; tl = []; t = 0; queue = collections.deque(); added = set()
    def admit():
        for n, a, b, p in jobs:
            if n not in added and a <= t: queue.append(n); added.add(n)
    admit()
    while any(rem.values()):
        if not queue: tl.append(None); t += 1; admit(); continue
        n = queue.popleft(); run = min(q, rem[n])
        for _ in range(run): tl.append(n); t += 1
        rem[n] -= run; admit()
        if rem[n] > 0: queue.append(n)
    return tl

def report(title, jobs, tl):
    m = metrics(jobs, tl)
    print(title)
    print("  Gantt: " + " ".join("%s(%d-%d)" % tuple(s) for s in compress(tl)))
    print("  %-4s %6s %5s %5s %5s" % ("Job", "Finish", "TAT", "WT", "RT"))
    for n in sorted(m): print("  %-4s %6d %5d %5d %5d" % ((n,) + m[n]))
    k = len(m)
    print("  Averages: TAT = %.2f  WT = %.2f  RT = %.2f   context switches = %d\n" % (
        sum(v[1] for v in m.values()) / k, sum(v[2] for v in m.values()) / k, sum(v[3] for v in m.values()) / k, len(compress(tl)) - 1))
    return m

print("Jobs (name, arrival, burst, priority):", JOBS, "\n")
report("FCFS", JOBS, sim(JOBS, lambda n, r, a, p: a[n], False))
report("SJF (non-preemptive)", JOBS, sim(JOBS, lambda n, r, a, p: r[n], False))
report("SRTF (preemptive SJF)", JOBS, sim(JOBS, lambda n, r, a, p: r[n], True))
report("Priority (non-preemptive, 1 = highest)", JOBS, sim(JOBS, lambda n, r, a, p: p[n], False))
for q in (1, 3, 5, 20): report("Round Robin, quantum %d" % q, JOBS, rr(JOBS, q))

# ---- Quantum study on a mixed workload: 1 long film + 6 short thumbnails
print("=" * 70)
MIX = [("Film", 0, 60, 3)] + [("T%d" % i, 2 * i, 2, 1) for i in range(1, 7)]
print("Mixed workload (1 long job of 60 units and 6 short jobs of 2 units):")
for name, tl in [("FCFS", sim(MIX, lambda n, r, a, p: a[n], False)), ("SJF", sim(MIX, lambda n, r, a, p: r[n], False)),
                 ("SRTF", sim(MIX, lambda n, r, a, p: r[n], True))] + [("RR q=%d" % q, rr(MIX, q)) for q in (1, 3, 5, 20)]:
    m = metrics(MIX, tl); short = [m[n][2] for n in m if n != "Film"]
    print("  %-8s avg WT short = %6.2f   WT long = %3d   avg WT all = %6.2f   switches = %d" % (
        name, sum(short) / len(short), m["Film"][2], sum(v[2] for v in m.values()) / len(m), len(compress(tl)) - 1))

# ---- Multilevel feedback queue
def mlfq(jobs, quanta=(2, 4, 8), boost=None):
    rem = {n: b for n, a, b, p in jobs}; arr = {n: a for n, a, b, p in jobs}
    level = {n: 0 for n in rem}; qs = [collections.deque() for _ in quanta]; added = set(); tl = []; t = 0
    used = 0; cur = None
    def admit():
        for n, a, b, p in jobs:
            if n not in added and a <= t: qs[0].append(n); added.add(n)
    while any(rem.values()):
        admit()
        if boost and t > 0 and t % boost == 0:
            for lv in range(1, len(qs)):
                while qs[lv]: n = qs[lv].popleft(); level[n] = 0; qs[0].append(n)
            if cur is not None and level[cur] > 0: level[cur] = 0
        top = next((i for i, q in enumerate(qs) if q), None)
        if cur is not None and top is not None and top < level[cur]:     # preempted by a higher queue
            qs[level[cur]].appendleft(cur); cur = None; used = 0
        if cur is None:
            if top is None: tl.append(None); t += 1; continue
            cur = qs[top].popleft(); used = 0
        tl.append(cur); rem[cur] -= 1; used += 1; t += 1
        if rem[cur] == 0: cur = None
        elif used >= quanta[level[cur]]:
            level[cur] = min(level[cur] + 1, len(qs) - 1); qs[level[cur]].append(cur); cur = None
    return tl
m = metrics(MIX, mlfq(MIX)); short = [m[n][2] for n in m if n != "Film"]
print("  %-8s avg WT short = %6.2f   WT long = %3d   avg WT all = %6.2f   switches = %d" % ("MLFQ", sum(short) / len(short), m["Film"][2], sum(v[2] for v in m.values()) / len(m), len(compress(mlfq(MIX))) - 1))

# ---- Starvation and ageing
print("=" * 70)
print("Starvation demo: one long job (10 units, arrives at 1) and a stream of short jobs (2 units) arriving every 2 units from t=0 until t=60.")
STR = [("Long", 1, 10, 5)] + [("S%02d" % i, 2 * (i - 1), 2, 1) for i in range(1, 32)]
for label, key in [("SJF without ageing", lambda n, r, a, p: r[n]), ("Priority without ageing", lambda n, r, a, p: p[n])]:
    tl = sim(STR, key, False); m = metrics(STR, tl); print("  %-26s Long job waits %3d units (finishes at %d)" % (label, m["Long"][2], m["Long"][0]))
def sim_age(jobs, rate):
    rem = {n: b for n, a, b, p in jobs}; arr = {n: a for n, a, b, p in jobs}; tl = []; t = 0; cur = None
    while any(rem.values()):
        ready = [n for n in rem if rem[n] > 0 and arr[n] <= t]
        if not ready: tl.append(None); t += 1; continue
        if cur is None or rem[cur] == 0: cur = min(ready, key=lambda n: (rem[n] - rate * (t - arr[n]), arr[n]))
        tl.append(cur); rem[cur] -= 1; t += 1
    return tl
for rate in (0.0, 0.25, 0.5):
    m = metrics(STR, sim_age(STR, rate)); print("  SJF with ageing rate %.2f   Long job waits %3d units (finishes at %d)" % (rate, m["Long"][2], m["Long"][0]))
