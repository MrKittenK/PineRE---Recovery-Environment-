#!/bin/sh
# ==============================================================================
# PineRE - Boot ES Desktop
# ==============================================================================
ES_BIN="/usr/bin/emulationstation"
. "$(dirname "$0")/../common/logging.sh"
. "$(dirname "$0")/../common/progress.sh"
pinere_progress_start "Preparing ES Desktop..."
pinere_log_info "Preparing to boot ES desktop environment."
if [ -x "$ES_BIN" ]; then
    pinere_progress_update "50"
    pinere_log_info "Launching ES session."
    sync
    exec "$ES_BIN"
else
    pinere_log_error "ES binary not found."
    exit 2
fi
