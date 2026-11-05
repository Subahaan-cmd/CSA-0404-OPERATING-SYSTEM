import sys; sys.path.insert(0, '..')
from figlib import *
import numpy as np
# state diagram
fig, ax = canvas(10, 5)
for x, y, t, c in [(4, 32, 'New', '#E8EEF7'), (30, 32, 'Ready', '#DDF0DD'), (62, 32, 'Running', '#FFF2CC'), (88, 32, 'Terminated', '#F8D7D7'), (46, 8, 'Waiting\n(I/O or event)', '#E0D8F0')]:
    box(ax, x - 7, y - 4, 16, 8, t, c, fs=10)
arrow(ax, 13, 36, 23, 36, 'admitted'); arrow(ax, 39, 38, 55, 38, 'scheduler dispatch'); arrow(ax, 55, 33, 39, 33, 'interrupt\n(time slice over)', dy=-6.5)
arrow(ax, 73, 36, 81, 36, 'exit'); arrow(ax, 63, 28, 54, 14, 'I/O or event wait', dy=0); arrow(ax, 42, 14, 33, 28, 'I/O or event\ncompletion', dy=0)
ax.text(50, 46, 'Figure 1.1 Process-state diagram', ha='center', fontsize=11, fontweight='bold')
save(fig, 'fig_states.png')
# PCB
fig, ax = canvas(7, 5.2)
rows = ['Process state (new/ready/running/waiting)', 'Process ID (PID), parent PID', 'Program counter', 'CPU registers, stack pointer', 'CPU-scheduling info (priority, queue, time used)', 'Memory-management info (page-table base, limits)', 'Accounting info (CPU time, limits)', 'I/O status (open files, devices, signals)']
for i, r in enumerate(rows): box(ax, 8, 44 - i * 5.6, 54, 4.6, r, '#E8EEF7' if i % 2 == 0 else '#F5F8FC', fs=8)
ax.text(35, 49, 'Process control block (PCB)', ha='center', fontsize=11, fontweight='bold')
save(fig, 'fig_pcb.png')
# gantt
import matplotlib.pyplot as plt
def gantt(name, segs, title):
    fig, ax = plt.subplots(figsize=(9, 1.5)); cols = {'P1': '#9fc5e8', 'P2': '#b6d7a8', 'P3': '#ffe599', 'P4': '#ea9999'}
    for n, a, b in segs:
        ax.barh(0, b - a, left=a, color=cols[n], edgecolor='black'); ax.text((a + b) / 2, 0, n, ha='center', va='center', fontsize=8)
    ax.set_yticks([]); ax.set_xticks(sorted({s[1] for s in segs} | {segs[-1][2]})); ax.tick_params(labelsize=7); ax.set_title(title, fontsize=10); return fig
G = {'FCFS': [('P1', 0, 8), ('P2', 8, 12), ('P3', 12, 21), ('P4', 21, 26)],
     'SJF': [('P1', 0, 8), ('P2', 8, 12), ('P4', 12, 17), ('P3', 17, 26)],
     'SRTF': [('P1', 0, 1), ('P2', 1, 5), ('P4', 5, 10), ('P1', 10, 17), ('P3', 17, 26)],
     'Priority': [('P1', 0, 8), ('P3', 8, 17), ('P4', 17, 22), ('P2', 22, 26)],
     'RR q=3': [('P1', 0, 3), ('P2', 3, 6), ('P3', 6, 9), ('P4', 9, 12), ('P1', 12, 15), ('P2', 15, 16), ('P3', 16, 19), ('P4', 19, 21), ('P1', 21, 23), ('P3', 23, 26)]}
fig, axs = plt.subplots(5, 1, figsize=(9, 7.5)); cols = {'P1': '#9fc5e8', 'P2': '#b6d7a8', 'P3': '#ffe599', 'P4': '#ea9999'}
for ax, (k, segs) in zip(axs, G.items()):
    for n, a, b in segs: ax.barh(0, b - a, left=a, color=cols[n], edgecolor='black'); ax.text((a + b) / 2, 0, n, ha='center', va='center', fontsize=8)
    ax.set_yticks([]); ax.set_xlim(0, 26); ax.set_xticks(sorted({s[1] for s in segs} | {26})); ax.tick_params(labelsize=7); ax.set_ylabel(k, rotation=0, ha='right', va='center', fontsize=9)
fig.suptitle('Figure 2.1 Gantt charts for the four jobs', fontsize=11); fig.tight_layout(); fig.savefig('fig_gantt.png', dpi=150); plt.close(fig)
# quantum study
q = [1, 3, 5, 20]; wt = [12.5, 13.5, 11.0, 8.75]; cs = [23, 9, 5, 3]
fig, (a, b) = plt.subplots(1, 2, figsize=(9, 3.4)); a.plot(q, wt, 'o-'); a.set_xlabel('time quantum'); a.set_ylabel('average waiting time'); a.grid(alpha=.3)
b.plot(q, cs, 's-r'); b.set_xlabel('time quantum'); b.set_ylabel('context switches'); b.grid(alpha=.3); fig.suptitle('Figure 2.2 Effect of the time quantum on Round Robin'); save(fig, 'fig_quantum.png')
# speedup
fig, ax = plt.subplots(figsize=(7, 3.6)); lab = ['seq', 'pool 4', 'pool 8', 'pool 16', '64 threads']
ax.bar(np.arange(5) - .2, [1, 1.02, 1.08, 1.06, 1.05], .4, label='CPU-only frames'); ax.bar(np.arange(5) + .2, [1, 2.0, 1.98, 1.92, 2.0], .4, label='frames with 5 ms I/O wait')
ax.set_xticks(range(5)); ax.set_xticklabels(lab); ax.set_ylabel('speed-up'); ax.legend(); ax.set_title('Figure 3.1 Measured speed-up on a one-core machine'); save(fig, 'fig_speedup.png')
# ipc
fig, ax = plt.subplots(figsize=(7, 3.6)); x = np.arange(3); w = .2
for i, (l, v) in enumerate([('pipe', [0.3, 1.5, 13.8]), ('socketpair', [0.6, 1.8, 11.1]), ('message queue', [0.3, 2.3, 18.2]), ('shared memory', [0.4, 1.0, 5.2])]): ax.bar(x + (i - 1.5) * w, v, w, label=l)
ax.set_xticks(x); ax.set_xticklabels(['1 MB', '10 MB', '100 MB']); ax.set_ylabel('transfer time (ms)'); ax.legend(); ax.set_title('Figure 4.1 Transfer time by IPC method'); save(fig, 'fig_ipc.png')
# locks
fig, ax = plt.subplots(figsize=(7, 3.6)); th = [1, 4, 16]
for l, v in [('mutex', [19.1, 19.2, 20.3]), ('spin lock', [8.9, 20.6, 46.1]), ('atomic add', [8.6, 9.1, 8.7])]: ax.plot(th, v, 'o-', label=l)
ax.set_xlabel('threads'); ax.set_ylabel('ns per update'); ax.set_xscale('log', base=2); ax.legend(); ax.grid(alpha=.3); ax.set_title('Figure 5.1 Cost of a protected counter update'); save(fig, 'fig_locks.png')
# RAG
fig, ax = canvas(10, 4.8)
for x, y, t in [(10, 30, 'J1'), (45, 30, 'J2'), (80, 30, 'J3')]: ax.add_patch(plt.Circle((x + 4, y + 4), 5, fc='#FFF2CC', ec='black')); ax.text(x + 4, y + 4, t, ha='center', va='center', fontweight='bold')
for x, y, t in [(26, 6, 'GPU'), (62, 6, 'LIC'), (46, 40, 'STO')]: box(ax, x, y, 12, 6, t, '#CFE2F3')
arrow(ax, 26, 10, 18, 31, 'assigned', dy=-1); arrow(ax, 16, 36, 46, 41, '', '<-', ls='--')
arrow(ax, 14, 31, 30, 13, '', '->'); 
arrow(ax, 62, 10, 52, 31, 'assigned', dy=-1); arrow(ax, 56, 34, 62, 13, '', '<-')
arrow(ax, 48, 40, 83, 36, 'assigned', dy=0.5); arrow(ax, 84, 31, 38, 12, 'request GPU', dy=0)
ax.text(50, 46, 'Figure 6.1 Resource-allocation graph with a deadlock cycle J1 -> LIC -> J2 -> STO -> J3 -> GPU -> J1', ha='center', fontsize=9, fontweight='bold')
save(fig, 'fig_rag.png')
# banker time
n = [10, 100, 1000, 4000]; t = [0.1, 6.9, 504.1, 7834.8]
fig, ax = plt.subplots(figsize=(6, 3.6)); ax.loglog(n, t, 'o-', label='measured worst case'); ax.loglog(n, [0.07 * x * x / 100 * 100 / 100 * 1 for x in n], '--', label='c * n^2'); ax.set_xlabel('processes n'); ax.set_ylabel('time (us)'); ax.legend(); ax.grid(alpha=.3, which='both'); ax.set_title('Figure 6.2 Safety algorithm: time against n'); save(fig, 'fig_banker.png')
print('ok')
