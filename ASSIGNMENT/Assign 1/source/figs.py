import sys; sys.path.insert(0, '..')
from figlib import *
import numpy as np

# 1 hospital architecture
fig, ax = canvas(10, 6.2)
box(ax, 2, 50, 20, 9, 'Bedside monitors\n(hard real-time RTOS)', '#F8D7D7')
box(ax, 26, 50, 20, 9, 'Nurse tablets\n(handheld OS: Android)', '#DDF0DD')
box(ax, 50, 50, 22, 9, 'Doctor workstations\n(desktop OS: Linux/Windows)', '#FFF2CC')
box(ax, 76, 50, 22, 9, 'Partner-hospital link\n(distributed services, TLS)', '#E0D8F0')
box(ax, 14, 28, 30, 10, 'Ward gateway / message broker\n(soft real-time Linux, PREEMPT_RT)', '#F5F5F5')
box(ax, 52, 28, 46, 10, 'Hospital network (VLANs, firewall, VPN)', '#F5F5F5')
box(ax, 4, 4, 30, 12, 'Database cluster (2-3 nodes)\nLinux, fail-over in seconds', '#CFE2F3')
box(ax, 38, 4, 30, 12, 'Imaging server\nLinux SMP, 32+ cores, DMA/NVMe', '#CFE2F3')
box(ax, 72, 4, 26, 12, 'Backup / archive\n(RAID 10, off-site copy)', '#CFE2F3')
for x in (12, 36, 61, 87): arrow(ax, x, 50, x, 38.5, '', '<->')
arrow(ax, 29, 28, 29, 16.5, '', '<->'); arrow(ax, 53, 28, 53, 16.5, '', '<->'); arrow(ax, 85, 28, 85, 16.5, '', '<->')
ax.text(50, 63, 'Figure 1.1 Proposed heterogeneous operating-system plan for the 500-bed hospital', ha='center', fontsize=11, fontweight='bold')
save(fig, 'fig_hospital.png')

# 2 syscall flow
fig, ax = canvas(10, 6.4)
ax.add_patch(plt.Rectangle((2, 35), 96, 24, fc='#F7FBFF', ec='black')); ax.text(4, 56, 'USER MODE (mode bit = 1)', fontsize=10, fontweight='bold')
ax.add_patch(plt.Rectangle((2, 4), 96, 26, fc='#FFF7F0', ec='black')); ax.text(4, 27, 'KERNEL MODE (mode bit = 0)', fontsize=10, fontweight='bold')
box(ax, 5, 40, 20, 8, '1. Application\nn = read(fd, buf, 100)')
box(ax, 32, 40, 22, 8, '2. libc wrapper read()\nEAX/RAX = 0, RDI=fd,\nRSI=buf, RDX=100')
box(ax, 62, 40, 18, 8, '3. SYSCALL (trap)\nsave PC/flags, mode->kernel')
box(ax, 62, 10, 20, 9, '4. Entry code: look up\nsys_call_table[0]')
box(ax, 34, 10, 22, 9, '5. vfs_read -> file system\n-> page cache / driver / DMA')
box(ax, 6, 10, 22, 9, '6. copy_to_user(buf),\nreturn value in RAX')
arrow(ax, 25, 44, 32, 44); arrow(ax, 54, 44, 62, 44); arrow(ax, 71, 40, 72, 19.5); arrow(ax, 62, 14.5, 56, 14.5); arrow(ax, 34, 14.5, 28, 14.5)
arrow(ax, 17, 19.5, 17, 40, '7. SYSRET: restore state,\nmode->user, resume', '->', dy=0, fs=9)
ax.text(50, 62, 'Figure 5.1 Step-by-step path of the read() system call', ha='center', fontsize=11, fontweight='bold')
save(fig, 'fig_syscall.png')

# 3 structures
fig, ax = canvas(10, 5.6)
def stack(x, title, rows, colors):
    ax.text(x + 10, 52, title, ha='center', fontsize=10, fontweight='bold')
    for i, (r, c) in enumerate(zip(rows, colors)): box(ax, x, 40 - i * 9, 20, 7, r, c, fs=8)
stack(1, 'Monolithic', ['Applications', 'ONE kernel: scheduler,\nFS, drivers, net, VM', 'Hardware'], ['#DDF0DD', '#F8D7D7', '#DDD'])
stack(26, 'Layered', ['Applications', 'Layer 3: user interface', 'Layer 2: I/O mgmt', 'Layer 1: memory/CPU', 'Hardware'], ['#DDF0DD', '#FFF2CC', '#FFF2CC', '#FFF2CC', '#DDD'])
stack(51, 'Microkernel', ['Apps | FS server | Driver\nserver | Net server', 'Microkernel: IPC,\nscheduling, addr spaces', 'Hardware'], ['#DDF0DD', '#CFE2F3', '#DDD'])
stack(76, 'Modular / Hybrid', ['Applications', 'Core kernel + loadable\nmodules (drivers, FS)', 'Hardware'], ['#DDF0DD', '#E0D8F0', '#DDD'])
ax.text(50, 58, 'Figure 3.1 Operating-system structures', ha='center', fontsize=11, fontweight='bold')
save(fig, 'fig_structures.png')

# 4 buffer size
bs = [1, 16, 512, 4096, 65536, 1048576]; calls = [4194304, 262144, 8192, 1024, 64, 4]; t = [2.3343, .1392, .0121, .0073, .0062, .0030]
fig, (a, b) = plt.subplots(1, 2, figsize=(10, 4))
a.loglog(bs, calls, 'o-'); a.set_xlabel('buffer size b (bytes)'); a.set_ylabel('read() calls for 4 MB'); a.grid(True, which='both', alpha=.3); a.set_title('System calls = n / b')
b.loglog(bs, t, 's-r'); b.set_xlabel('buffer size b (bytes)'); b.set_ylabel('elapsed time (s)'); b.grid(True, which='both', alpha=.3); b.set_title('Elapsed time')
save(fig, 'fig_bufsize.png')

# 5 creation times
fig, ax = plt.subplots(figsize=(8, 4))
lab = ['thread\ncreate+join', 'fork+exit\n(small parent)', 'fork+exit\n(200 MB parent)', 'posix_spawn\n(true)', 'fork+exec\n(true)']
v = [10.3, 59.5, 2120.9, 681.4, 880.5]
ax.bar(lab, v, color=['#6aa84f', '#3d85c6', '#cc4125', '#e69138', '#8e7cc3']); ax.set_yscale('log'); ax.set_ylabel('microseconds per creation (log)')
for i, x in enumerate(v): ax.text(i, x * 1.1, '%.1f' % x, ha='center', fontsize=9)
ax.set_title('Time to create one execution unit (1000 repetitions)')
save(fig, 'fig_create.png')

# 6 memory hierarchy
sz = ['4KB', '16KB', '64KB', '256KB', '1MB', '4MB', '16MB', '64MB', '256MB']; lat = [1.75, 1.81, 3.98, 5.45, 7.69, 29.72, 71.43, 130.74, 145.91]
fig, ax = plt.subplots(figsize=(8, 4)); ax.plot(sz, lat, 'o-'); ax.set_ylabel('ns per dependent access'); ax.set_xlabel('working-set size'); ax.grid(alpha=.3)
ax.set_title('Measured access time as the working set outgrows each cache level')
save(fig, 'fig_hier.png')

# 7 polling
fig, ax = plt.subplots(figsize=(8, 4))
m = ['poll 50 ms', 'poll 10 ms', 'poll 1 ms', 'busy poll', 'interrupt']; d = [47.09, 5.73, 0.937, 0.0, 0.146]; c = [0.254, 0.81, 3.651, 208.9, 0.063]
x = np.arange(5); ax.bar(x - .2, d, .4, label='detection delay (ms)'); ax.bar(x + .2, c, .4, label='CPU time used (ms)'); ax.set_yscale('symlog', linthresh=0.1); ax.set_xticks(x); ax.set_xticklabels(m); ax.legend(); ax.set_title('Polling versus interrupt-driven detection (measured)')
save(fig, 'fig_poll.png')

# 8 fork/exec
fig, ax = canvas(10, 5)
box(ax, 3, 36, 20, 8, 'shell (parent)\nreads "expr 6 * 7"')
box(ax, 38, 36, 20, 8, 'fork(): child is a COPY\n(same code, COW pages)', '#FFF2CC')
box(ax, 70, 36, 26, 8, 'child: execlp("expr")\nimage replaced, same PID', '#F8D7D7')
box(ax, 70, 16, 26, 8, 'expr prints 42, exit(0)', '#F8D7D7')
box(ax, 38, 16, 20, 8, 'parent: waitpid()\nblocked until child ends', '#DDF0DD')
box(ax, 3, 16, 24, 8, 'WEXITSTATUS = 0\nshell prints "job 1 exit status 0"', '#DDF0DD')
arrow(ax, 23, 40, 38, 40); arrow(ax, 58, 40, 70, 40, 'child'); arrow(ax, 83, 36, 83, 24.5); arrow(ax, 70, 20, 58, 20, 'SIGCHLD / status'); arrow(ax, 38, 20, 27, 20)
arrow(ax, 48, 36, 48, 24.5, 'parent', dy=0)
ax.text(50, 48, 'Figure 7.1 How a shell runs a command: fork - exec - wait - exit', ha='center', fontsize=11, fontweight='bold')
save(fig, 'fig_forkexec.png')

# 9 revised architecture
fig, ax = canvas(10, 6.4)
box(ax, 2, 50, 22, 9, 'Home monitor (embedded RTOS:\nZephyr/FreeRTOS, secure boot)', '#F8D7D7')
box(ax, 28, 50, 20, 9, 'Home gateway / phone\n(Android, TLS 1.3)', '#DDF0DD')
box(ax, 52, 50, 20, 9, 'Hospital edge\n(ward monitors, RTOS)', '#F8D7D7')
box(ax, 76, 50, 22, 9, 'Staff devices\n(tablets, workstations)', '#FFF2CC')
box(ax, 18, 30, 30, 9, 'Internet: MQTT/HTTPS, mutual-TLS, VPN', '#F5F5F5')
box(ax, 54, 30, 30, 9, 'Zero-trust access gateway + IAM', '#F5F5F5')
box(ax, 4, 6, 28, 12, 'Cloud records service\n(Linux VMs/containers, k8s,\nreplicated across 3 zones)', '#CFE2F3')
box(ax, 36, 6, 28, 12, 'On-prem cluster (DB primary,\nimaging) - kept from design', '#CFE2F3')
box(ax, 68, 6, 28, 12, 'Partner hospitals\n(federated identity, shared records)', '#E0D8F0')
for x in (13, 38): arrow(ax, x, 50, 33, 39.5, '', '<->')
for x in (62, 87): arrow(ax, x, 50, 69, 39.5, '', '<->')
arrow(ax, 33, 30, 18, 18.5, '', '<->'); arrow(ax, 69, 30, 50, 18.5, '', '<->'); arrow(ax, 75, 30, 82, 18.5, '', '<->'); arrow(ax, 33, 30, 50, 18.5, '', '<->')
ax.text(50, 63, 'Figure 12.1 Revised design with home monitoring and cloud records', ha='center', fontsize=11, fontweight='bold')
save(fig, 'fig_home.png')
print('ok')
