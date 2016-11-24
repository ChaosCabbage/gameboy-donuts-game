#include <gb/gb.h>
#include "Entity.h"
#include "Animation16x16.h"
#include "Sprite16x16.h"
#include "res/james_sprite.h"
#include "res/donut_sprite.h"
#include "res/robot_sprite.h"

/* global game state */
const UINT8 JAMES_X = 75;
UINT8 g_robot_x = 0;
Entity g_donut;

/* global animation data */
UINT8 JAMES_SPR_ID;
UINT8 DONUT_SPR_ID;
UINT8 ROBOT_SPR_ID;

UINT8 JAMES_TILE_OFFSET;
UINT8 DONUT_TILE_OFFSET;
UINT8 ROBOT_TILE_OFFSET;

Animation16x16 g_james_anim;
Animation16x16 g_robot_anim[3];

/* Animation time tracker */
UINT8 g_frame_counter = 0;

/* -- */


void eat_the_donut()
{
  teleport_Entity(&g_donut, 200, g_donut.y);
}

void redraw()
{
  step_Animation16x16(&g_james_anim);
  step_Animation16x16(&g_robot_anim[0]);
  step_Animation16x16(&g_robot_anim[1]);
  step_Animation16x16(&g_robot_anim[2]);
  
  move_Sprite16x16(ROBOT_SPR_ID, g_robot_x, 20);
  move_Sprite16x16(ROBOT_SPR_ID+2, g_robot_x, 40);
  move_Sprite16x16(ROBOT_SPR_ID+4, g_robot_x, 140);
}

void update() 
{
  /*
  step_Entity(&g_donut);
  */

  ++g_frame_counter;
  if (g_frame_counter == 3) {
    g_robot_x -= 1;
    if (g_robot_x == 0) {
      g_robot_x = MAXWNDPOSX;
    }

    g_frame_counter = 0;
  }

  /*
  if (g_donut.x < JAMES_X) {
    eat_the_donut();
  }
  */
}


void loop()
{
  while (1) {
    update();
    redraw();
    wait_vbl_done();
  }
}


void main()
{
  UINT8 tile_count = 0;
  UINT8 sprite_count = 0;
  Animation16x16Info donut_anim;

  g_robot_x = MAXWNDPOSX;

  g_donut.x = 150;
  g_donut.y = 80;

  SPRITES_8x16;
  JAMES_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 2, jamesSprite);
  DONUT_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 1, donutSprite);
  ROBOT_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 8, robotSprite);

  JAMES_SPR_ID = create_Sprite16x16(&sprite_count, JAMES_TILE_OFFSET, JAMES_X, 75);
  DONUT_SPR_ID = create_Sprite16x16(&sprite_count, DONUT_TILE_OFFSET, g_donut.x, g_donut.y);
  ROBOT_SPR_ID = create_Sprite16x16(&sprite_count, ROBOT_TILE_OFFSET, g_robot_x, 20);
  
  /* two more robots: */
  create_Sprite16x16(&sprite_count, ROBOT_TILE_OFFSET, g_robot_x, 40);
  create_Sprite16x16(&sprite_count, ROBOT_TILE_OFFSET, g_robot_x, 140);
  SHOW_SPRITES;

  /* set up the donut */

  donut_anim.sprite_number = DONUT_SPR_ID;
  donut_anim.frame_count = 1;
  donut_anim.tile_offset = DONUT_TILE_OFFSET;
  donut_anim.vblanks_per_frame = 255;
  init_Entity(&g_donut, &donut_anim, g_donut.x, g_donut.y);
  /*
  
  set_speed_Entity(&g_donut, -1, 0);
  */

  init_Animation16x16(&g_james_anim, JAMES_SPR_ID, JAMES_TILE_OFFSET, 2, 20);
  init_Animation16x16(&g_robot_anim[0], ROBOT_SPR_ID, ROBOT_TILE_OFFSET, 8, 3);
  init_Animation16x16(&g_robot_anim[1], ROBOT_SPR_ID + 2, ROBOT_TILE_OFFSET, 8, 3);
  init_Animation16x16(&g_robot_anim[2], ROBOT_SPR_ID + 4, ROBOT_TILE_OFFSET, 8, 3);

  loop();
}
