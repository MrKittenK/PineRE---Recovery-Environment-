# ARCHITECTURE

Hierarchy

Language
    ↓
UI
    ↓
Dispatcher
    ↓
Work Layer
    ↓
Linux

Boot Mapping

No Button -> EmulationStation
L1        -> Maintenance Mode
R1        -> XFCE Desktop
R2        -> Runtime 3 (placeholder)
SELECT    -> PineRE Boot Manager

Boot Manager Philosophy

The Boot Manager has one responsibility:
Allow the user to choose what to boot.

It has:
- no memory
- no preferences
- no history
- no hidden state

It simply asks.
Then quietly steps aside.
