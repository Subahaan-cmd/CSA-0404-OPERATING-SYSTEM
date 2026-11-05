"""Q7 Stage 3: simulate the memory use of the three tasks of the edge device (2 GB RAM, no swap) with a page-replacement simulator.
Pages are 4 KB.  Only FILE-BACKED pages (code, model file, log file) can be evicted: there is no swap for anonymous memory.
Frames available for evictable pages = 2 GB - OS/kernel (300 MB) - locked RT task (64 MB) - anonymous heap of ML (1.2 GB) - logging buffers (40 MB)."""
import random, collections
random.seed(5)
MB = 256                      # pages per MB
RAM = 2048 * MB
kernel, rt_locked, ml_heap, log_buf = 300 * MB, 64 * MB, 1200 * MB, 40 * MB
frames = RAM - kernel - rt_locked - ml_heap - log_buf
print("evictable frames: %d pages = %.0f MB" % (frames, frames / MB))
# reference streams (file-backed): RT code/tables 20 MB hot; ML model file 300 MB read in sweeps; logging appends sequentially
def stream(n, pinned):
    rt_pages = list(range(0, 20 * MB, 1)); ml_base = 100000; log_base = 500000
    refs = []; ml_pos = 0; log_pos = 0
    for i in range(n):
        r = random.random()
        if r < 0.50: refs.append(("RT", random.choice(rt_pages[:5 * MB])))                 # RT task: hot 5 MB of its 20 MB
        elif r < 0.90: refs.append(("ML", ml_base + ml_pos)); ml_pos = (ml_pos + 1) % (300 * MB // 4 * 4)  # sequential sweep of the 300 MB model file
        else: refs.append(("LOG", log_base + log_pos)); log_pos += 1                        # sequential log
    return refs
def simulate(refs, nframes, pin_rt):
    lru = collections.OrderedDict(); faults = collections.Counter(); total = collections.Counter()
    for task, pg in refs:
        total[task] += 1
        if pin_rt and task == "RT": continue               # locked with mlock(): always resident, never faults
        if pg in lru: lru.move_to_end(pg); continue
        faults[task] += 1
        if len(lru) >= nframes: lru.popitem(last=False)
        lru[pg] = 1
    return faults, total
refs = stream(2_000_000, False)
for pin in (False, True):
    # memory pressure: only 8% of the evictable frames are left after the fourth task / peak (to expose the effect)
    nf = int(frames * 0.02)
    f, t = simulate(refs, nf, pin)
    print("\nRT task %s, %d frames for evictable pages (%.0f MB):" % ("LOCKED (mlock)" if pin else "NOT locked", nf, nf / MB))
    for k in ("RT", "ML", "LOG"): print("  %-4s references %8d  page faults %8d  fault rate %.4f%%" % (k, t[k], f[k], 100 * f[k] / t[k]))
print("\nFourth task of 500 MB added: free memory before = %d MB -> after = %d MB" % (2048 - 300 - 64 - 1200 - 40 - 20, 2048 - 300 - 64 - 1200 - 40 - 20 - 500))
