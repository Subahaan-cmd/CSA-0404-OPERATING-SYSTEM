#!/usr/bin/env bash
# Warn when any file system is more than THRESHOLD percent full.
set -euo pipefail
THRESHOLD=${1:-80}
df -P -x tmpfs -x devtmpfs | awk -v t="$THRESHOLD" 'NR > 1 && $1 ~ /^\/dev\// {
    use = $5; sub(/%/, "", use)
    if (use + 0 >= t) printf "ALERT: %s is %s%% full (mounted on %s)\n", $1, use, $6
    else              printf "ok:    %s is %s%% full (mounted on %s)\n", $1, use, $6 }'
