/*
 * Some useful maths macros
 */
#ifndef MATHS_H
#define MATHS_H

#include <types.h>

/* The maximum of two numbers */
#define MAX(x,y)        ((x) > (y) ? (x) : (y))

/* The minimum of two numbers */
#define MIN(x,y)        ((x) < (y) ? (x) : (y))

/* The absolute value of a number */
#define ABS(x)          ((x) > 0 ? (x) : (-(x)))

/* The sign (+1 or -1) of a number if you don't care about zero */
#define NONZERO_SIGN(x) (((x) > 0) ? 1 : -1)

/* The sign (+1, -1 or 0) of a number */
#define SIGN(x)         (((x) == 0) ? 0 : NONZERO_SIGN(x))

/* Greatest common denominator */
UINT8 gcd(UINT8 a, UINT8 b);

/* Lowest common multiple */
UINT8 lcm(UINT8 a, UINT8 b);

#endif 

