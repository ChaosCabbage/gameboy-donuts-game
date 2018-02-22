/*
* Functions for working with external ram.
* 
* External ram is the range A000-BFFF, but only if your cartridge type supports it.
*
* Before reading or writing to any external ram, you must enable it.
* For example, if you want the first 12 bytes, you need to do
*     enable_external_ram_range(0, 12);
*
* It is recommended to disable external RAM after accessing it, in order to protect its contents from damage during power down of the gameboy.
* 
*/
#ifndef EXTERNALRAM_H
#define EXTERNALRAM_H

#include <asm/gbz80/types.h>

#define EXTERNAL_RAM 0xA000

/*
 * Allow read and write access to a range of external ram from [start, end).
 * Addresses are given relative to the start of external ram.
 */
void enable_external_ram_range(UINT8 relative_start_address, UINT8 relative_address_end);

/*
 * Disallow read and write access to a range of external ram from [start, end).
 * Addresses are given relative to the start of external ram.
 */
void disable_external_ram_range(UINT8 relative_start_address, UINT8 relative_address_end);

#endif
