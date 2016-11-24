#include <gb/gb.h>
#include "Animation16x16.h"
#include "Sprite16x16.h"
#include "res/james_sprite.h"
#include "res/donut_sprite.h"
#include "res/robot_sprite.h"


typedef struct 
{
  UINT8 x;
  UINT8 y;
  BOOLEAN alive;

} donut;


/* global game state */
const UINT8 JAMES_X = 75;
UINT8 g_robot_x = 0;
donut g_donut;

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
  g_donut.x = 200;
}

void redraw()
{
  step_Animation16x16(&g_james_anim);
  step_Animation16x16(&g_robot_anim[0]);
  step_Animation16x16(&g_robot_anim[1]);
  step_Animation16x16(&g_robot_anim[2]);

  move_Sprite16x16(DONUT_SPR_ID, g_donut.x, g_donut.y);
  
  move_Sprite16x16(ROBOT_SPR_ID, g_robot_x, 20);
  move_Sprite16x16(ROBOT_SPR_ID+2, g_robot_x, 40);
  move_Sprite16x16(ROBOT_SPR_ID+4, g_robot_x, 140);
}

void update() 
{
  ++g_frame_counter;
  if (g_frame_counter == 3) {
    g_robot_x -= 1;
    if (g_robot_x == 0) {
      g_robot_x = MAXWNDPOSX;
    }

    g_frame_counter = 0;
  }

  g_donut.x -= 1;
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

  g_robot_x = MAXWNDPOSX;

  g_donut.x = 150;
  g_donut.y = 80;
  g_donut.alive = TRUE;

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

  init_Animation16x16(&g_james_anim, JAMES_SPR_ID, JAMES_TILE_OFFSET, 2, 20);
  init_Animation16x16(&g_robot_anim[0], ROBOT_SPR_ID, ROBOT_TILE_OFFSET, 8, 3);
  init_Animation16x16(&g_robot_anim[1], ROBOT_SPR_ID + 2, ROBOT_TILE_OFFSET, 8, 3);
  init_Animation16x16(&g_robot_anim[2], ROBOT_SPR_ID + 4, ROBOT_TILE_OFFSET, 8, 3);

  loop();
}
