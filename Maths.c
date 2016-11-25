#include "Maths.h"

UINT8 gcd(UINT8 a, UINT8 b) 
{
  UINT8 r;
  while (b != 0) {
    r = a % b;
    a = b;
    b = r;
  }
  return a;
}

UINT8 lcm(UINT8 a, UINT8 b)
{
  return (a*b) / gcd(a, b);
}
