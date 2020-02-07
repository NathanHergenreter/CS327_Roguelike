#ifndef PATH_FINDER_H

#define PATH_FINDER_H

//ENUMS
enum path_type{Path_Open, Path_Tunnel};

//STRUCTS
//Path struct - contains x,y coords in an array, a cost, and a heap_node pointer
typedef struct path_open {
  int pos[2];
  int cost;
  heap_node_t *hn;
} path_open_t;

void dijkstra_path(dungeon_t *, enum path_type, int *, int *, int, int);

#endif
