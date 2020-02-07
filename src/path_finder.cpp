#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "dungeon_gen.h"
#include "dungeon.h"
#include "heap.h"
#include "path_finder.h"

#define dim_x 0
#define dim_y 1

//FUNCTIONS
int32_t path_cmp(const void *, const void *);
void print_path(path_open_t path[DUNGEON_HEIGHT][DUNGEON_WIDTH], int, int);
int weight(int, int, dungeon_t *);


//Path finder function for monsters who can only go through open tiles
void dijkstra_path(dungeon_t *d, enum path_type path_type, int *start_x, int *start_y, int known_x, int known_y)
{

  static path_open_t path[DUNGEON_HEIGHT][DUNGEON_WIDTH], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x,y;

  //Initializes path positions
  if(!initialized) 
    {
      for(y=0; y<DUNGEON_HEIGHT; y++)
	{
	  for(x=0; x<DUNGEON_WIDTH; x++)
	    {
	      path[y][x].pos[dim_x] = x;
	      path[y][x].pos[dim_y] = y;
	    }
	}
      initialized=1;
    }

  //Initializes costs
  for(y=0; y<DUNGEON_HEIGHT; y++)
    {
      for(x=0; x<DUNGEON_WIDTH; x++)
	{
	  path[y][x].cost = INT_MAX;
	}
    }

  path[known_y][known_x].cost = 0;

  //Initializes heap using heap_t h and comparator function path_open_cmp
  heap_init(&h, path_cmp, NULL);

  for(y=0; y<DUNGEON_HEIGHT; y++)
    {
      for(x=0; x<DUNGEON_WIDTH; x++)
	{
	  int hardness = d->cells[y][x].hardness;
	  //Checks if open path
	  //If so, checks if tile is not rock, and if so, adds to heap
	  if(path_type == Path_Open && hardness == 0)
	    {
	      path[y][x].hn = heap_insert(&h, &path[y][x]);
	    }

	  //Otherwise, tunneling is taken into account and checks if tile is not immutable
	  //If so, adds to heap
	  else if(path_type == Path_Tunnel && hardness<255)
	    {
	      path[y][x].hn = heap_insert(&h, &path[y][x]);
	    }

	  //Sets rock tiles (open) or immutable tiles (tunnel) to NULL, not added to heap
	  else
	    {
	      path[y][x].hn = NULL;
	    }
	}
    }

  //Dijkstra's Algorithm
  while((p = (path_open_t *) heap_remove_min(&h)))
   { 
     int cur_x = p->pos[dim_x];
     int right = p->pos[dim_x]+1;
     int left = p->pos[dim_x]-1;
     int cur_y = p->pos[dim_y];
     int down = p->pos[dim_y]+1;
     int up = p->pos[dim_y]-1;
     int cost = p->cost;

     p->hn=NULL;

      //Goes in clockwise rotation starting from above: 
      //Up[-1][+0] -- UpRight[-1][+1] -- Right[+0][+1] -- DownRight[+1][+1] -- 
      //Down[+1][+0] -- DownLeft[+1][-1] -- Left[+0][-1] -- UpLeft[-1][-1]

      //Up
      if( ( path[up][cur_x].hn ) &&
	  ( path[up][cur_x].cost > cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[up][cur_x].cost = cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[up][cur_x].hn);
	}

      //UpRight
      if( ( path[up][right].hn ) &&
	  ( path[up][right].cost > cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[up][right].cost = cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[up][right].hn);
	}

      //Right
      if( ( path[cur_y][right].hn ) &&
	  ( path[cur_y][right].cost > cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[cur_y][right].cost = cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[cur_y][right].hn);
	}

      //DownRight
      if( ( path[down][right].hn ) &&
	  ( path[down][right].cost > p->cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[down][right].cost = p->cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[down][right].hn);
	}

      //Down
      if( ( path[down][cur_x].hn ) &&
	  ( path[down][cur_x].cost > cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[down][cur_x].cost = cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[down][cur_x].hn);
	}

      //DownLeft
      if( ( path[down][left].hn ) &&
	  ( path[down][left].cost > cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[down][left].cost = cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[down][left].hn);
	}

      //Left
      if( ( path[cur_y][left].hn ) &&
	  ( path[cur_y][left].cost > cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[cur_y][left].cost = cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[cur_y][left].hn);
	}

      //UpLeft
      if( ( path[up][left].hn ) &&
	  ( path[up][left].cost > cost+weight(cur_x, cur_y, d) )
        )
	{
	    path[up][left].cost = cost+weight(cur_x, cur_y, d);
	    heap_decrease_key_no_replace(&h, path[up][left].hn);
	}

    }

  int i,j;
  int move_x = -1, move_y = -1;
  //Determines next move for character
  //Finds the shortest nearby path
      for(i=-1; i<2; i++)
	{
	  for(j=-1; j<2; j++)
	    {
	      //Ignores starting point
	      if(i==0 && j==0)
		{}
	      else
		{
		  //Checks if current move cost is lower than previous min move cost
		  //Also makes sure checked move isn't to an immutable tile
		  //If so, updates min move
		  if( *(start_y) + i > 0 && *(start_y) + i < 20 &&
		      *(start_x) + j > 0 && *(start_x) + j < 79 &&
		      path[*(start_y)+i][*(start_x)+j].cost < path[*(start_y)+move_y][*(start_x)+move_x].cost )
		    {
		      move_y=i;
		      move_x=j;
		    }
		}
	    }
	}
      *start_x+=move_x;
      *start_y+=move_y;


  //For testing / Assignment-1.03 purposes
  //print_path(path, known_x, known_y);
}

//Comparator for open_path
//Returns -1 if key cost < with cost
//Returns 0 if key cost = with cost
//Returns 1 if key cost > with cost
int32_t path_cmp(const void *key, const void *with)
{
  path_open_t *path_key = (path_open_t *) key;
  path_open_t *path_with = (path_open_t *) with;
  int cost_key = path_key->cost;
  int cost_with = path_with->cost;
  if(cost_key < cost_with)
    {
      return -1;
    }
  else if(cost_key > cost_with)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

//Prints the distance from PC as calculated in dijkstra_path
void print_path(path_open_t path[DUNGEON_HEIGHT][DUNGEON_WIDTH], int pc_x, int pc_y)
{
  int x,y;
  for(y=0; y<DUNGEON_HEIGHT; y++)
    {
      for(x=0; x<DUNGEON_WIDTH; x++)
	{
	  //Checks if path has a proper cost
	  if(path[y][x].cost<INT_MAX)
	    {
	      //Checks if PC location, is so, prints '@'
	      if(y==pc_y && x==pc_x)
		{
		  printf("@");
		}
	      //Otherwise, prints cost%10
	      else
		{
		  int cost = (unsigned int) path[y][x].cost;
		  printf("%d", cost%10);
		}
	    }
	  else
	    {
	      printf(" ");
	    }
	}
      printf("\n");
    }
}

//Calculates the weight based on hardness
int weight(int x, int y, dungeon_t *d)
{
  int hardness = d->cells[y][x].hardness;
  return hardness/85 + 1;
}
