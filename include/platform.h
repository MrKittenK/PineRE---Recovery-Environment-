#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Platform Hardware Abstraction Layer (HAL)
 * Responsibility: Abstract hardware-specific initialization and timing.
 * Ownership: platform.c (Implementation), Main (Orchestrator).
 */

/* Initialize display, input, and framebuffer hardware */
bool platform_init(void);

/* Platform-specific delay for timing management */
void platform_delay_ms(uint32_t ms);

#endif
