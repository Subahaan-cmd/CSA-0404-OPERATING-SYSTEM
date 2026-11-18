#!/usr/bin/env bash
# Usage: loganalysis.sh access.log   (combined-style log: ip - - [time] "METHOD path" status bytes)
set -euo pipefail
log=${1:?usage: $0 logfile}
echo "Total requests : $(wc -l < "$log")"
echo "Errors (5xx)   : $(awk '$7 ~ /^5/' "$log" | wc -l)"
echo "Top 3 paths:";   awk '{gsub(/"/,"",$6); print $6}' "$log" | sort | uniq -c | sort -rn | head -3
echo "Top 3 clients:"; awk '{print $1}' "$log" | sort | uniq -c | sort -rn | head -3
