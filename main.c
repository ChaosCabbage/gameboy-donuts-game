#include <gb/gb.h>
#include "Entity.h"
#include "Animation16x16.h"
#include "Sprite16x16.h"
#include "res/james_sprite.h"
#include "res/donut_sprite.h"
#include "res/robot_sprite.h"

/* global game state */
const UINT8 JAMES_X = 75;
Entity g_donut;
Entity g_robot[3];

/* global animation data */
UINT8 JAMES_SPR_ID;
UINT8 DONUT_SPR_ID;
UINT8 ROBOT_SPR_ID;

UINT8 JAMES_TILE_OFFSET;
UINT8 DONUT_TILE_OFFSET;
UINT8 ROBOT_TILE_OFFSET;

Animation16x16 g_james_anim;

/* -- */


void eat_the_donut()
{
  teleport_Entity(&g_donut, 200, g_donut.y);
}

void redraw()
{
  step_Animation16x16(&g_james_anim);
}

void update() 
{
  step_Entity(&g_donut);
  step_Entity(&g_robot[0]);
  step_Entity(&g_robot[1]);
  step_Entity(&g_robot[2]);

  if (g_robot[0].x == 0) {
    teleport_Entity(&g_robot[0], MAXWNDPOSX, g_robot[0].y);
    teleport_Entity(&g_robot[1], MAXWNDPOSX, g_robot[1].y);
    teleport_Entity(&g_robot[2], MAXWNDPOSX, g_robot[2].y);
  }

  if (g_donut.x < JAMES_X) {
    eat_the_donut();
  }
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
  Animation16x16Info robot_anim;

  g_donut.x = 150;
  g_donut.y = 80;

  SPRITES_8x16;
  JAMES_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 2, jamesSprite);
  DONUT_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 1, donutSprite);
  ROBOT_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 8, robotSprite);

  JAMES_SPR_ID = create_Sprite16x16(&sprite_count, JAMES_TILE_OFFSET, JAMES_X, 75);
  DONUT_SPR_ID = create_Sprite16x16(&sprite_count, DONUT_TILE_OFFSET, g_donut.x, g_donut.y);
  ROBOT_SPR_ID = create_Sprite16x16(&sprite_count, ROBOT_TILE_OFFSET, MAXWNDPOSX, 20);
  
  /* two more robots: */
  create_Sprite16x16(&sprite_count, ROBOT_TILE_OFFSET, MAXWNDPOSX, 40);
  create_Sprite16x16(&sprite_count, ROBOT_TILE_OFFSET, MAXWNDPOSX, 140);
  SHOW_SPRITES;

  /* set up some animation data*/

  donut_anim.sprite_number = DONUT_SPR_ID;
  donut_anim.frame_count = 1;
  donut_anim.tile_offset = DONUT_TILE_OFFSET;
  donut_anim.vblanks_per_frame = 255;

  robot_anim.sprite_number = ROBOT_SPR_ID;
  robot_anim.frame_count = 8;
  robot_anim.tile_offset = ROBOT_TILE_OFFSET;
  robot_anim.vblanks_per_frame= 3;

  /* Start your donuts! */
  init_Entity(&g_donut, &donut_anim, g_donut.x, g_donut.y);
  set_speed_Entity(&g_donut, -1, 0);

  /* Robots, GO! */
  init_Entity(&g_robot[0], &robot_anim, 0, 20);
  robot_anim.sprite_number += 2;
  init_Entity(&g_robot[1], &robot_anim, 0, 40);
  robot_anim.sprite_number += 2;
  init_Entity(&g_robot[2], &robot_anim, 0, 140);

  set_speed_Entity(&g_robot[0], -3, 0);
  set_speed_Entity(&g_robot[1], -6, 0);
  set_speed_Entity(&g_robot[2], -2, 0);

  init_Animation16x16(&g_james_anim, JAMES_SPR_ID, JAMES_TILE_OFFSET, 2, 20);

  loop();
}
