#include <gb/gb.h>
#include "Entity.h"
#include "Animation16x16.h"
#include "Sprite16x16.h"
#include "res/james_sprite.h"
#include "res/donut_sprite.h"
#include "res/robot_sprite.h"

/* global game state */
Entity g_donut;
Entity g_james;
Entity g_robot[3];

/* global animation data */
Animation16x16Info JAMES_ANIM;
Animation16x16Info DONUT_ANIM;
Animation16x16Info ROBOT_ANIM;

/* ----- */

void eat_the_donut()
{
  teleport_Entity(&g_donut, 200, g_donut.y);
}

void update() 
{
  const UINT8 offscreen = (UINT8)(-8);
  UINT8 i;

  step_Entity(&g_donut);
  step_Entity(&g_james);

  for (i = 0; i < 3; ++i) {
    step_Entity(&g_robot[i]);
    if (g_robot[i].x == offscreen) {
      teleport_Entity(&g_robot[i], MAXWNDPOSX, g_robot[i].y);
    }
  }

  if (g_donut.x < g_james.x) {
    eat_the_donut();
  }
}


void loop()
{
  while (1) {
    update();
    wait_vbl_done();
  }
}


void main()
{
  UINT8 JAMES_TILE_OFFSET;
  UINT8 DONUT_TILE_OFFSET;
  UINT8 ROBOT_TILE_OFFSET;

  UINT8 tile_count = 0;
  SpriteTable sprite_table;

  g_donut.x = 150;
  g_donut.y = 80;

  SPRITES_8x16;

  /* Load the tiles */
  JAMES_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 2, jamesSprite);
  DONUT_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 1, donutSprite);
  ROBOT_TILE_OFFSET = load_tiles_Sprite16x16(&tile_count, 8, robotSprite);

  /* set up some animation data*/

  JAMES_ANIM.frame_count = 2;
  JAMES_ANIM.tile_offset = JAMES_TILE_OFFSET;
  JAMES_ANIM.vblanks_per_frame = 15;

  DONUT_ANIM.frame_count = 1;
  DONUT_ANIM.tile_offset = DONUT_TILE_OFFSET;
  DONUT_ANIM.vblanks_per_frame = 255;

  ROBOT_ANIM.frame_count = 8;
  ROBOT_ANIM.tile_offset = ROBOT_TILE_OFFSET;
  ROBOT_ANIM.vblanks_per_frame = 3;

  /* Engage Donuts! */
  init_SpriteTable(&sprite_table);
  init_Entity(&g_james, &JAMES_ANIM, &sprite_table, 75, 75);
  init_Entity(&g_donut, &DONUT_ANIM, &sprite_table, g_donut.x, g_donut.y);
  init_Entity(&g_robot[0], &ROBOT_ANIM, &sprite_table, MAXWNDPOSX, 20);
  init_Entity(&g_robot[1], &ROBOT_ANIM, &sprite_table, MAXWNDPOSX, 40);
  init_Entity(&g_robot[2], &ROBOT_ANIM, &sprite_table, MAXWNDPOSX, 140);

  set_speed_Entity(&g_donut, -1, 0);
  set_speed_Entity(&g_robot[0], -3, 0);
  set_speed_Entity(&g_robot[1], -6, 0);
  set_speed_Entity(&g_robot[2], -2, 7);

  SHOW_SPRITES;

  loop();
}
