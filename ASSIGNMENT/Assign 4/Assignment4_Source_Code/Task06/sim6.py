# sim6.py - video-streaming storage model: two alternative designs
# Design A: HDD-only servers.  Design B: HDD + SSD cache of the hottest videos (LRU, whole video).
import numpy as np, collections, random
random.seed(7); np.random.seed(7)
VIDEOS = 5000                    # catalogue size (videos)
AVG_GB = 1.0                     # average video size
REQ = 200000                     # playback starts simulated
ZIPF_S = 1.0                     # popularity skew
ranks = np.arange(1, VIDEOS+1)
p = 1/ranks**ZIPF_S; p /= p.sum()
seq = np.random.choice(VIDEOS, size=REQ, p=p)
def lru_hit(cache_videos):
    c = collections.OrderedDict(); hit = 0
    for v in seq:
        if v in c: c.move_to_end(v); hit += 1
        else:
            c[v] = 1
            if len(c) > cache_videos: c.popitem(last=False)
    return hit/REQ
# device model: time to start playback at a random minute = one random read of a 4 MB chunk
def t_hdd(): return 8.0 + 4.17 + 4/150*1000          # seek + half rotation(7200rpm) + 4 MB @150 MB/s  (ms)
def t_ssd(): return 0.1 + 4/500*1000                   # 0.1 ms access + 4 MB @500 MB/s (ms)
print("HDD chunk read %.1f ms, SSD chunk read %.1f ms" % (t_hdd(), t_ssd()))
print("%-18s %-10s %-12s %-14s" % ("SSD cache size","hit ratio","avg start ms","HDD load left"))
for pct in (0, 2, 5, 10, 20):
    k = int(VIDEOS*pct/100)
    h = lru_hit(k) if k else 0.0
    avg = h*t_ssd() + (1-h)*t_hdd()
    print("%-18s %-10.3f %-12.1f %-14.3f" % (f"{pct}% ({k} videos, {k*AVG_GB:.0f} GB)", h, avg, 1-h))
