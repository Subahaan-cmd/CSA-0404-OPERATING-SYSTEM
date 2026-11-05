"""Step-by-step trace used for the hand verification (3 frames, textbook string)."""
ref = [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1]
def trace(policy):
    frames = []; faults = 0; rows = []
    for i, p in enumerate(ref):
        hit = p in frames
        if hit:
            if policy == 'LRU': frames.remove(p); frames.append(p)
        else:
            faults += 1
            if len(frames) == 3:
                if policy == 'OPT':
                    def nu(x):
                        try: return ref.index(x, i + 1)
                        except ValueError: return 10**9
                    frames.remove(max(frames, key=nu))
                else: frames.pop(0)
            frames.append(p)
        rows.append((p, sorted(frames) if policy == 'OPT' else list(frames), 'hit' if hit else 'FAULT'))
    return rows, faults
for pol in ('FIFO', 'LRU', 'OPT'):
    rows, f = trace(pol)
    print("%s, 3 frames: %d faults" % (pol, f))
    print("  ref : " + " ".join("%2d" % r[0] for r in rows))
    print("  flt : " + " ".join(" X" if r[2] == 'FAULT' else " ." for r in rows))
    print()
