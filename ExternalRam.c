#include "ExternalRam.h"
#include "BitTwiddling.h"

const UINT8 RAM_OFF = 0x00;
const UINT8 RAM_ON = 0x0A;

void enable_external_ram_range(UINT8 start, UINT8 end)
{
  wipe_memory((UINT8*)start, (UINT8*)end, RAM_ON);
}

void disable_external_ram_range(UINT8 start, UINT8 end)
{
  wipe_memory((UINT8*)start, (UINT8*)end, RAM_OFF);
}
