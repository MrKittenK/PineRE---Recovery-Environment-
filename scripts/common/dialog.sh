#!/bin/sh
# PineRE: Dialog Toolbox
# Responsibility: Send messages to the UI without blocking script execution.

pinere_dialog_ok() {
    echo "DIALOG_OK|$1" > /tmp/pinere_ui_modal
}

pinere_dialog_yesno() {
    echo "DIALOG_YESNO|$1" > /tmp/pinere_ui_modal
}
