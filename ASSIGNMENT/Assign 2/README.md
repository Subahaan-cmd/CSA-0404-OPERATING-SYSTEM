# CSA0404 - Operating System - Assignment 2 (CO2)
## Process Management, Scheduling, Synchronisation and Deadlock Handling

Name: Mohammed Subahaan H
Register Number: 192572165
Course: Operating System
Course Code: CSA0404
GitHub Repository: https://github.com/Subahaan-cmd/CSA-0404-OPERATING-SYSTEM

Compiler: gcc 13.3.0
Linux distribution: Ubuntu 24.04 LTS (Linux kernel 6.x)
Compile command: `gcc -std=c11 -Wall -Wextra -pthread -O2 file.c -o file`

Report: `CSA0404_Assignment2_CO2.pdf`  |  Source code: `source/`

Notes: add `-lrt` when compiling `q4_bench.c`. Python 3.12 is needed for `q2_sched.py`, `q6_detect.py`, `ext.py` and `figs.py` (matplotlib for figures).

## How to run each program

| Q | File(s) | Compile | Run |
|---|---|---|---|
| 1 | q1_fork.c | `gcc -std=c11 -Wall -Wextra -pthread -O2 -o q1_fork q1_fork.c` | `./q1_fork 5` |
| 1 | q1_ctx.c | same pattern | `./q1_ctx` |
| 1 | q1_mem.c | same pattern | `./q1_mem p` and `./q1_mem t` |
| 2 | q2_rr.c (completed code) | `gcc -std=c11 -Wall -Wextra -o q2_rr q2_rr.c` (add `-DQUANTUM=1`, `5` or `20` for the quantum study) | `./q2_rr` |
| 2 | q2_sched.py | - | `python3 q2_sched.py` |
| 3 | q3_cameras.c | `gcc -std=c11 -Wall -Wextra -pthread -O2 -o q3_cameras q3_cameras.c` | `./q3_cameras` |
| 3 | q3_race.c, q3_proc.c | same pattern | `./q3_race`, `./q3_proc` |
| 4 | q4_pipe.c (completed code) | `gcc -std=c11 -Wall -Wextra -O2 -o q4_pipe q4_pipe.c` | `./q4_pipe` |
| 4 | q4_noclose.c | same | `timeout 2 ./q4_noclose; echo $?` (exit 124 = hangs) |
| 4 | q4_shm.c, q4_bench.c | `gcc ... -pthread -O2 -lrt` | `./q4_shm`, `./q4_bench` |
| 5 | q5_bb.c (completed code) | `gcc -std=c11 -Wall -Wextra -pthread -O2 -o q5_bb q5_bb.c` | `./q5_bb`; 100 runs: `for i in $(seq 100); do ./q5_bb; done | sort | uniq -c` |
| 5 | q5_bbmulti.c | same (add `-DNOLOCK` to remove the mutex) | `./q5_bbmulti 1`, `./q5_bbmulti 4` |
| 5 | q5_lock.c, q5_phil.c, q5_rw.c, q5_monitor.c | `gcc ... -pthread -O2` | run each |
| 6 | q6_banker.c (completed code) | `gcc -std=c11 -Wall -Wextra -O2 -o q6_banker q6_banker.c` | `./q6_banker` |
| 6 | q6_bench.c | same | `./q6_bench` |
| 6 | q6_detect.py | - | `python3 q6_detect.py` |

## Result of each question

| Q | Result |
|---|---|
| 1 Processes | State diagram and PCB; fork program shows children in ps and /proc; fixed worker pool chosen over process-per-student |
| 2 Scheduling | Round Robin output identical to expected (average TAT 20.00, WT 13.50). Averages WT: FCFS 8.75, SJF 7.75, SRTF 6.50, Priority 10.25. Context switches for RR q=1/3/5/20: 23/9/5/3. MLFQ with ageing recommended |
| 3 Threads | Speed-up 1.0-1.08 for CPU-only work on a 1-core machine, 2.0 with I/O waits; unprotected counter lost 14.8 million of 40 million updates |
| 4 IPC | Pipe output identical to expected; removing close(fd[1]) hangs (exit 124); 100 MB: pipe 13.8 ms, shared memory 5.2 ms |
| 5 Synchronisation | Sum 210 in all 100 runs; 1 and 4 producers/consumers CORRECT; without mutex WRONG; lock per coach chosen; atomic add 8.6 ns vs mutex 19 ns |
| 6 Deadlock | Output identical to expected (safe sequence P1 P3 P4 P0 P2; P4 waits; P0 denied). Worst case O(m n^2): 504 us for 1,000 processes. Recommendation: resource ordering + detection and recovery |

## Limitations

The test machine had 1 CPU, so thread and lock speed-ups are limited. Repeat the experiments on a multi-core machine and add screenshots to this folder.
