#include <gb/gb.h>
#include "res/james_sprite.h"
#include "res/donut_sprite.h"

typedef struct 
{
  UINT8 x;
  UINT8 y;
  BOOLEAN alive;

} donut;

typedef struct 
{
  UINT8 sprite_number;
  UINT8 frame_count;
  UINT8 frame;

} animation16x16;


void next_frame(animation16x16* anim)
{
  UINT8 tile;

  ++anim->frame;
  if (anim->frame == anim->frame_count) {
    anim->frame = 0;
  }
  
  tile = (4 * anim->frame);
  set_sprite_tile(anim->sprite_number, tile);
  set_sprite_tile(anim->sprite_number+1, tile+2);
}


/*
  Each 16x16 image creates two 16x8 sprites 
  Each 16x8 sprite takes up 2 slots in the VRAM
*/
void load_16x16_sprites(
  UINT8* sprite_count, 
  UINT8 num_frames,
  const unsigned char* source, 
  UINT8 x, UINT8 y
) 
{
  UINT8 next_sprite = *sprite_count;
  UINT8 next_data_slot = next_sprite * 2;
  UINT8 number_of_slots = num_frames * 4;

  /* load in the sprites to VRAM */
  set_sprite_data(next_data_slot, number_of_slots, source);

  /* poUINT8 the two sprites to their data slots */
  set_sprite_tile(next_sprite, next_data_slot);
  set_sprite_tile(next_sprite + 1, next_data_slot+2);

  /* draw them in plain sight */
  move_sprite(next_sprite, x, y);
  move_sprite(next_sprite + 1, x+ 8, y);

  *sprite_count += 2 * num_frames;
}

void move_16x16(UINT8 first_sprite_id, UINT8 x, UINT8 y)
{
  move_sprite(first_sprite_id, x, y);
  move_sprite(first_sprite_id + 1, x + 8, y);
}


const UINT8 DONUT_SPR_ID = 4;
const UINT8 JAMES_X = 75;

/* global game state */
animation16x16 james_anim;
donut the_donut;
/* -- */

/* Animation time tracker */
UINT8 g_frame_counter = 0;


void eat_the_donut()
{
  the_donut.x = 200;
}


void refresh()
{
  ++g_frame_counter;
  if (g_frame_counter == 30) {
    next_frame(&james_anim);
    g_frame_counter = 0;
  }

  if (g_frame_counter % 2 == 0) {
    the_donut.x -= 1;
    move_16x16(DONUT_SPR_ID, the_donut.x, the_donut.y);
  }

  if (the_donut.x < JAMES_X) {
    eat_the_donut();
  }

}


void main()
{
  UINT8 sprite_count = 0;

  the_donut.x = 150;
  the_donut.y = 80;
  the_donut.alive = TRUE;

  SPRITES_8x16;
  load_16x16_sprites(&sprite_count, 2, jamesSprite, JAMES_X, 75);
  load_16x16_sprites(&sprite_count, 1, donutSprite, the_donut.x, the_donut.y);
  SHOW_SPRITES;

  james_anim.sprite_number = 0;
  james_anim.frame_count = 2;
  james_anim.frame = 0;

  add_VBL(refresh);

}
