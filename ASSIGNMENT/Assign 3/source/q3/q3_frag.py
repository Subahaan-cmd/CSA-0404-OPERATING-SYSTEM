"""Load and remove 20 segments of random sizes in 1024 KB of memory (first fit) and measure external fragmentation."""
import random
random.seed(7)
MEM = 1024
free = [(0, MEM)]            # list of (start, size) holes
placed = {}
def alloc(i, size):
    for k, (s, sz) in enumerate(free):
        if sz >= size:
            placed[i] = (s, size)
            if sz == size: free.pop(k)
            else: free[k] = (s + size, sz - size)
            return True
    return False
def release(i):
    s, sz = placed.pop(i); free.append((s, sz)); free.sort()
    merged = []
    for a, b in free:
        if merged and merged[-1][0] + merged[-1][1] == a: merged[-1] = (merged[-1][0], merged[-1][1] + b)
        else: merged.append((a, b))
    free[:] = merged
sizes = [random.randint(20, 120) for _ in range(20)]
for i, s in enumerate(sizes): alloc(i, s)
print("20 segments loaded, sizes (KB):", sizes, " total =", sum(sizes), "KB")
for i in random.sample(range(20), 10): release(i)
tot = sum(s for _, s in free); big = max(s for _, s in free)
print("after removing 10 segments: free holes =", free)
print("  total free = %d KB, largest hole = %d KB, number of holes = %d" % (tot, big, len(free)))
print("  external fragmentation = 1 - largest/total = %.1f %%" % (100 * (1 - big / tot)))
req = big + 1
print("  a new segment of %d KB (smaller than the %d KB free) can be placed: %s" % (req, tot, alloc(99, req)))
free.sort()
# compaction
print("  after compaction one hole of %d KB would exist (cost: copy %d KB of live segments)" % (tot, sum(s for _, s in placed.values())))
print("\nTable sizes for a 4 GB process using 4 segments: segment table = 4 entries x 8 bytes = 32 bytes")
print("Single-level page table (4 KB pages, 4 GB): 1,048,576 entries x 4 bytes = 4 MB; two-level: 4 KB top + used 4 KB leaf tables")
