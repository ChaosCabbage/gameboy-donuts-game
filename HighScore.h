/*
 * Global and permanent storage of the high score using external ram.
 */
#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <asm/gbz80/types.h>

UINT16 load_high_score();
void write_high_score(UINT16 new_score);

#endif
