# CSA0404 - Operating Systems - Assignment 4 (CO4): Optimising File Storage and Disk Access

Name: Mohammed Subahaan H
Register number: 192572165
Course: Operating Systems (CSA0404)
GitHub: https://github.com/Subahaan-cmd/CSA-0404-OPERATING-SYSTEM

Compiler: gcc 13.3.0 | Linux: Ubuntu 24.04.5 LTS (kernel 6.18) | Python 3.12.3
Compile command: gcc -std=c11 -Wall -Wextra -O2 <file>.c -o <file> -lm   (no warnings)

## Folders and results
Task01/  setup.sh                      Improved file-system layout: tree, 0700 homes, ACLs, setgid group folder, quota
Task02/  task2.c, t2b.c, frag.c,       First-fit contiguous allocation completed; output equals the expected output;
         inode_calc.py, *_output.txt   14-block file fails contiguous (-1) but succeeds indexed; inode max size 4,402,345,721,856 bytes; bit vector 128 MiB
Task03/  syncbench.c, stat/df/ls       ext4 on /dev/vda, 4096-byte blocks; fsync costs 14x a buffered write
Task04/  task4.c, sim4.c, starve.c     FCFS 640, SSTF 236, SCAN 236, C-SCAN 183 (+199 jump), LOOK 208, C-LOOK 322; SSTF starves cylinder 183
Task05/  notes.md                      Written answer in the PDF report
Task06/  sim6.py, streams.fio          28 HDD + 10 percent NVMe cache design; hit ratio 0.649; average start 18.9 ms

## How to run
cd Task02 && gcc -std=c11 -Wall -Wextra task2.c -o task2 && ./task2
cd Task04 && gcc -std=c11 -Wall -Wextra task4.c -o task4 && ./task4
cd Task04 && gcc -std=c11 -Wall -Wextra -O2 sim4.c -o sim4 -lm && ./sim4
cd Task04 && gcc -std=c11 -Wall -Wextra -O2 starve.c -o starve && ./starve
cd Task06 && python3 sim6.py
