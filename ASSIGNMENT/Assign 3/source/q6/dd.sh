echo '$ dd if=/dev/zero of=/tmp/tf bs=1M count=512 oflag=direct   (sequential write)'; dd if=/dev/zero of=/tmp/tf bs=1M count=512 oflag=direct 2>&1 | tail -1
echo '$ dd if=/tmp/tf of=/dev/null bs=1M iflag=direct   (sequential read, no cache)'; dd if=/tmp/tf of=/dev/null bs=1M iflag=direct 2>&1 | tail -1
echo '$ dd if=/tmp/tf of=/dev/null bs=1M   (sequential read, through the page cache)'; dd if=/tmp/tf of=/dev/null bs=1M 2>&1 | tail -1; rm -f /tmp/tf
