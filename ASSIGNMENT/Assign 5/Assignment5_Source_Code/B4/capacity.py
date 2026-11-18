# capacity.py - how many containers are needed for 150,000 students in two hours?
import math
students, hours = 150_000, 2
req_per_session = 12          # page loads + API calls per student session (assumption)
peak_factor = 4               # peak rate / average rate (assumption)
target_util = 0.7             # keep each container at 70 percent of its measured capacity
avg_rps = students * req_per_session / (hours * 3600)
peak_rps = avg_rps * peak_factor
print(f"sessions per second (average): {students/(hours*3600):.1f}")
print(f"requests per second: average {avg_rps:.0f}, peak {peak_rps:.0f}")
print()
print(f"{'RPS per container (measured)':30s} {'containers (70% util)':>22s} {'with N+1 and 3 zones':>22s}")
for c in (50, 100, 200, 400, 800):
    n = math.ceil(peak_rps / (c * target_util))
    n_ha = max(3, n + max(1, math.ceil(0.2 * n)))      # 20 percent spare, at least 3
    print(f"{c:30d} {n:22d} {n_ha:22d}")
print()
# VM alternative: 4 containers per 4-vCPU VM
print("VMs of 4 vCPU (4 containers each) for 200 RPS per container:", math.ceil(math.ceil(peak_rps/(200*target_util))/4))
print()
# yearly cost model (all prices are assumptions)
vm_hour = 0.17
own_capex, own_years = 3 * 6000, 5
own_power = 3 * 0.4 * 8760 * 0.10 * 1.5
own_staff = 0.2 * 15000
own = own_capex / own_years + own_power + own_staff
cloud_nodes = 3 * 8760 * vm_hour
cloud_ctrl = 12 * 73
cloud_db = 12 * 250
cloud_lb = 12 * 20
cloud_burst = 10 * 12 * 2 * vm_hour       # 10 extra VMs, 12 h, 2 result days
cloud_store = 12 * 25
cloud = cloud_nodes + cloud_ctrl + cloud_db + cloud_lb + cloud_burst + cloud_store
hyb = own + 12 * 25 + 12 * 20 + 0.3 * cloud_db      # own cluster + object storage backup + small cloud standby
print(f"own servers per year   : {own:8.0f} USD  (capex/5y {own_capex/own_years:.0f}, power {own_power:.0f}, staff {own_staff:.0f})")
print(f"public cloud per year  : {cloud:8.0f} USD  (nodes {cloud_nodes:.0f}, k8s {cloud_ctrl}, db {cloud_db}, lb {cloud_lb}, burst {cloud_burst:.0f}, storage {cloud_store})")
print(f"hybrid per year        : {hyb:8.0f} USD  (own + backup bucket + standby database)")
