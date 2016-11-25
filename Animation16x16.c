#include "Animation16x16.h"
#include <gb/gb.h>

static void advance_frame(Animation16x16* anim);
static void draw_frame(const Animation16x16* anim);

void init_Animation16x16(
  Animation16x16* anim, 
  UINT8 sprite_id, 
  const Animation16x16Info* info
)
{
  anim->info.tile_offset = info->tile_offset;
  anim->info.frame_count = info->frame_count;
  anim->info.vblanks_per_frame = info->vblanks_per_frame;

  anim->sprite_number = sprite_id;
  anim->frame = 0;
  anim->vblanks = 0;

  draw_frame(anim);
}

void step_Animation16x16(Animation16x16* anim)
{
  ++anim->vblanks;
  if (anim->vblanks == anim->info.vblanks_per_frame) {
    advance_frame(anim);
    draw_frame(anim);
    anim->vblanks = 0;
  }

}

/*
* Animation16x16::advance_frame
*
*/
void advance_frame(Animation16x16* anim)
{
  ++anim->frame;
  if (anim->frame == anim->info.frame_count) {
    anim->frame = 0;
  }
}

/*
 * Animation16x16::draw_frame
 *
 * Update the sprite tiles to show the current frame.
 */
void draw_frame(const Animation16x16* anim)
{
  UINT8 tile = (4 * anim->frame) + anim->info.tile_offset;
  set_sprite_tile(anim->sprite_number, tile);
  set_sprite_tile(anim->sprite_number + 1, tile + 2);

}



