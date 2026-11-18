# Task 2.3 / 2.4 calculations
B = 4096; ptr = 4
n = B // ptr
print("pointers per block", n)
maxblocks = 12 + n + n**2 + n**3
print("max blocks", maxblocks, "max size bytes", maxblocks * B, "GiB", maxblocks * B / 2**30, "TiB", maxblocks * B / 2**40)
fileblocks = 4 * 2**30 // B
print("4GB file blocks", fileblocks)
blocks = 4 * 2**40 // B
print("4TB blocks", blocks, "bitvector bytes", blocks // 8, "MiB", blocks / 8 / 2**20)
print("FAT entries 4TB 4-byte", blocks * 4 / 2**30, "GiB")
