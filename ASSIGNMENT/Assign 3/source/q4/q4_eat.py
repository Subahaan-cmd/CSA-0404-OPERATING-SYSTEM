mem, svc = 100e-9, 8e-3
print("== Effective access time EAT = (1-p) x 100 ns + p x 8 ms")
for lab, p in (("1 in 1,000", 1e-3), ("1 in 100,000", 1e-5), ("1 in 1,000,000", 1e-6)):
    eat = (1 - p) * mem + p * svc
    print("  p = %-15s EAT = %9.1f ns  (slow-down factor %.1f)" % (lab, eat * 1e9, eat / mem))
p10 = 0.1 * mem / (svc - mem)
print("\n== Maximum page-fault rate for a 10%% slow-down: EAT <= 110 ns -> p <= 10 ns / (8,000,000 ns - 100 ns) = %.3e  (about 1 fault in %d accesses)" % (p10, round(1 / p10)))
print("\n== 24 GB program on 16 GB RAM: if the program touches memory with locality so that p = 1e-6, EAT = %.1f ns" % (((1 - 1e-6) * 100e-9 + 1e-6 * 8e-3) * 1e9))
print("== 32 GB machine, no faults after loading: EAT = 100 ns; with p=1e-6 on the small machine EAT = 108 ns (8% slower)")
print("\n== Equal and proportional frame allocation: 62 frames, processes of 10 and 127 pages")
print("  equal: 31 and 31 frames; proportional: 62 x 10/137 = %.1f -> 4, and 62 x 127/137 = %.1f -> 57 frames" % (62 * 10 / 137, 62 * 127 / 137))
