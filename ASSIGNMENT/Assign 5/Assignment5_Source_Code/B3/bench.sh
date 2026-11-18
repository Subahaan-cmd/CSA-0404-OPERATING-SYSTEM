#!/usr/bin/env bash
# bench.sh <url> <label> : requests per second with ab (or hey/wrk) and memory of the server
URL=${1:-http://localhost:8000/health}; LABEL=${2:-run}
for i in 1 2 3 4 5; do
    ab -k -n 20000 -c 50 "$URL" 2>/dev/null | awk '/Requests per second/ {print $4}'
done | sort -n | awk '{a[NR]=$1} END {print "'"$LABEL"' median requests/s:", a[int((NR+1)/2)]}'

# start-up time: from the start command to the first successful response
start=$(date +%s.%N)
until curl -sf "$URL" >/dev/null; do sleep 0.05; done
echo "start-up (s): $(echo "$(date +%s.%N) - $start" | bc)"
