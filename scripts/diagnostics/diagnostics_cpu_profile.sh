#!/bin/sh
# PineRE: CPU Performance Profile
LOGFILE="/tmp/pinere_cpu_diag.txt"
{ echo "--- PineRE CPU Profile ---"; cat /proc/loadavg; ps | head -n 10; } > "$LOGFILE"
