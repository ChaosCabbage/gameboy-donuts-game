/*
* Sprite16x16
*
* Tools for working with a 16x16 sprite - just the graphics.
*
* Works only in 8x16 mode!
*/

#ifndef SPRITE16X16_H
#define SPRITE16X16_H

#include <asm/gbz80/types.h>

/*
* Load tiles into VRAM.
* Each 16x16 image involves four 8x8 tiles in the correct order.
* Other frames of animation should also be contiguous.
*
* Params:
*   tile_count: Number of tiles already in memory. This will be updated.
*   num_animation_frames: Number of contiguous 16x16 sprites to load.
*   source: Tile data.
*
* Returns: The tile ID of the first tile loaded.
*
*/
UINT8 load_tiles_Sprite16x16(
  UINT8* tile_count,
  UINT8 num_animation_frames,
  const unsigned char* source
);

/*
* Each 16x16 image needs two 16x8 sprites.
* Graphics tiles should already be loaded.
*
* Params:
*   sprite_count: Number of sprites already created. This will be updated.
*   tile_location: ID of the first tile for this sprite.
*   x, y: 2D screen location to put the sprite.  
*
* Returns: The sprite ID of the first sprite in the pair.
*/
UINT8 create_Sprite16x16(
  UINT8* sprite_count,
  UINT8 tile_location,
  UINT8 x, UINT8 y
);

/*
 * Move this sprite to a new x,y.
 */
void move_Sprite16x16(UINT8 first_sprite_id, UINT8 x, UINT8 y);

#endif