"""Appendix G: extended calculations used in the discussion (page tables, EAT sweep, loss probabilities, swap sizing)."""
print("== Page-table size for different page sizes (single level, 4-byte entries, 32-bit address space = 4 GB)")
print("%10s %14s %16s %18s" % ("page size", "pages", "table size", "offset bits"))
for ps, nm in ((512, "512 B"), (1024, "1 KB"), (4096, "4 KB"), (16384, "16 KB"), (65536, "64 KB"), (2 << 20, "2 MB"), (4 << 20, "4 MB")):
    n = 2 ** 32 // ps; print("%10s %14s %16s %18d" % (nm, format(n, ","), "%.1f KB" % (n * 4 / 1024) if n * 4 < 2 ** 20 else "%.1f MB" % (n * 4 / 2 ** 20), ps.bit_length() - 1))
print("\n== Effective access time (ns) against TLB hit ratio, memory 100 ns, TLB 10 ns")
print("%8s %12s %12s %12s" % ("hit", "1 level", "2 levels", "4 levels"))
for h in (0.50, 0.60, 0.70, 0.80, 0.90, 0.95, 0.98, 0.99, 0.995, 0.999):
    f = lambda L: h * 110 + (1 - h) * (10 + L * 100 + 100)
    print("%8.3f %12.1f %12.1f %12.1f" % (h, f(1), f(2), f(4)))
print("\n== Demand paging: EAT (ns) for fault rate p and service time 8 ms (hard disk), 100 us (SSD)")
print("%12s %14s %14s" % ("p", "hard disk", "SSD"))
for p in (1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8):
    print("%12.0e %14.1f %14.1f" % (p, (1 - p) * 100 + p * 8e6, (1 - p) * 100 + p * 1e5))
print("\n== Maximum fault rate for a 10%% slow-down: hard disk %.2e, SSD (100 us) %.2e" % (10 / (8e6 - 100), 10 / (1e5 - 100)))
print("\n== Probability of losing a RAID 5 / RAID 6 / RAID 10 set during a 24-hour rebuild for different annual failure rates (6 disks)")
print("%6s %14s %14s %14s" % ("AFR", "RAID 5", "RAID 6", "RAID 10"))
for afr in (0.01, 0.02, 0.03, 0.05, 0.08):
    p = afr * 24 / 8760
    print("%5.0f%% %14.2e %14.2e %14.2e" % (afr * 100, 1 - (1 - p) ** 5, 10 * p * p, p))
print("\n== Swap sizing rules of thumb (no hibernation)")
for ram in (2, 4, 8, 16, 64):
    print("  RAM %3d GB: swap %5.1f - %5.1f GB" % (ram, ram * (1.0 if ram <= 2 else 0.5 if ram <= 8 else 0.25 if ram <= 16 else 0.15), ram * (2.0 if ram <= 2 else 1.0 if ram <= 8 else 0.5 if ram <= 16 else 0.25)))
print("\n== Number of page-table pages for one process mapping m MB contiguously (x86-64, 4 KB pages, 512 entries per table)")
for mb in (1, 10, 100, 1024, 24 * 1024):
    leaf = -(-mb * 256 // 512); l2 = -(-leaf // 512); l3 = -(-l2 // 512); total = leaf + l2 + l3 + 1
    print("  %6d MB: leaf tables %6d, level-2 %4d, level-3 %2d, root 1 -> %6d pages = %8.1f KB (%.3f%% of the mapped memory)" % (mb, leaf, l2, l3, total, total * 4, 100 * total * 4 / (mb * 1024)))
