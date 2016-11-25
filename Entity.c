#include "Entity.h"
#include "Sprite16x16.h"
#include "Animation16x16.h"

#include <types.h>

#define MAX(x,y)        ((x) > (y) ? (x) : (y))
#define ABS(x)          ((x) > 0 ? (x) : (-(x)))
#define NONZERO_SIGN(x) (((x) > 0) ? 1 : -1)
#define SIGN(x)         (((x) == 0) ? 0 : NONZERO_SIGN(x))


static UINT8 gcd(UINT8 a, UINT8 b) {
  UINT8 r;
  while (b != 0) {
    r = a % b;
    a = b;
    b = r;
  }
  return a;
}

static UINT8 lcm(UINT8 a, UINT8 b) {
  return (a*b) / gcd(a,b);
}



static void incr_step(Entity* e);

/* returns TRUE if any movement occurred */
static BOOLEAN update_position(Entity* e);

static void move_sprite(Entity* e);

void init_Entity(Entity* entity, const Animation16x16Info* animation, UINT8 x, UINT8 y)
{
  entity->x = x;
  entity->y = y;

  set_speed_Entity(entity, 0, 0);

  entity->step_counter = 0;

  init_Animation16x16(&entity->animation, 
                      animation->sprite_number, 
                      animation->tile_offset, 
                      animation->frame_count, 
                      animation->vblanks_per_frame);
    
  move_sprite(entity);
}

void teleport_Entity(Entity* e, UINT8 x, UINT8 y)
{
  e->x = x;
  e->y = y;
}

void set_speed_Entity(Entity* e, int inverse_x, int inverse_y)
{
  e->inverse_dx = inverse_x;
  e->inverse_dy = inverse_y;
  e->step_lcm = lcm(ABS(inverse_x), ABS(inverse_y));

  if (e->step_lcm == 0) {
    /* No point doing all the movement stuff on every frame */
    e->step_lcm = 255;
  }
}

void step_Entity(Entity* e)
{
  BOOLEAN moved = FALSE;

  incr_step(e);
  moved = update_position(e);
  if (moved) {
    move_sprite(e);
  }

  step_Animation16x16(&e->animation);

}

void incr_step(Entity* e) 
{
  ++e->step_counter;

  /* To ensure that the movement doesn't skip a frame,
   * the step counter is reset on the greatest common divisior of movement frames.
   */
  if (e->step_counter == e->step_lcm) {
    e->step_counter = 0;
  }
}

BOOLEAN is_movement_frame(UINT8 counter, int frames_per_step)
{
  return (counter % frames_per_step) == 0;
}

BOOLEAN update_position(Entity * e)
{
  BOOLEAN moved = FALSE;

  if (is_movement_frame(e->step_counter, ABS(e->inverse_dx))) {
    e->x += SIGN(e->inverse_dx);
    moved = TRUE;
  }
  if (is_movement_frame(e->step_counter, ABS(e->inverse_dy))) {
    e->y += SIGN(e->inverse_dy);
    moved = TRUE;
  }

  return moved;
}

void move_sprite(Entity* e)
{
  move_Sprite16x16(e->animation.info.sprite_number, e->x, e->y);
}
