/*
 * Animation16x16
 *
 * Tools for animating a 16x16 sprite.
 *
 * Works only in 8x16 mode!
 *
 */
#ifndef ANIMATION16X16_H
#define ANIMATION16X16_H

#include <asm/gbz80/types.h>

typedef struct
{
  UINT8 sprite_number;
  UINT8 tile_offset;

  UINT8 frame_count;
  UINT8 frame;

  UINT8 vblanks_per_frame;
  UINT8 vblanks;

} Animation16x16;

/*
 * Animation16x16::init
 *
 * Expects sprites already loaded.
 * All sprites for the entire animation must be contiguous, 
 *  e.g. the first frame is sprites 0 and 1, the next frame is sprites 2 and 3...
 * 
 * Params: 
 *  anim: the thing you want to set up
 *  sprite_id: The first sprite of the two that make this image
 *  tile_id: The position of the first tile
 *  num_animation_frames: The total number of images in the animation.
 *  rate: Number of vblanks in a single animation frame. Smaller numbers are faster.
 */
void init_Animation16x16(Animation16x16* anim, UINT8 sprite_id, UINT8 tile_id, UINT8 num_animation_frames, UINT8 rate);

/*
 * Animation16x16::step
 *
 * Call this on every frame to advance the animation at the correct rate.
 */
void step_Animation16x16(Animation16x16* anim);

#endif 
