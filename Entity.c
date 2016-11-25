#include "Entity.h"
#include "Sprite16x16.h"
#include "Animation16x16.h"
#include "Maths.h"

#include <types.h>

static void incr_step(Entity* e);

/* returns TRUE if any movement occurred */
static BOOLEAN update_position(Entity* e);

static void move_sprite(Entity* e);

void init_Entity(Entity* entity, const Animation16x16Info* animation, SpriteTable* sprite_table, UINT8 x, UINT8 y)
{
  UINT8 sprite_id = create_Sprite16x16(sprite_table, animation->tile_offset, x, y);

  entity->x = x;
  entity->y = y;

  set_speed_Entity(entity, 0, 0);

  entity->step_counter = 0;

  init_Animation16x16(&entity->animation, sprite_id, animation);
}

void teleport_Entity(Entity* e, UINT8 x, UINT8 y)
{
  e->x = x;
  e->y = y;
  move_sprite(e);
}

void scroll_Entity(Entity * e, UINT8 dx, UINT8 dy)
{
  e->x += dx;
  e->y += dy;
  move_sprite(e);
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
  move_Sprite16x16(e->animation.sprite_number, e->x, e->y);
}
