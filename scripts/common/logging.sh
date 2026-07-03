#!/bin/sh
# PineRE: Logging Toolbox
# Responsibility: Format and timestamp system logs.

PINERE_LOG="/tmp/pinere.log"

pinere_log_info() {
    echo "[$(date +'%Y-%m-%dT%H:%M:%S')] [INFO] $1" >> "$PINERE_LOG"
}

pinere_log_error() {
    echo "[$(date +'%Y-%m-%dT%H:%M:%S')] [ERROR] $1" >> "$PINERE_LOG"
}
