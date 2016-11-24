#include <gb/gb.h>
#include "Animation16x16.h"
#include "res/james_sprite.h"
#include "res/donut_sprite.h"
#include "res/robot_sprite.h"


typedef struct 
{
  UINT8 x;
  UINT8 y;
  BOOLEAN alive;

} donut;

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

  /* point the two sprites to their data slots */
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
const UINT8 ROBOT_SPR_ID = 6;
const UINT8 JAMES_X = 75;

/* global game state */
Animation16x16 james_anim;
Animation16x16 robot_anim;
donut the_donut;

UINT8 robot_x;

/* -- */

/* Animation time tracker */
UINT8 g_frame_counter = 0;


void eat_the_donut()
{
  the_donut.x = 200;
}

void redraw()
{
  on_vblank(&james_anim);
  on_vblank(&robot_anim);
  move_16x16(DONUT_SPR_ID, the_donut.x, the_donut.y);
  move_16x16(ROBOT_SPR_ID, robot_x, 20);
}

UINT8 g_counter = 0;
void update() 
{
  ++g_counter;
  if (g_counter == 3) {
    robot_x -= 1;
    if (robot_x == 0) {
      robot_x = MAXWNDPOSX;
    }

    g_counter = 0;
  }

  the_donut.x -= 1;
  if (the_donut.x < JAMES_X) {
    eat_the_donut();
  }
}


void loop()
{
  while (1) {
    redraw();
    update();
    wait_vbl_done();
  }
}


void main()
{
  UINT8 sprite_count = 0;

  robot_x = MAXWNDPOSX;

  the_donut.x = 150;
  the_donut.y = 80;
  the_donut.alive = TRUE;

  SPRITES_8x16;
  load_16x16_sprites(&sprite_count, 2, jamesSprite, JAMES_X, 75);
  load_16x16_sprites(&sprite_count, 1, donutSprite, the_donut.x, the_donut.y);
  load_16x16_sprites(&sprite_count, 8, robotSprite, robot_x, 20);
  SHOW_SPRITES;

  init(&james_anim, 0, 2, 20);
  init(&robot_anim, ROBOT_SPR_ID, 8, 3);

  loop();
}
