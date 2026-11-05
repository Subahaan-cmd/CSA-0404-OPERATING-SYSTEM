import sys; sys.path.insert(0, '../..')
import matplotlib; matplotlib.use('Agg'); import matplotlib.pyplot as plt
plt.rcParams['font.family'] = 'serif'; plt.rcParams['font.serif'] = ['Liberation Serif', 'DejaVu Serif']
rows = [l.strip().split(',') for l in open('faults.csv')]
fig, axs = plt.subplots(1, 2, figsize=(10, 3.8))
for ax, kind, title in zip(axs, ('locality', 'random'), ('Strong locality', 'Random access')):
    d = [r for r in rows if r[0] == kind]; fr = [int(r[1]) for r in d]
    for i, n in enumerate(('FIFO', 'LRU', 'Clock', 'Optimal')): ax.plot(fr, [int(r[2 + i]) for r in d], 'o-', label=n, ms=4)
    ax.set_xlabel('frames'); ax.set_ylabel('page faults (100,000 refs)'); ax.set_title(title); ax.grid(alpha=.3); ax.legend()
fig.suptitle('Figure 5.1 Page faults against the number of frames'); fig.tight_layout(); fig.savefig('fig_faults.png', dpi=150)
# small reference string: faults vs frames 1..7
def fifo(r, f):
    q = []; n = 0
    for p in r:
        if p in q: continue
        n += 1
        if len(q) == f: q.pop(0)
        q.append(p)
    return n
def lru(r, f):
    q = []; n = 0
    for p in r:
        if p in q: q.remove(p); q.append(p); continue
        n += 1
        if len(q) == f: q.pop(0)
        q.append(p)
    return n
def opt(r, f):
    fr = []; n = 0
    for i, p in enumerate(r):
        if p in fr: continue
        n += 1
        if len(fr) < f: fr.append(p); continue
        def nu(x):
            try: return r.index(x, i + 1)
            except ValueError: return 10**9
        fr.remove(max(fr, key=nu)); fr.append(p)
    return n
b = [1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5]; w = [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1]
fig, axs = plt.subplots(1, 2, figsize=(10, 3.6))
for ax, s, t in ((axs[0], b, "Belady's string (12 refs)"), (axs[1], w, 'Textbook string (20 refs)')):
    fs = list(range(1, 8))
    for fn, nm in ((fifo, 'FIFO'), (lru, 'LRU'), (opt, 'Optimal')): ax.plot(fs, [fn(s, f) for f in fs], 'o-', label=nm, ms=4)
    ax.set_xlabel('frames'); ax.set_ylabel('page faults'); ax.set_title(t); ax.grid(alpha=.3); ax.legend()
fig.suptitle('Figure 4.2 Page faults against the number of frames for a reference string'); fig.tight_layout(); fig.savefig('fig_belady.png', dpi=150)
print({f: (fifo(b, f), lru(b, f), opt(b, f)) for f in range(1, 8)})
