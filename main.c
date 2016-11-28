#include "Entity.h"
#include "Animation16x16.h"
#include "Sprite16x16.h"
#include "Maths.h"

#include "res/james_sprite.h"
#include "res/donut_sprite.h"
#include "res/robot_sprite.h"
#include "res/krispykreme_bkg.h"

#include <gb/gb.h>
#include <gb/rand.h>


/* global game state */
Entity g_donut;
Entity g_james;
Entity g_robot[4];

BOOLEAN g_dead;

/* global animation data */
SpriteTable SPRITE_TABLE;

Animation16x16Info JAMES_ANIM;
Animation16x16Info DONUT_ANIM;
Animation16x16Info ROBOT_ANIM;

/* ----- */

BOOLEAN are_colliding(Entity* e1, Entity* e2)
{
  if ((e1->x + 14) < (e2->x)) { return FALSE; }
  if ((e1->y + 14) < (e2->y)) { return FALSE; }
  if ((e2->x + 14) < (e1->x)) { return FALSE; }
  if ((e2->y + 14) < (e1->y)) { return FALSE; }
  return TRUE;
}

void simple_joypad_movement(Entity* e)
{
  UINT8 buttons = joypad();
  UINT8 x = 0;
  UINT8 y = 0;

  if (buttons & J_RIGHT) { ++x; }
  if (buttons & J_LEFT)  { --x; }
  if (buttons & J_DOWN)  { ++y; }
  if (buttons & J_UP)    { --y; }

  x = e->x + x;
  if (x == MAXWNDPOSX - 9) {
    x = MAXWNDPOSX - 10;
  }
  if (x == MINWNDPOSX) {
    x = MINWNDPOSX+1;
  }

  y = e->y + y;
  if (y == MAXWNDPOSY+2) {
    y = MAXWNDPOSY+1;
  }
  if (y == 15) {
    y = 16;
  }

  teleport_Entity(e, x, y);
}

void eat_the_donut()
{
  teleport_Entity(&g_donut, 200, _rand());
}

/*
 * Do a single step!
 * Basically, we just need to update all the entities.
 *
 * After that, we can mess around with them a bit.
 */
void update() 
{
  /* Since our sprites are 16x16, and the screen starts at pixel 8,
   * they aren't offscreen until they reach -8.
   * (These are unsigned numbers, so this causes an overflow - it's fine)
   */
  const UINT8 offscreen = (UINT8)(-8);
  UINT8 i;

  step_Entity(&g_donut);
  step_Entity(&g_james);

  for (i = 0; i != 4; ++i) {
    step_Entity(&g_robot[i]);
    if (g_robot[i].x == offscreen) {
      teleport_Entity(&g_robot[i], MAXWNDPOSX, g_robot[i].y);
    }

    if (are_colliding(&g_james, &g_robot[i])) {
      g_dead = TRUE;
      set_sprite_prop(g_james.animation.sprite_number, S_FLIPY);
      set_sprite_prop(g_james.animation.sprite_number+1, S_FLIPY);
    }

  }

  if (are_colliding(&g_donut, &g_james)) {
    eat_the_donut();
  }
  if (g_donut.x == offscreen) {
    eat_the_donut();
  }
}

UINT8 g_scroll_counter;
void scroll()
{
  ++g_scroll_counter;
  if (g_scroll_counter == 5) {
    scroll_bkg(0, 1);
    g_scroll_counter = 0;
  }
}

/*
 * Update, forever.
 * Wait for a vertical blank each time, so we get 60 FPS at most.
 */
void loop()
{
  UINT8 death_frame_countdown = 200;

  while (1) {
    if (!g_dead) {
      simple_joypad_movement(&g_james);
    } else {
      --death_frame_countdown;
    }

    update();
    scroll();
    wait_vbl_done();

    if (death_frame_countdown == 0) {
      break;
    }
  }

  DISPLAY_OFF;
}


UINT8 KrispyTiles[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 5, 7, 9,11, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 8,10,12, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 3, 5, 7, 9,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 2, 4, 6, 8,10,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};


void main()
{

  UINT8 JAMES_TILE_OFFSET;
  UINT8 DONUT_TILE_OFFSET;
  UINT8 ROBOT_TILE_OFFSET;

  UINT8 tile_count = 0;


  /* load background */
  set_bkg_data(1, KrispyKremeBkgLen, KrispyKremeBkg);
  set_bkg_tiles(0,  0, 20, 10, KrispyTiles);
  set_bkg_tiles(0, 10, 20, 10, KrispyTiles);
  set_bkg_tiles(0, 20, 20, 10, KrispyTiles);
  SHOW_BKG;

  g_dead = FALSE;

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
  init_SpriteTable(&SPRITE_TABLE);
  init_Entity(&g_james, &JAMES_ANIM, &SPRITE_TABLE, 75, 75);
  init_Entity(&g_donut, &DONUT_ANIM, &SPRITE_TABLE, g_donut.x, g_donut.y);
  init_Entity(&g_robot[0], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 20);
  init_Entity(&g_robot[1], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 40);
  init_Entity(&g_robot[2], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 80);
  init_Entity(&g_robot[3], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 140);

  set_speed_Entity(&g_donut, -1, 0);
  set_speed_Entity(&g_robot[0], -3, 0);

  set_speed_Entity(&g_robot[1], -6, 0);
  g_robot[1].animation.info.vblanks_per_frame = 6;

  set_speed_Entity(&g_robot[2], -2, 0);
  g_robot[2].animation.info.vblanks_per_frame = 2;

  set_speed_Entity(&g_robot[3], -4, 0);
  g_robot[3].animation.info.vblanks_per_frame = 4;

  SHOW_SPRITES;

  initrand(8765);

  loop();
}
