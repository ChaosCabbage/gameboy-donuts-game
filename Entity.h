/*
 * The Entity type.
 * An Entity is a 16x16 animated sprite with a location and a speed.
 *
 */
#ifndef ENTITY_H
#define ENTITY_H

#include "Animation16x16.h"

typedef struct 
{
  /* screen position */
  UINT8 x;
  UINT8 y;
  
  /* 
   * INVERSE x and y speed 
   * 
   * (inverse_dx ==  3) means the entity moves +1 x every 3 frames.
   * (inverse_dx == -4) means the entity moves -1 x every 4 frames.
   * (inverse_dx ==  0) is a special case: The entity is not moving.
   *
   */
  INT8 inverse_dx;
  INT8 inverse_dy;

  Animation16x16 animation;

  UINT8 step_counter;

} Entity;

/*
 * Entity::init
 *
 * Animation data is COPIED, and does not need to live on.
 *
 */
void init_Entity(Entity* e, const Animation16x16Info* animation, UINT8 x, UINT8 y);

void teleport_Entity(Entity* e, UINT8 x, UINT8 y);

void set_speed_Entity(Entity* e, INT8 inverse_x, INT8 inverse_y);

void step_Entity(Entity* e);

#endif
