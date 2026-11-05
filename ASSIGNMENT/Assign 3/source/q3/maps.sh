sleep 2 & pid=$!; sleep 0.2
echo "segments (mappings) of a running process, pid $pid:"; awk '{print $1, $2, $6}' /proc/$pid/maps | head -14; wait
