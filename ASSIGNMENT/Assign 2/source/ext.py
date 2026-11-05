"""Appendix E: extended calculations (Amdahl speed-up, round-robin overhead, Banker's cost, queueing delay of 2,000 sessions)."""
print("== Amdahl speed-up S(p) = 1 / (s + (1-s)/p) for different serial fractions s")
print("%6s %8s %8s %8s %8s" % ("cores", "s=0.01", "s=0.05", "s=0.10", "s=0.25"))
for p in (1, 2, 4, 8, 16, 32, 64, 1024):
    print("%6d %8.2f %8.2f %8.2f %8.2f" % ((p,) + tuple(1 / (s + (1 - s) / p) for s in (0.01, 0.05, 0.10, 0.25))))
print("   limit as p grows: 100, 20, 10, 4")
print("\n== Round-robin overhead: fraction of CPU lost = c / (q + c) for context-switch cost c")
print("%10s %12s %12s %12s" % ("quantum", "c=1 us", "c=5 us", "c=50 us"))
for q in (10e-6, 100e-6, 1e-3, 4e-3, 10e-3, 100e-3):
    print("%8.0f us %11.2f%% %11.2f%% %11.2f%%" % ((q * 1e6,) + tuple(100 * c / (q + c) for c in (1e-6, 5e-6, 50e-6))))
print("\n== Waiting between turns for N runnable sessions on C cores with quantum q: N/C x q")
for n in (100, 500, 2000, 10000):
    print("  N = %5d: 4 cores, q = 4 ms -> %.2f s; 16 cores -> %.2f s" % (n, n / 4 * 0.004, n / 16 * 0.004))
print("\n== Safety-algorithm work (process checks) in the worst case n(n+1)/2 and time at 1.0 ns per process check (measured: 504 us for 500,500 checks), m = 3")
for n in (10, 100, 1000, 10000, 100000):
    chk = n * (n + 1) // 2; print("  n = %6d: %16s checks, about %10.3f ms" % (n, format(chk, ","), chk * 1.0e-6))
print("\n== Mutual exclusion: probability that two random bookings conflict")
for name, locks in (("one global lock", 1), ("lock per train (20)", 20), ("lock per coach (200)", 200), ("lock per seat (14,400)", 14400)):
    print("  %-24s %.5f" % (name, 1 / locks))
print("\n== Pipe capacity: readings (4-byte ints) that fit in the 64 KB pipe buffer =", 65536 // 4)
print("== Thread stack memory for 2,000 sessions: 2,000 x 8 MB virtual = 16 GB virtual, resident only touched pages (about 2,000 x 24 KB = 47 MB)")
