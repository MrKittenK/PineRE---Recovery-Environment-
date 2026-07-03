#!/bin/sh
# ==============================================================================
# PineRE - Boot XFCE Desktop
# ==============================================================================
XFCE_BIN="/usr/bin/startxfce4"
. "$(dirname "$0")/../common/logging.sh"
. "$(dirname "$0")/../common/progress.sh"
pinere_progress_start "Preparing XFCE Desktop..."
pinere_log_info "Preparing to boot XFCE desktop environment."
if [ -x "$XFCE_BIN" ]; then
    pinere_progress_update "50"
    pinere_log_info "Launching XFCE session."
    sync
    exec "$XFCE_BIN"
else
    pinere_log_error "XFCE binary not found."
    exit 2
fi
