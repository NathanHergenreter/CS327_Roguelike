#ifndef DUNGEON_H

#define DUNGEON_H

#include "character.h"

//DUNGEON
//Constants - Width and Height of the dungeon as described
#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21
#define MIN_OBJECTS 10

//ROOM
//Constant minimums/maximums for Rooms
#define  MIN_ROOMS 5
#define MAX_ROOMS 10
#define MIN_ROOM_WIDTH 3
#define MIN_ROOM_HEIGHT 2

//TILE
//Tile Types - Each type has a corresponding char
enum tile_type{Tile_Rock, Tile_Room, Tile_Corridor, Tile_Stairs_Up, Tile_Stairs_Down};

//Represents a tile of the dungeon
//Hardness - ? functionality to be known in the future ?
class tile_t
{
 public:
  //hardness - Hardness of cell
  //seen - Determines if PC has seen cell before
  //type - Type of tile the cell is
  //seen_type - Type the cell was as last seen by PC
  //*character - Keeps track of what character is at the cell
  int hardness, seen;
  enum tile_type type, seen_type;
  character_t *character;
  object_t *object;

  tile_t();
  ~tile_t();
};

//Room struct
//Contains starting generation point (x,y) [start[2]]
//Contains furthest possible horizontal/vertical distance from starting point [width/height]
//Contains number of tiles contained within
class room_t
{
 public:
  //x,y - Coordinates of top-left corner
  //width - x dimension of room
  //height - y dimension of room
  //size - Room space
  int x, y, width, height, size;

  room_t();
  room_t(int, int, int, int);
  room_t(int, int);
  ~room_t();
};

//Dungeon
class dungeon_t
{
 public:
  //cells - 2D array of tiles for cells
  //room_count - Keeps track of the number of rooms in dungeon
  //pc_x, pc_y - Keeps track of current PC coordinates
  //rooms - An array of all the rooms in the dungeon
  //nummon - Keeps track of how many monsters are currently in the dungeon
  tile_t cells[DUNGEON_HEIGHT][DUNGEON_WIDTH];
  int room_count, pc_x, pc_y;
  room_t* rooms;
  int nummon;

  dungeon_t(int);
  ~dungeon_t();
};

#endif
