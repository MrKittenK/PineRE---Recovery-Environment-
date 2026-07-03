# PineRE Recovery Design System (Draft v0.1)

> Internal reference document --- Architecture Freeze

## Purpose

PineRE Recovery is a standalone Buildroot + LVGL recovery environment.
Its purpose is to boot independently of the operating systems it can
recover.

## Design Philosophy

-   Simple, but not dumb.
-   One responsibility per layer.
-   Every feature earns its place.
-   Categories introduce; actions explain.
-   One outcome. One sentence.
-   Read aloud before freezing.
-   Pass the Grandad Test.
-   PineRE speaks with one voice.

## Architecture

    LVGL
      ↓
    Menu Tree
      ↓
    Dispatcher
      ↓
    Shell Script
      ↓
    Linux / BusyBox

## Single Source of Truth

-   menu_assets.txt --- words
-   main.c --- structure
-   pinere_menu.c --- behaviour
-   /usr/libexec/pinere/scripts --- work

## Visual Language

### Symbols

  Symbol   Meaning
  -------- ----------------------------------
  ✓        Enabled / Running / Success
  ✗        Disabled / Stopped / Not Running
  ★        Current default
  ›        Current selection
  →        Reserved for forward navigation

### Typography (Current Direction)

-   IBM Plex Sans --- UI
-   IBM Plex Mono --- logs, terminal, diagnostics

## Voice

Categories: - Choose what to boot. - Recover and repair your
installation. - Manage storage and files. - Manage background
services. - System information and hardware tests. - Developer tools and
advanced options.

Writing rules: - Explain the outcome. - Avoid Linux implementation
details. - Prefer everyday language.

## Current Recovery Structure

-   Boot
-   Recovery
-   Storage
-   Services
-   Diagnostics
-   Developer

## Interaction

Toggle items: - Label shows ✓ or ✗. - A button shows Enable or
Disable. - Details show Running ✓ or ✗.

## Status

Draft for internal reference.
