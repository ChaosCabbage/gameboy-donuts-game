#include "BitTwiddling.h"
#include "HighScore.h"
#include "GameScene.h"
#include "res/numbers.h"

#include <gb/gb.h>

inline void clear_nintendo_bkg_map()
{
  wipe_memory((UINT8*)0x9904, (UINT8*)0x9930, 0);
}

inline void sound_off()
{
  UINT8* const sound_control = (UINT8*)0xFF26;
  *sound_control = 0;
}

void main()
{
  sound_off();
  display_off();
  disable_interrupts();
  clear_nintendo_bkg_map();

  while (1) {
    game_scene();
  }
}