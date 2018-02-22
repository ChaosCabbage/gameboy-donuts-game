#include "BitTwiddling.h"
#include "HighScore.h"
#include "Entity.h"
#include "Animation16x16.h"
#include "Sprite16x16.h"
#include "Maths.h"

#include "res/james_sprite.h"
#include "res/donut_sprite.h"
#include "res/robot_sprite.h"
#include "res/numbers.h"

#include <gb/gb.h>
#include <gb/rand.h>


/* global game state */
Entity g_donut;
Entity g_robot[4];

struct {
  Entity ent;
  BOOLEAN dead;
} g_james;


UINT16 g_score;

/* global animation data */
SpriteTable SPRITE_TABLE;

Animation16x16Info JAMES_ANIM;
Animation16x16Info DONUT_ANIM;
Animation16x16Info ROBOT_ANIM;

/* ----- */

void set_score_bg_map(UINT8 x, UINT8 y, UINT16 score)
{
  /*
   * The score is exactly 5 background tiles.
   * The numbers 0-9 are already loaded into background tiles, starting at index 1.
  */
  UINT8 ten_thousands = score / 10000;
  UINT8 thousands = (score / 1000) % 10;
  UINT8 hundreds = (score / 100) % 10;
  UINT8 tens = (score / 10) % 10;
  UINT8 units = score % 10;

  unsigned char score_map[] = {
    ten_thousands + 1,
    thousands + 1,
    hundreds + 1,
    tens + 1,
    units + 1
  };

  set_bkg_tiles(x, y, 5, 1, score_map);
}

void show_score()
{
  set_score_bg_map(14, 0, g_score);
}

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

void go_to_random_right_of_screen(Entity* e)
{
  UINT8 y = _rand();
  if (y > 16 && y < MAXWNDPOSY) {
    teleport_Entity(e, 200, y);
  } else {
    go_to_random_right_of_screen(e);
  }
}

void teleport_the_donut()
{
  go_to_random_right_of_screen(&g_donut);
}

void eat_the_donut()
{
  teleport_the_donut();
  ++g_score;
  show_score();
}

void set_robot_speed(Entity* robot, UINT8 inverse_x)
{
  robot->animation.info.vblanks_per_frame = inverse_x;
  set_speed_Entity(robot, -inverse_x, 0);
}

void set_random_robot_speed(Entity* robot)
{
  UINT8 y = _rand();
  if (y > 220) {
    set_robot_speed(robot, 1);
  } else if (y > 184) {
    set_robot_speed(robot, 2);
  } else if (y > 148) {
    set_robot_speed(robot, 3);
  } else if (y > 112) {
    set_robot_speed(robot, 4);
  } else if (y > 76) {
    set_robot_speed(robot, 5);
  } else if (y > 40) {
    set_robot_speed(robot, 6);
  } else {
    set_robot_speed(robot, 7);
  }
}

void kill_james()
{
  g_james.dead = TRUE;
  set_sprite_prop(g_james.ent.animation.sprite_number,     S_FLIPY);
  set_sprite_prop(g_james.ent.animation.sprite_number + 1, S_FLIPY);
}

void reset_james()
{
  g_james.dead = FALSE;  
  set_sprite_prop(g_james.ent.animation.sprite_number,     0);
  set_sprite_prop(g_james.ent.animation.sprite_number + 1, 0);
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
  step_Entity(&g_james.ent);

  for (i = 0; i != 4; ++i) {
    step_Entity(&g_robot[i]);
    if (g_robot[i].x == offscreen) {
      go_to_random_right_of_screen(&g_robot[i]);
      set_random_robot_speed(&g_robot[i]);
    }

    if (are_colliding(&g_james.ent, &g_robot[i])) {
      kill_james();
    }

  }

  if (are_colliding(&g_donut, &g_james.ent)) {
    eat_the_donut();
  }
  if (g_donut.x == offscreen) {
    teleport_the_donut();
  }
}

/*
 * Update, forever.
 * This is the main loop.
 * Wait for a vertical blank each time, so we get 60 FPS at most.
 */
void loop()
{
  UINT8 death_frame_countdown = 255;

  while (1) {
    if (!g_james.dead) {
      simple_joypad_movement(&g_james.ent);
    } else {
      --death_frame_countdown;
    }

    update();
    wait_vbl_done();

    if (death_frame_countdown == 0) {
      break;
    }
  }
}

void display_high_score(UINT16 high_score)
{
  UINT8 count = 100;
  set_score_bg_map(6, 6, high_score);
  SHOW_BKG;
  DISPLAY_ON;
  enable_interrupts();
  while (--count) {
    wait_vbl_done();
  }
  display_off();
  disable_interrupts();
  wipe_memory((UINT8*)0x98C6, (UINT8*)0x98CB, 0);  
}

/*
 * Set up and play the game.
 */
void game_scene()
{
  UINT8 JAMES_TILE_OFFSET;
  UINT8 DONUT_TILE_OFFSET;
  UINT8 ROBOT_TILE_OFFSET;

  UINT8 tile_count = 0;

  UINT16 high_score = load_high_score();

  HIDE_SPRITES;
  HIDE_BKG;
  SPRITES_8x16;

  g_donut.x = 150;
  g_donut.y = 80;

  /* Load the score tiles */
  set_bkg_data(1, 10, Numbers);

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
  init_Entity(&g_james.ent, &JAMES_ANIM, &SPRITE_TABLE, 75, 75);
  init_Entity(&g_donut, &DONUT_ANIM, &SPRITE_TABLE, g_donut.x, g_donut.y);
  init_Entity(&g_robot[0], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 20);
  init_Entity(&g_robot[1], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 40);
  init_Entity(&g_robot[2], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 80);
  init_Entity(&g_robot[3], &ROBOT_ANIM, &SPRITE_TABLE, MAXWNDPOSX, 140);

  set_speed_Entity(&g_donut, -1, 0);

  set_robot_speed(&g_robot[0], 3);
  set_robot_speed(&g_robot[1], 6);
  set_robot_speed(&g_robot[2], 2);
  set_robot_speed(&g_robot[3], 4);

  display_high_score(high_score);

  SHOW_SPRITES;
  SHOW_BKG;

  initrand(DIV_REG);

  reset_james();
  g_score = 0;
  show_score();

  DISPLAY_ON;
  enable_interrupts();

  loop();

  if (g_score > high_score) {
    write_high_score(g_score);
  }
}
