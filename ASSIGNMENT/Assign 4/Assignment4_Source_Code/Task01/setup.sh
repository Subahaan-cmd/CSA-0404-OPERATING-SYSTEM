#!/usr/bin/env bash
# Task 1.2: directory tree, permissions, ACLs, group project folder and quota (run as root)
mkdir -p /home/CSE/2025 /projects/CSA0404
chmod 711 /home/CSE /home/CSE/2025
useradd -m -d /home/CSE/2025/192572165 -s /bin/bash 192572165
chmod 700 /home/CSE/2025/192572165
setfacl -m g:faculty:rx /home/CSE/2025/192572165
mkdir -p /home/CSE/2025/192572165/Courses/CSA0404
setfacl -R -m g:faculty:rX /home/CSE/2025/192572165/Courses
groupadd csa0404-g07
mkdir /projects/CSA0404/G07
chgrp csa0404-g07 /projects/CSA0404/G07
chmod 2770 /projects/CSA0404/G07
usermod -aG csa0404-g07 192572165
ln -s /projects/CSA0404/G07 /home/CSE/2025/192572165/Courses/CSA0404/group
setquota -u 192572165 5000000 6000000 200000 250000 /home
