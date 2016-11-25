#include "Sprite16x16.h"
#include <gb/gb.h>


UINT8 load_tiles_Sprite16x16(
  UINT8* tile_count,
  UINT8 num_animation_frames,
  unsigned char* source
)
{
  UINT8 next_tile = *tile_count;
  UINT8 number_of_tiles = num_animation_frames * 4;

  /* load in the sprites to VRAM */
  set_sprite_data(next_tile, number_of_tiles, source);

  *tile_count += number_of_tiles;
  return next_tile;
}


UINT8 create_Sprite16x16(
  UINT8* sprite_count,
  UINT8 tile_location,
  UINT8 x, UINT8 y
)
{
  UINT8 next_sprite = *sprite_count;

  /* point the two sprites to their data slots */
  set_sprite_tile(next_sprite, tile_location);
  set_sprite_tile(next_sprite + 1, tile_location + 2);

  /* draw them in plain sight */
  move_sprite(next_sprite, x, y);
  move_sprite(next_sprite + 1, x + 8, y);

  *sprite_count += 2;

  return next_sprite;
}


void move_Sprite16x16(UINT8 first_sprite_id, UINT8 x, UINT8 y)
{
  move_sprite(first_sprite_id, x, y);
  move_sprite(first_sprite_id + 1, x + 8, y);
}
