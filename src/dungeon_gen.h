#ifndef DUNGEON_GEN_H

#define DUNGEON_GEN_H

#include "dungeon.h"

void dungeon_main(int, int);

void print_dungeon(dungeon_t);

void update_dungeon(dungeon_t *);

dungeon_t construct_dungeon(tile_t[DUNGEON_HEIGHT][DUNGEON_WIDTH], int, int, int, room_t*, int);

void end_dungeon(int);

int get_FOW();

void update_FOW(int);

#endif
