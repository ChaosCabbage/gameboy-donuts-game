#include "Entity.h"
#include "Sprite16x16.h"
#include "Animation16x16.h"

#include <types.h>

#define ABS(x) ((x) > 0 ? (x) : (-(x)))
#define NONZERO_SIGN(x) (((x) > 0) ? 1 : -1)
#define SIGN(x)         (((x) == 0) ? 0 : NONZERO_SIGN(x))


static void incr_step(Entity* e);

/* returns TRUE if any movement occurred */
static BOOLEAN update_position(Entity* e);

static void move_sprite(Entity* e);

void init_Entity(Entity* entity, const Animation16x16Info* animation, UINT8 x, UINT8 y)
{
  entity->x = x;
  entity->y = y;

  entity->inverse_dx = 0;
  entity->inverse_dy = 0;

  entity->step_counter = 0;

  /*
  init_Animation16x16(&entity->animation, 
                      animation->sprite_number, 
                      animation->tile_offset, 
                      animation->frame_count, 
                      animation->vblanks_per_frame);
                      */

  /*
  move_sprite(entity);
  */
}

void teleport_Entity(Entity* e, UINT8 x, UINT8 y)
{
  e->x = x;
  e->y = y;
}

void set_speed_Entity(Entity* e, INT8 inverse_x, INT8 inverse_y)
{
  e->inverse_dx = inverse_x;
  e->inverse_dy = inverse_y;
}

void step_Entity(Entity* e)
{
  /*
  BOOLEAN moved = FALSE;

  incr_step(e);
  moved = update_position(e);
  step_Animation16x16(&e->animation);

  if (moved) {
    move_sprite(e);
  }
  
  */
}

void incr_step(Entity* e) 
{
  ++e->step_counter;
  if (e->step_counter == 128) {
    e->step_counter = 0;
  }
}

BOOLEAN update_position(Entity * e)
{
  BOOLEAN moved = FALSE;

  if (e->step_counter == ABS(e->inverse_dx)) {
    e->x += SIGN(e->inverse_dx);
    moved = TRUE;
  }
  if (e->step_counter == ABS(e->inverse_dy)) {
    e->y += SIGN(e->inverse_dy);
    moved = TRUE;
  }

  return moved;
}

void move_sprite(Entity* e)
{
  move_Sprite16x16(e->animation.info.sprite_number, e->x, e->y);
}
