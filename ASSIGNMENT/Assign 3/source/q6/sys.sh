run() { echo "\$ $*"; eval "$*" 2>&1 | head -${LINES_MAX:-14}; }
run "lsblk"
run "fdisk -l"
run "cat /proc/swaps; swapon --show"
run "free -h"
run "df -hT / | head -3"
run "cat /sys/block/vda/queue/scheduler 2>/dev/null || ls /sys/block"
