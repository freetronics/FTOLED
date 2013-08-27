/*
 * A header file designed to provide PROGMEM compatible (no-op) macros for
 * Arduino Due or other non-AVR based boards.
 *
 * This allows code written to use PROGMEM on AVRs to transparently run on
 * other platforms
 */
#ifndef _PROGMEM_COMPAT_H
#define _PROGMEM_COMPAT_H

#ifdef __AVR__
#include <avr/pgmspace.h>
#else // ARM

// Stub out some other common PROGMEM functions
#define PROGMEM
#define memcpy_P memcpy

static inline uint8_t pgm_read_byte(const void *addr) { return *((uint8_t *)addr); }

#define F(X) X


#endif
#endif
