import random
print("== Page-table size (single level, 4-byte entries for 32-bit, 8-byte entries for 48-bit), page size 4 KB")
for bits, e in ((32, 4), (48, 8)):
    entries = 2 ** (bits - 12)
    print("  %d-bit address space: %s entries x %d bytes = %s" % (bits, format(entries, ','), e, "%.0f MB" % (entries * e / 2**20) if entries * e < 2**40 else "%.0f GB" % (entries * e / 2**30)))
print("  (48-bit: 4-level table: only used parts exist; a 10 MB process needs about 1 + 1 + 1 + 3 = 6 pages = 24 KB)")
print("\n== Effective memory-access time, memory 100 ns, TLB 10 ns, 1-level table")
def eat(h, mem=100, tlb=10, levels=1): return h * (tlb + mem) + (1 - h) * (tlb + levels * mem + mem)
print("  without TLB (1 level): %d ns; (4 levels): %d ns" % (2 * 100, 5 * 100))
for h in (0.80, 0.95, 0.99):
    print("  hit ratio %2d%%: 1 level %.1f ns  (slow-down %.0f%% over 100 ns); 4 levels %.1f ns" % (h * 100, eat(h), (eat(h) - 100), eat(h, levels=4)))
print("\n== Internal fragmentation for 100 processes with random sizes (seed 42, sizes 10 KB - 5 MB; 2 MB pages use 1-64 MB processes)")
random.seed(42)
sizes = [random.randint(10 * 1024, 5 * 1024 * 1024) for _ in range(100)]
big = [random.randint(1 * 2**20, 64 * 2**20) for _ in range(100)]
for ps, lab, data in ((1024, "1 KB", sizes), (4096, "4 KB", sizes), (2 * 2**20, "2 MB", big)):
    waste = sum((-s) % ps for s in data); total = sum(data)
    print("  page %5s: total memory requested %9.1f MB, internal fragmentation %10.1f KB (%.4f%%), average %.1f KB per process, pages needed %d" % (lab, total / 2**20, waste / 1024, 100 * waste / total, waste / 1024 / 100, sum(-(-s // ps) for s in data)))
print("  expected waste per process = page size / 2: 0.5 KB, 2 KB, 1024 KB")
