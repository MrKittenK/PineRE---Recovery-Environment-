# PineRE Architecture & Progress Bible
Version: Night Owl Edition
Status: Living Document

---

# Executive Summary

PineRE is a custom operating system project for RK3326 handhelds, focused on:

- Reliability
- Recoverability
- Simplicity
- Developer friendliness
- Low resource consumption
- Long-term maintainability

PineRE aims to be:

> A gaming handheld first.
>
> A development platform second.

---

# Core PineRE Philosophy

## Frozen Philosophy

Explore freely.

Recover easily.

Status: FROZEN ✅

---

## Resource Philosophy

Gaming Mode:

- Give resources to games.
- Give resources to emulators.
- Give resources to Waydroid.
- Minimise background CPU usage.
- Minimise RAM usage.
- Minimise battery impact.

Management services must justify their existence.

Status: FROZEN ✅

---

# Frozen Boot Architecture

No button:
- EmulationStation

L1:
- Maintenance Shell

R1:
- XFCE Desktop

R2:
- Waydroid

SELECT:
- PineRE Boot Manager

Status: FROZEN ✅

---

# Frozen Partition Layout

Partition 1:
BOOT (FAT32)

Partition 2:
EASYROMS (exFAT)

Partition 3:
ROOTFS (ext4)

Status: FROZEN ✅

---

# Recovery Philosophy

Recovery Mode is a first-class feature.

Recovery Mode exists to:

- Recover systems
- Diagnose issues
- Develop features
- Repair configuration mistakes

Recovery Mode enables all development and recovery services regardless of runtime settings.

Status: FROZEN ✅

---

# Runtime Philosophy

Gaming Mode:

Run only what is necessary.

Maintenance Mode:

Run everything useful.

Status: FROZEN ✅

---

# Service Model

Gaming Mode:

- Minimal services
- Minimal resource consumption

Maintenance Mode:

- USB SSH
- WiFi SSH
- Dashboard
- SFTP
- Remote Logs
- Remote Terminal
- Developer Tools

Recovery Mode overrides runtime settings.

Status: FROZEN ✅

---

# Developer Mode

Developer Mode is not hidden.

Developer Mode is not locked.

Developer Mode is not discouraged.

Developer Mode is clearly marked.

Status: FROZEN ✅

Developer Warning:

⚠ Here Be Dragons

Developer Mode contains advanced features intended for developers,
tinkerers and curious explorers.

If you break something,
Recovery Mode is your friend.

Status: FROZEN ✅

---

# Configuration Architecture

Single source of truth.

Proposed location:

/etc/pinere.conf

Human readable.
Easy to backup.
Easy to recover.

Example:

WIFI_SSH=0
SFTP=0
DEV_MODE=0

Status: FROZEN ✅

---

# Logging Architecture

Location:

/var/log/pinere/

Examples:

boot.log
network.log
services.log
dashboard.log

Status: FROZEN ✅

---

# Dashboard Architecture

Dashboard is module based.

Core Dashboard:

- Status
- Resources
- Services

Modules:

- ROM Manager
- Save Manager
- Screenshot Gallery
- Package Manager
- Service Manager

Status: FROZEN ✅

---

# Dashboard Data Sources

Primary sources:

/proc
/sys

No metrics database.

No telemetry daemon.

No permanent monitoring service.

Information is retrieved on demand.

Status: FROZEN ✅

---

# Networking Vision

Long-Term Goal:

Connect USB cable.

Open terminal.

ssh root@pinere.local

Done.

No special host software.
No PineRE-specific tooling required.

Status: TARGET ARCHITECTURE 🚧

---

# Networking Stack

Layer 1:
USB ECM

Layer 2:
DHCP

Layer 3:
mDNS (pinere.local)

Layer 4:
SSH / SFTP

Layer 5:
Dashboard

Status: DESIGN APPROVED ✅

---

# Completed Work

## Phase 31

USB SSH Infrastructure

Status: COMPLETE ✅

Achievements:

- USB networking operational.
- USB ECM gadget working.
- SSH access proven.
- Remote shell obtained.
- Development possible without external keyboard.

Major Milestone:

localhost:~#

This marked the transition from image building to remote system administration.

---

## Phase 32

Persistent USB SSH

Status: COMPLETE ✅

Achievements:

- Startup automation created.
- SSH startup automated.
- USB interface configured automatically.
- Persistent development environment established.

---

## Phase 32A

USB Internet Sharing

Status: PROVEN ✅

Achievements:

- Internet routing through host proven.
- DNS configuration proven.
- Gateway configuration proven.

---

## Phase 32B.1

PineRE Self Configuration

Status: COMPLETE ✅

Achievements:

- Automatic USB interface setup.
- Automatic gateway setup.
- Automatic DNS setup.
- Improved diagnostics.
- Reduced manual setup.

---

## Phase 32B.2

MX Linux Host Automation

Status: COMPLETE ✅

Achievements:

- pinere-connect.sh created.
- Host networking automated.
- NAT configuration automated.
- Connectivity verification automated.

Workflow:

Connect PineRE.
Run pinere-connect.sh.
SSH.
Develop.

---

## Phase 32B.3

Zero Host Configuration

Status: DESIGN PHASE 🚧

Objectives:

- DHCP
- mDNS
- pinere.local
- Automatic host configuration
- No PineRE-specific host tooling

---

# Future Dashboard Vision

Version 1:

- CPU
- RAM
- Battery
- Storage
- Services

Version 2:

- Service toggles
- Network status
- Logs

Version 3:

- Remote terminal
- File browser
- Performance tools

Version 4:

- Full remote management environment

---

# Future Service Ideas

Debug & Development Menu:

- USB SSH
- WiFi SSH
- SFTP
- Remote Log Viewer
- Web Dashboard
- Remote Terminal
- Serial Console
- Developer Mode

Status: FUTURE ROADMAP

---

# Future Dashboard Modules

- ROM Manager
- Save Manager
- Screenshot Gallery
- Package Manager UI
- Service Manager
- Kernel Log Viewer
- USB Gadget Control
- Performance Overlay
- Remote Terminal
- Web File Browser

Status: FUTURE ROADMAP

---

# Architectural Principles

Prefer:

- Existing Linux facilities
- Existing kernel interfaces
- Existing standards

Avoid:

- Heavy frameworks
- Unnecessary daemons
- Resource waste

Examples:

Use:
/proc
/sys

Instead of:

Dedicated monitoring services.

Status: FROZEN ✅

---

# PineRE Identity

PineRE is not attempting to be:

- A server
- A telemetry platform
- A cloud appliance

PineRE is attempting to be:

- A handheld gaming platform
- A development platform
- A recoverable platform
- A platform for curious explorers

Status: FROZEN ✅

---

# Memorable Milestones

"We don't have a spare keyboard."

Lead to:

- USB Networking
- USB SSH
- Internet Sharing
- Dashboard Architecture
- Remote Development Platform

A small problem created a large vision.

---

# Current State

Phase 31: COMPLETE ✅
Phase 32: COMPLETE ✅
Phase 32A: PROVEN ✅
Phase 32B.1: COMPLETE ✅
Phase 32B.2: COMPLETE ✅

Current Target:

Phase 32B.3
Zero Host Configuration

---

# Project Motto

Explore freely.

Recover easily.

End of Document.
