import math
n, tb = 6, 4
print("Usable capacity of 6 disks x 4 TB")
for name, cap, tol, rd, wr in (("RAID 0", n * tb, "0", "6x", "6x"), ("RAID 1 (3 mirrored pairs)", n * tb // 2, "1 (up to 3, one per pair)", "up to 6x", "3x"), ("RAID 5", (n - 1) * tb, "1", "5x", "about 1.5x (4 I/Os per small write)"),
                                  ("RAID 6", (n - 2) * tb, "2", "4x", "about 1x (6 I/Os per small write)"), ("RAID 10 (3 mirrored pairs striped)", n * tb // 2, "1 per pair (at least 1, up to 3)", "6x", "3x")):
    print("  %-36s usable %2d TB (%.0f%%)  survives %-30s read %s  write %s" % (name, cap, 100 * cap / (n * tb), tol, rd, wr))
print("\nProbability of data loss during one rebuild (a disk has failed; annual failure rate AFR = 3%, rebuild takes 24 h)")
afr = 0.03; hours = 24; p = afr * hours / 8760     # probability that one given disk fails during the rebuild window
print("  p(one given surviving disk fails during rebuild) = %.2e" % p)
print("  RAID 1 (pair): the single partner disk must fail:           P = %.2e" % p)
print("  RAID 5 (6 disks): any of the 5 survivors fails:             P = 1-(1-p)^5 = %.2e   (about %.1fx RAID 1)" % (1 - (1 - p) ** 5, (1 - (1 - p) ** 5) / p))
print("  RAID 6 (6 disks): two more of the 5 survivors must fail:    P = C(5,2) p^2 = %.2e" % (10 * p * p))
print("  RAID 10 (3 pairs): only the failed disk's partner matters:  P = %.2e" % p)
ure = 1e-14   # unrecoverable read error per bit
bits = 5 * 4e12 * 8
print("  Rebuild of RAID 5 reads 5 x 4 TB = %.0e bits; with URE rate 1e-14 per bit expected read errors = %.2f -> P(at least one) = %.0f%%" % (bits, bits * ure, 100 * (1 - math.exp(-bits * ure))))
print("  RAID 1/10 rebuild reads only 4 TB = %.1e bits: P(URE) = %.0f%%" % (4e12 * 8, 100 * (1 - math.exp(-4e12 * 8 * ure))))
print("\nSwap sizing example: 64 GB RAM records server, hibernation not used: swap = 8-16 GB (safety margin for peaks) on SSD; swappiness 10")
