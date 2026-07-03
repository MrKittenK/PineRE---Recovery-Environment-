# PineRE Development Log --- Kernel Build Recovery & First Boot Milestone

**Date:** 2026-06-21

## Objective

Continue from the successful build environment setup and achieve the
first successful PineRE kernel build and deployment onto actual R36
Ultra hardware.

## Phase 10 --- Kernel Build Failure Investigation

### Initial Symptoms

Kernel compilation repeatedly stopped before producing:

-   vmlinux
-   Image

Observed issues included:

-   Build termination without obvious errors.
-   Error 2 failures inside vendor wireless drivers.
-   Missing generated artifacts despite lengthy compilation runs.

## Phase 11 --- Source Tree Integrity Investigation

### Discovery

The kernel source tree had originally been cloned onto NTFS storage
before being copied onto ext4.

Investigation revealed Git symlinks had not been preserved correctly.

### Resolution

Repository integrity was restored after correcting the symlink handling
issue.

Verification confirmed symbolic links were present and functioning
correctly.

### Conclusion

Kernel source integrity was successfully restored.

------------------------------------------------------------------------

## Phase 12 --- GCC Wrapper Reapplication

The warning relaxation modification in `scripts/gcc-wrapper.py` was
reapplied to prevent GCC 12 warnings from terminating the build.

### Conclusion

Experimental compilation could proceed again.

------------------------------------------------------------------------

## Phase 13 --- Vendor Driver Audit Begins

### Qualcomm CLD WLAN Failure

Build failure encountered in:

`drivers/staging/qcacld-2.0`

Configuration discovered:

`CONFIG_QCA_CLD_WLAN=y`

### Resolution

Disabled `CONFIG_QCA_CLD_WLAN` via menuconfig.

### Result

Compilation progressed significantly further.

------------------------------------------------------------------------

## Phase 14 --- Realtek RTL8723AU Failure

Build failure encountered in:

`drivers/staging/rtl8723au`

### Resolution

Disabled:

`CONFIG_R8723AU`

and associated options.

### Result

Build progressed further.

------------------------------------------------------------------------

## Phase 15 --- Realtek RTL8812AU Failure

Subsequent failure encountered in:

`drivers/net/wireless/rtl8812au`

### Resolution

Disabled RTL8812AU support through kernel configuration.

### Result

Compilation resumed successfully.

------------------------------------------------------------------------

## Phase 16 --- First Successful Kernel Build

### Artifacts Produced

-   ARM64 Image generated successfully.
-   Kernel image size approximately 14 MB.
-   541 kernel modules generated.
-   136 DTBs generated.

### Significant Discovery

Compilation successfully produced RK3326 handheld DTBs including:

-   rk3326-odroidgo2-linux
-   rk3326-odroidgo3-linux
-   rk3326-rgb10max-linux

### Conclusion

PineRE achieved its first successful native ARM64 kernel build.

------------------------------------------------------------------------

## Phase 17 --- Device Tree Analysis

### Existing Hardware DTB

Current R36 Ultra boot configuration uses:

`rk3326-r36ultra-linux.dtb`

which had previously been generated from the shipping R36 Ultra DTB.

### Conclusion

PineRE possesses both:

-   Generic RK3326 handheld DTS references.
-   Actual R36 Ultra hardware DTS references.

This provides a strong foundation for future PineRE-specific DTS
development.

------------------------------------------------------------------------

## Phase 18 --- Boot Partition Investigation

### Existing Boot Layout

-   Image
-   uInitrd
-   rk3326-r36ultra-linux.dtb
-   boot.ini

### Important Finding

The system uses separate Image and DTB files.

The kernel image can therefore be replaced independently from the DTB.

------------------------------------------------------------------------

## Phase 19 --- First PineRE Kernel Deployment

### Procedure

-   Existing Image backed up.
-   Newly compiled kernel copied to the BOOT partition.
-   Existing R36 Ultra DTB retained.

### Result

System booted successfully.

The only obstacle encountered was a previously modified `/etc/inittab`
used during testing.

After restoring the original configuration, the system reached a login
prompt successfully.

------------------------------------------------------------------------

# PineRE Milestone 0

## First Successful Native Kernel Boot

**Date:** 2026-06-21

### Achieved

-   Custom kernel compiled.
-   ARM64 Image produced.
-   DTBs produced.
-   Kernel deployed to physical hardware.
-   Existing R36 Ultra DTB retained.
-   Root filesystem mounted.
-   Userspace launched.
-   Login prompt reached.

### Proven

-   Toolchain functional.
-   Source tree functional.
-   Bootloader functional.
-   DTB functional.
-   Kernel functional.
-   R36 Ultra capable of booting PineRE-built kernels.

### Strategic Impact

PineRE has transitioned from:

**Concept**

to

**Bootable Platform**

for the first time.

------------------------------------------------------------------------

## Frozen PineRE Decisions

### Boot Selection

-   No button = EmulationStation
-   L1 = Maintenance Shell
-   R1 = XFCE Desktop
-   R2 = Waydroid
-   SELECT = PineRE Boot Manager

### Partition Layout

-   Partition 1: BOOT (FAT32)
-   Partition 2: EASYROMS (exFAT)
-   Partition 3: ROOTFS (ext4)

### Required Filesystem Support

-   VFAT
-   exFAT
-   ext4
-   tmpfs

------------------------------------------------------------------------

## Next Milestones

1.  Audit generated kernel configuration.
2.  Continue pruning unnecessary vendor drivers.
3.  Investigate USB gadget functionality.
4.  Verify UDC availability.
5.  Test USB Ethernet gadget operation.
6.  Begin defining a dedicated PineRE DTS.
7.  Work toward the first PineRE-branded bootable image.

**End of Log --- PineRE Milestone 0: First Successful Native Kernel
Boot**
