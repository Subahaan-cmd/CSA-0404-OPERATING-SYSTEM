# CSA0404 - Operating System - Assignment 1 (CO1)
## Evaluate and Justify the Operating-System Design of a Smart Hospital

Name: Mohammed Subahaan H
Register Number: 192572165
Course: Operating System
Course Code: CSA0404
GitHub Repository: https://github.com/Subahaan-cmd/CSA-0404-OPERATING-SYSTEM

Compiler: gcc 13.3.0
Linux distribution: Ubuntu 24.04 LTS (Linux kernel 6.x)
Compile command: `gcc -std=c11 -Wall -Wextra -pthread -O2 file.c -o file`

Report: `CSA0404_Assignment1_CO1.pdf`  |  Source code: `source/`

Notes: programs that use POSIX/GNU functions were compiled with `-std=gnu11` or `-D_POSIX_C_SOURCE=200809L` as stated in each file. `mystrace.c` needs `-D_GNU_SOURCE` and runs on x86-64 Linux only. On a machine with strace installed, use `strace -c ./program` instead of `./mystrace -c ./program`.

## How to run each program

| Task | File(s) | Compile | Run |
|---|---|---|---|
| 5 | t05_demo.c, mystrace.c | `gcc -std=c11 -Wall -Wextra -o t05_demo t05_demo.c` ; `gcc -std=c11 -D_GNU_SOURCE -o mystrace mystrace.c` | `./mystrace ./t05_demo` |
| 5 | t05_stdio.c | `gcc -std=c11 -Wall -Wextra -o t05_stdio t05_stdio.c` | `./mystrace -c ./t05_stdio` |
| 6 | t06_copy.c (completed code) | `gcc -std=c11 -Wall -Wextra -o t06_copy t06_copy.c` | `./t06_copy` |
| 6, 10 | t06_bench.c | `gcc -std=c11 -Wall -Wextra -o t06_bench t06_bench.c` | `./t06_bench` |
| 7 | t07_shell.c (completed code) | `gcc -std=c11 -Wall -Wextra -o t07_shell t07_shell.c` | `./t07_shell` |
| 7 | t07_noflush.c | `gcc -std=c11 -Wall -Wextra -o t07_noflush t07_noflush.c` | `./t07_noflush > out.txt; cat out.txt` |
| 7 | t07_zombie.c | `gcc -std=c11 -Wall -Wextra -o t07_zombie t07_zombie.c` | `./t07_zombie` |
| 8 | t08_polling.c | `gcc -std=c11 -Wall -Wextra -O2 -o t08_polling t08_polling.c` | `./t08_polling` |
| 9 | t09_latency.c | `gcc -std=c11 -Wall -Wextra -O2 -o t09_latency t09_latency.c` | `./t09_latency` |
| 7, 10 | t10_create.c | `gcc -std=c11 -Wall -Wextra -pthread -O2 -o t10_create t10_create.c` | `./t10_create` and `./t10_create 200` |
| figures | figs.py | needs python3 + matplotlib | `python3 figs.py` |

## Result of each task

| Task | Result |
|---|---|
| 1 Requirements | Ranked requirement table for 6 environments; a single general-purpose OS cannot bound the 10 ms alarm delay |
| 2 OS types | Hard real-time for monitors, handheld for tablets, desktop for workstations, clustered Linux for database, SMP for imaging, distributed services for partner link |
| 3 Structures | Microkernel/protected RTOS for monitors; modular monolithic Linux for database server |
| 4 Services | Service table per environment; monitor drops UI, accounting, file system, virtual memory |
| 5 System calls | read() path explained in 7 steps; trace of t05_demo; stdio needs about 3 read calls for 10,000 byte-reads instead of 10,000 |
| 6 Cost of syscalls | Output 10001 / 21 / 4 read() calls (matches expected). 4 MB copy: 4,194,304 calls and 2.33 s with 1-byte buffer; 4 calls and 0.003 s with 1 MB; 256 KB buffer recommended |
| 7 Process control | Output matches expected; with the second fflush removed and exit(3), "shell: running job 2" appears twice; zombie shown as state Z |
| 8 CPU states | Polling every 50 ms: 47 ms delay; 1 ms: 0.94 ms; interrupt-style: 0.15 ms and almost no CPU |
| 9 Memory/I/O | Measured latency 1.8 ns (L1) to 146 ns (RAM); DMA needs 8,000 interrupts vs 131 million for per-word interrupts on 500 MB |
| 10 Measurements | Thread create 10 us, fork 60 us (2,121 us with 200 MB parent), posix_spawn 681 us, fork+exec 880 us |
| 11 Final plan | Three OS families (RTOS, handheld, Linux/desktop) with ward gateway; VM consolidation only for workstations, gateways and test |
| 12 Day-4 challenge | MODIFY: embedded RTOS on home devices, replicated cloud records service with local replica, zero-trust security |

## Limitations

The test machine was a virtual machine with 1 CPU; absolute times are noisy, but ratios are valid. Re-run on your own machine and add screenshots to this folder.
