/* Terrible experimental title screen. 
 * Looks awful. Ignore this.
 */


#include <gb/gb.h>

#include "res/krispy_bkg_large.h"
#include "res/krispy_bkg_large_map.h"
#include "res/attack_text.h"
#include "res/attack_text_map.h"

volatile UINT8 g_time_counter;

void timer_handler()
{
  ++g_time_counter;
}

void shift_tile_map(unsigned char data[], UINT8 size, UINT8 offset)
{
  UINT8 i;
  for (i = 0; i < size; ++i) {
    data[i] += offset;
  }
}

void title_scene()
{
  HIDE_SPRITES;
  HIDE_WIN;
  HIDE_BKG;

  shift_tile_map(KrispyBkgLargeMapData, 360, 1);

  g_time_counter = 0;

  set_bkg_data(1, 114, KrispyBkgLargeTileData);
  set_bkg_tiles(0, 5, 20, 18, KrispyBkgLargeMapData);

  shift_tile_map(AttackTextMapData, 80, 115);

  set_bkg_data(115, 54, AttackTextTileData);
  set_bkg_tiles(0, 0, 20, 4, AttackTextMapData);

  SHOW_BKG;

  TAC_REG = 0b111;

  disable_interrupts();
  add_TIM(timer_handler);
  enable_interrupts();

  set_interrupts(TIM_IFLAG | VBL_IFLAG);
  
  while (1) {
    wait_vbl_done();
    if (g_time_counter == 255) {
      g_time_counter = 0;
      break;
    }
  }

  while (1) {
    wait_vbl_done();
    scroll_bkg(0, (INT8)-1);
    if (g_time_counter == 255) {
      g_time_counter = 0;
      break;
    }
  }

  SHOW_WIN;

  while (1) {
    wait_vbl_done();
    if (g_time_counter == 255) {
      g_time_counter = 0;
      break;
    }
  }

  disable_interrupts();
  remove_TIM(timer_handler);
  enable_interrupts();

  HIDE_BKG;
  HIDE_WIN;
}