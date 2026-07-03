#!/bin/sh
# ==============================================================================
# PineRE - Boot Android Runtime
# ==============================================================================
ANDROID_BIN="/usr/bin/anbox-session-manager"
. "$(dirname "$0")/../common/logging.sh"
. "$(dirname "$0")/../common/progress.sh"
pinere_progress_start "Preparing Android Runtime..."
pinere_log_info "Preparing to boot Android runtime environment."
if [ -x "$ANDROID_BIN" ]; then
    pinere_progress_update "50"
    pinere_log_info "Launching Android session."
    sync
    exec "$ANDROID_BIN"
else
    pinere_log_error "Android runtime binary not found."
    exit 2
fi
