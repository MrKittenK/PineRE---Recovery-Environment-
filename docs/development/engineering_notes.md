# PineRE Engineering Notes (The "Boring Engine")

This document captures the architectural decisions behind PineRE.

## The Boring Engine Philosophy
1. Isolation: We treat the C code, the assets, and the shell scripts as distinct layers. This allows us to modify one without breaking the others.
2. Determinism: The C Dispatcher only cares about exit codes. It does not need to know *what* the script does, only that it succeeded (0) or failed (non-zero).
3. The Toolbox: The `scripts/common/` directory is not a dumping ground. It is a strictly controlled API (logging, dialogs, progress).
4. The Grandad Test: Every script, every label, and every menu item must be understandable to a non-technical user without external documentation.

## Why this works
By freezing the Architecture, the Language, and the Work Layer separately, we have created a development environment that is resilient to feature creep. We can iterate on the UI or the scripts independently.
