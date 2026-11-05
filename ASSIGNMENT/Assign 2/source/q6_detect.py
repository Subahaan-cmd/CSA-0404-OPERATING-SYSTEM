"""Q6 alternative: deadlock DETECTION (wait-for graph cycle search) on a small cluster, and cost of one check."""
def find_cycle(wait_for):
    state = {}
    def dfs(u, path):
        state[u] = 1; path.append(u)
        for v in wait_for.get(u, []):
            if state.get(v) == 1: return path[path.index(v):] + [v]
            if state.get(v) is None:
                c = dfs(v, path)
                if c: return c
        state[u] = 2; path.pop(); return None
    for u in list(wait_for):
        if state.get(u) is None:
            c = dfs(u, [])
            if c: return c
    return None
# J1 holds GPU1 wants LIC1; J2 holds LIC1 wants STO1; J3 holds STO1 wants GPU1; J4 holds GPU2 and is waiting for nothing
wf = {"J1": ["J2"], "J2": ["J3"], "J3": ["J1"], "J4": []}
print("wait-for graph:", wf)
print("cycle found   :", find_cycle(wf))
cycle = find_cycle(wf)
lost = {"J1": 35, "J2": 12, "J3": 50}
victim = min(set(cycle[:-1]), key=lambda j: lost[j])
print("victim (least work lost, minutes):", victim, lost[victim])
wf[victim] = []
wf = {k: [x for x in v if x != victim] for k, v in wf.items()}
print("after abort, cycle:", find_cycle(wf))
