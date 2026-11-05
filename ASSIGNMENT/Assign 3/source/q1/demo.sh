#!/usr/bin/env bash
# Q1 experiments: static v dynamic linking, replacing the library, missing library, start-up time, shared pages
cd "$(dirname "$0")"
run() { echo "\$ $*"; eval "$*" 2>&1; }
run "gcc -std=gnu11 -O2 -static -o app_static app.c libenc.c"
run "gcc -std=gnu11 -O2 -o app_dynamic app.c -L. -lenc -Wl,-rpath,'\$ORIGIN'"
run "ls -l app_static app_dynamic libenc.so | awk '{print \$5, \$9}'"
run "size app_static app_dynamic"
run "ldd app_dynamic"
run "ldd app_static"
echo; echo "--- replace the shared library WITHOUT recompiling (security fix = version 2)"
run "./app_dynamic | head -1"; run "./app_static | head -1"
cp libenc.so /tmp/libenc_v1.so; cp libenc_v2.so libenc.so
run "./app_dynamic | head -1   # dynamic program now uses the fixed library"
run "./app_static | head -1    # static program still contains the old code"
cp /tmp/libenc_v1.so libenc.so
echo; echo "--- library missing"
mv libenc.so /tmp/hold.so
run "./app_dynamic; echo exit status=\$?"
run "./app_dl encrypt; echo exit status=\$?"
mv /tmp/hold.so libenc.so
echo; echo "--- start-up time: 500 runs each"
s=$(date +%s.%N); for i in $(seq 1 500); do ./app_dynamic > /dev/null; done; m=$(date +%s.%N); for i in $(seq 1 500); do ./app_static > /dev/null; done; e=$(date +%s.%N)
echo "dynamic: $(echo "($m-$s)*2" | bc) ms per run; static: $(echo "($e-$m)*2" | bc) ms per run"
run "./app_dynamic | tail -1; ./app_static | tail -1"
echo; echo "--- two copies running: which pages are shared (smaps_rollup)"
./app_dynamic --wait > /dev/null & sleep 0.4; ./app_dynamic --wait > /dev/null & sleep 0.4
for p in $(pgrep app_dynamic); do echo "PID $p: $(grep -E '^(Rss|Pss|Shared_Clean|Private_Dirty)' /proc/$p/smaps_rollup | tr -s ' ' | tr '\n' ' ')"; done; wait
