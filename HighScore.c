#include "HighScore.h"
#include "ExternalRam.h"

const UINT8 MAGIC_NUMBER = 12;
UINT16* const HIGH_SCORE = (UINT16*)EXTERNAL_RAM;
UINT8* const CHECK_BYTE = (UINT8*)(EXTERNAL_RAM + 2);

/*
* Read the score in from external RAM (0xA000)
*
* EXT-RAM +0x0 : High score (16 bit)
* EXT-RAM +0x2 : Used to test for junk data: I always set this to 12.
*                The first time you turn on this game, it could be set to anything.
*                Not a very satisfying solution. Perhaps it should be a checksum of the score?
*/
UINT16 load_high_score()
{
  UINT16 score;
  enable_external_ram_range(0x00, 0x03);

  if (*CHECK_BYTE != MAGIC_NUMBER) {
    /* first time init */
    *CHECK_BYTE = MAGIC_NUMBER;
    *HIGH_SCORE = 0;
  }

  score = *HIGH_SCORE;

  disable_external_ram_range(0x00, 0x03);
  return score;
}

void write_high_score(UINT16 new_score)
{
  enable_external_ram_range(0x00, 0x02);
  *HIGH_SCORE = new_score;
  disable_external_ram_range(0x00, 0x02);
}