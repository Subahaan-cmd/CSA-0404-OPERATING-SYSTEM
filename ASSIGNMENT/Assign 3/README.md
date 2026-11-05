# CSA0404 - Operating System - Assignment 3 (CO3)
## Memory Management: Linking, Paging, Segmentation, Virtual Memory, Page Replacement and Disk Management

Name: Mohammed Subahaan H
Register Number: 192572165
Course: Operating System
Course Code: CSA0404
GitHub Repository: https://github.com/Subahaan-cmd/CSA-0404-OPERATING-SYSTEM

Compiler: gcc 13.3.0
Linux distribution: Ubuntu 24.04 LTS (Linux kernel 6.x)
Compile command: `gcc -std=c11 -Wall -Wextra -pthread -O2 file.c -o file`

Report: `CSA0404_Assignment3_CO3.pdf`  |  Source code: `source/q1 ... q7` (one folder per question)

Notes: programs using mmap, dlopen or getrusage were compiled with `-std=gnu11`. Python 3.12 is needed for the `.py` files (matplotlib for `plot.py`).

## How to run each program

| Q | Folder / file | Steps |
|---|---|---|
| 1 | source/q1 | `gcc -fPIC -shared -o libenc.so libenc.c` ; `gcc -fPIC -shared -o libenc_v2.so libenc_v2.c` ; `gcc -std=gnu11 -O2 -o app_dl app_dl.c -ldl` ; then `bash demo.sh` (builds static and dynamic versions, replaces the library, tests missing library, start-up time, shared pages) |
| 2 | q2_page.c (completed code) | `gcc -std=c11 -Wall -Wextra -o q2_page q2_page.c && ./q2_page` ; for 2048: add `-DPAGE_SIZE_VAL=2048` |
| 2 | q2_edge.c, q2_faults.c, q2_calc.py, ext_calc.py | `gcc -std=gnu11 -O2 -o q2_edge q2_edge.c && ./q2_edge` ; `python3 q2_calc.py` |
| 3 | q3_seg.c, q3_segv.c | `gcc -std=gnu11 -O0 -o q3_seg q3_seg.c && ./q3_seg` ; `./q3_segv` and `./q3_segv ro` |
| 3 | q3_frag.py, maps.sh | `python3 q3_frag.py` ; `bash maps.sh` |
| 4 | q4_eat.py, q4_touch.c, q4_cow.c | `python3 q4_eat.py` ; `gcc -std=gnu11 -O2 -o q4_touch q4_touch.c && ./q4_touch` ; same for q4_cow |
| 5 | q5_replace.c (completed code) | `gcc -std=c11 -Wall -Wextra -o q5_replace q5_replace.c && ./q5_replace` |
| 5 | q5_exp.c, plot.py, q5_trace.py | `gcc -std=c11 -O2 -o q5_exp q5_exp.c && ./q5_exp` ; `./q5_exp csv > faults.csv && python3 plot.py` |
| 6 | q6_raid.py, sys.sh, dd.sh | `python3 q6_raid.py` ; `bash sys.sh` ; `bash dd.sh` |
| 7 | q7_mlock.c, q7_sim.py | `gcc -std=gnu11 -O2 -o q7_mlock q7_mlock.c && ./q7_mlock` ; `python3 q7_sim.py` |

## Result of each question

| Q | Result |
|---|---|
| 1 Linking | Static 785,704 bytes vs dynamic 16,472 bytes; library fix picked up without recompiling (dynamic only); missing library: exit status 127; start-up 0.85 ms (static) vs 1.10 ms (dynamic); dynamic linking recommended |
| 2 Paging | Output identical to expected; PAGE_SIZE 2048 matches hand calculation; page table 4 MB (32-bit) and 512 GB (48-bit single level); EAT 130/115/111 ns for hit ratios 80/95/99 %; 4 KB pages chosen |
| 3 Segmentation | 8 translations (3 valid, 5 traps); SEGV_MAPERR and SEGV_ACCERR shown; 76.4 % external fragmentation after removing 10 of 20 segments |
| 4 Virtual memory | EAT 8,099.9 / 180 / 108 ns; max fault rate for 10 % slow-down 1.25e-6; fork of 100 MB parent shares 103 MB, 12,800 COW faults for 50 MB written |
| 5 Page replacement | Output identical to expected (FIFO 15/10, LRU 12/8, Optimal 9/8; Belady 9 to 10); LRU about half the faults of FIFO under locality; Clock between them; LRU/Clock recommended |
| 6 Disk management | Capacities 24/12/20/16/12 TB for RAID 0/1/5/6/10; RAID 10 recommended for the database, RAID 6 for archive; system has no swap |
| 7 Memory plan | mlock: worst-case delay 172 us to 3.3 us (anonymous), no page faults; simulator: RT task 33 % faults unlocked, 0 % locked |

## Limitations

The test machine was a VM with 1 CPU, 4 GB RAM, no swap and only an SSD, so the run-time-beyond-RAM experiment and the SSD-vs-hard-disk comparison could not be run. Repeat them on your machine and add screenshots to this folder.
