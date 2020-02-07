#include <cstdlib>
#include "dungeon.h"

//REMOVE
#include <ncurses.h>
#include <unistd.h>

//boolean
enum boolean{False, True};

/******** CONTENTS **********/

//(A) Tile
//tile_t()
//~tile_t()

//(B) Room
//room_t()
//~room_t()

//(C) Dungeon
//dungeon_t()
//~dungeon_t()

//(D) Construct Layout
void construct_layout(tile_t[][DUNGEON_WIDTH], room_t *, int);

/******** CONTENTS **********/


/********* (A) Tile ***********/
//Constructor for tile_t
tile_t :: tile_t()
{
  hardness=255;
  seen=0;
  type=Tile_Rock;
  seen_type=Tile_Rock;
  character=NULL;
  object=NULL;
}

//Destructor for tile_t
tile_t :: ~tile_t(){}


/********* (B) Room ***********/
//Default Constructor for room_t
room_t :: room_t()
{
  x=0;
  y=0;
  width=0;
  height=0;
  size=0;
}

//Default Constructor for room_t
room_t :: room_t(int set_x, int set_y, int set_width, int set_height)
{
  x=set_x;
  y=set_y;
  width=set_width;
  height=set_height;
  size=width*height;
}

//Room Constructor
//Builds room with given room number and room total
room_t :: room_t(int i, int total)
{

  //Checks if room total is even or odd
  enum boolean is_even;
  if(total%(total/2)==0)
    {
      is_even=True;
    }
  else
    {
      is_even=False;
    }

  //Room Dimension Calculations

  //If in upper half (or both if rooms# is even): horizontal limit is simply the dungeon's width divided by half the total # rooms
  //If in lower half and there are an odd number of rooms: dungeon width is divided by half the room total + 1 as there is one more than than on the upper half
  int horiz_limit;
  if(i<total/2 || is_even==True)
    {
      horiz_limit = (DUNGEON_WIDTH-4)/(total/2);
    }
  else
    {
      horiz_limit = (DUNGEON_WIDTH-4)/( (total/2) + 1);
    }

  //Calculates room width
  //Gets a random number from minimum room width to the width limit (horiz_limit)
  width = rand()%(horiz_limit-MIN_ROOM_WIDTH)+MIN_ROOM_WIDTH;

  //Height limit (vert_limit) is half of the dungeon height-2
  int vert_limit = (DUNGEON_HEIGHT-2)/2;

  //Calculates room height
  //Gets a random number from minimum room height up to the height limit (vert_limit)
  height = rand()%(vert_limit-MIN_ROOM_HEIGHT)+MIN_ROOM_HEIGHT;


  //Starting Point Calculations
  int section_shift;

  //Calculates how far over the x coord needs to be shifted
  //If in the upper half, the shift is simply the width limit times the current room's index in the room array
  //If in the lower half, first checks if the current room is the last and the room total is odd
  //If so, the shift is the product of the width limit and the room's index in the array minus half the total rounded down
  //Otherwise, the shift is the product of the limit and half the total modulo the array index
  if(i<total/2)
    {
      section_shift = horiz_limit*i;
    }
  else
    {
      if(is_even==False && i==total-1)
	{
	  section_shift = horiz_limit * (i - (total/2) );
	}

       else
	 {
	   section_shift = horiz_limit * (i % (total/2) );
	 }
    }

   //Calculates x placement
   //Gets a random number between 0 and and the difference between the width limit and the room width then adds the shift+1 (+1 to prevent placement on outer wall)
   x = rand()%(horiz_limit-width) + section_shift+1;

  //Checks if in upper half first
  //If so, gets a random number between 1 and the difference between the height limit and the room's height plus 1 (+1 to prevent placement on outer wall)
  //Otherwise, if on the bottom half, adds half the dungeon height as well
  if(i<total/2)
    {
      y = rand()%(vert_limit-height) + 1;
    }
  else
    {
      y = rand()%(vert_limit-height) + (DUNGEON_HEIGHT/2)+1;
    }

  //Sets size
  size = width*height;
}

//Destructor for room_t
room_t :: ~room_t(){}


/********* (C) Dungeon ***********/
//Constructor for dungeon_t
dungeon_t :: dungeon_t(int set_nummon)
{
  //Iterators
  int i;

  //Generates number of rooms
  room_count=MIN_ROOMS;
  for(i=0; i<MAX_ROOMS-MIN_ROOMS+1; i++)
  {
    //Gets even value between 0 and (MAX_ROOMS+2)/2
    //If i is greater than or equal to value, breaks
    //Else another room will be added
    if(i>=(rand()%(MAX_ROOMS+3))/2)
      {
	break;
      }
    else
      {
	room_count++;
      }
  }

  //Checks if nummon has been set, if not randomly chooses a value
  if(set_nummon==0)
    {
      nummon = rand()%(room_count/2) + room_count;
    }
  //Sets number of starting monsters
  else
    {
      nummon=set_nummon;
    }

  //Array of rooms
  rooms = (room_t *) malloc(room_count * (sizeof *rooms));

  //Initial Tile Generation
  //Dungeon's height and width are determined above
  construct_layout(cells, rooms, room_count);

  //Randomly generates a starting point for the PC
  int start_room = rand()%room_count;
  pc_x = rand()%rooms[start_room].width + rooms[start_room].x;
  pc_y = rand()%rooms[start_room].height + rooms[start_room].y;
}

//Destructor for tile_t
dungeon_t :: ~dungeon_t()
{
  free(rooms);
}


/********* (D) Construct Layout ***********/
//Constructs dungeon cells
void construct_layout(tile_t cells[][DUNGEON_WIDTH], room_t *rooms, int room_total)
{
  //Iterators
  int i,j,k;

  //Fills in board with rock
  for(i=0; i<DUNGEON_HEIGHT; i++)
    {
      for(j=0; j<DUNGEON_WIDTH; j++)
	{
	  int hardness;
	  //If on the border, hardness is 255
	  if(i==0 || i==DUNGEON_HEIGHT-1 || j==0 || j==DUNGEON_WIDTH-1)
	    {
	      hardness=255;
	    }
	  //Otherwise, updates to a random value between 1-254
	  else
	    {
	      hardness=rand()%254+1;
	    }
	  cells[i][j].hardness=hardness;
	}
    }

  for(i=0; i<room_total; i++)
  {
    /*
    room_t *room = new room_t(i, room_total);
    rooms[i] = *room;
    */
    rooms[i] = *(new room_t(i, room_total));
  }

  //Places rooms onto dungeon 2d array
  for(k=0; k<room_total; k++)
    {
      for(i=rooms[k].y; i<(rooms[k].height+rooms[k].y); i++)
	{
	  for(j=rooms[k].x; j<(rooms[k].width+rooms[k].x); j++)
	    {
	      cells[i][j].hardness=0;
	      cells[i][j].type=Tile_Room;
	    }
	}
    }
  
  for(i=1; i<4; i++)
    {
      //Gets two random rooms for each staircase
      int room_down = rand()%room_total;
      int room_up = rand()%room_total;

      //If == 0, places down stairs
      if(rand()% (i +(i-1)*2 ) == 0)
	{
	  int down_x = rooms[room_down].x + rand()%rooms[room_down].width;
	  int down_y = rooms[room_down].y + rand()%rooms[room_down].height;

	  //Loops until room tile placed upon is not an up stair
	  while(cells[down_y][down_x].type==Tile_Stairs_Up)
	    {
	      down_x = rooms[room_down].x + rand()%rooms[room_down].width;
	      down_y = rooms[room_down].y + rand()%rooms[room_down].height;
	    }

	  cells[down_y][down_x].type=Tile_Stairs_Down;
	}

      //If == 0, places up stairs
      if(rand()% (i +(i-1)*2 ) == 0)
	{
	  int up_x = rooms[room_up].x + rand()%rooms[room_up].width;
	  int up_y = rooms[room_up].y + rand()%rooms[room_up].height;

	  //Loops until room tile placed upon is not a down stair
	  while(cells[up_y][up_x].type==Tile_Stairs_Down)
	    {
	      up_x = rooms[room_up].x + rand()%rooms[room_up].width;
	      up_y = rooms[room_up].y + rand()%rooms[room_up].height;
	    }

	  cells[up_y][up_x].type=Tile_Stairs_Up;
	}
    }


  //Corridor Construction

  //Horizontal construction - connects all rooms in each half to each other
  //Starts somewhere along rightmost edge
  for(k=0; k<room_total-1; k++)
    {
      //Checks if k is the rightmost room of the upper half
      //If so, shifts k over to the leftmost room of the bottom half
      if(k==(room_total/2)-1)
	{
	  k++;
	}

      //Sets the starting coordinates of the corridor
      //Starting point is the x-value to the right of the rooms edge and a random y value within room k's range
      int start_x = rooms[k].x + rooms[k].width;
      int start_y = (rand() % rooms[k].height) + rooms[k].y;
      enum boolean is_reached = False;

      //Loops place corridors from starting point up to the tile left of the next room or until a room tile is reached
      for(i=start_x; i<=rooms[k+1].x && is_reached==False; i++)
        {
         //Checks if a room tile was reached
         //If so, exits loop
	  if(cells[start_y][i].type==Tile_Room)
	    {
	      is_reached=True;
	    }
	  //If not, replaces current tile (should be Tile_Rock) with Tile_Corridor
	   else
	     {
	      cells[start_y][i].hardness=0;
	      cells[start_y][i].type=Tile_Corridor;
	     }
	 }

       //Checks if room was reached
       //If not, corridor is shifted forward to an x-value of the next room
       if(is_reached==False)
	 {
	  //Determines last point corridor was at and the last point the corridor will reach
	  int cur_x = rooms[k+1].x;
	  int end_x = rand()%(rooms[k+1].width)+rooms[k+1].x;

	  //Continues corridor to some x within the next room's range
	  for(i=cur_x; i<=end_x && is_reached==False; i++)
	     {
	      //Checks if a room tile was reached
	      //If so, exits loop
	      if(cells[start_y][i].type==Tile_Room)
	        {
		 is_reached=True;
		}
	       //If not, replaces current tile (should be Tile_Rock) with Tile_Corridor
	       else
		 {
		  cells[start_y][i].hardness=0;
		  cells[start_y][i].type=Tile_Corridor;
		 }
	      }

	   //Determines the y value the corridor will be heading to and whether it is up or down
	   int iter, end_y;

	   //Checks if above, if so will move up
	   if(start_y>rooms[k+1].y)
	     {
	      iter=-1;
	      end_y=rooms[k+1].y+rooms[k+1].height-1;
	     }
	    //Otherwise moves down
	    else
	     {
	      iter=1;
	      end_y=rooms[k+1].y+1;
	     }
	    //Continues the corridor up/down
	    for(j=start_y; j!=end_y && is_reached==False; j+=iter)
	     {
	      //Checks if a room tile was reached
	      //If so, exits loop
	      if(cells[j][end_x].type==Tile_Room)
	        {
		 is_reached=True;
	        }
	      //If not, replaces current tile (should be Tile_Rock) with Tile_Corridor
	      else
	        {
		 cells[j][end_x].hardness=0;
		 cells[j][end_x].type=Tile_Corridor;
	        }
	      }
	}
    }

  //Vertical Construction - Adds vertical corridors between rooms
  //At least one corridor crossing the horizontal divide must exist
  enum boolean has_vert=False;
  enum boolean needs_vert=False;

  //Loop only goes through top half
  //Randomly determines if a connection to a vertically adjacent should be made
  for(k=0; k<room_total/2; k++)
    {
      int det;
      //Gets a random number between 0-9
      //Checks if loop iteration was completed without any vertical corridors made
      //If so, random room chosen is guaranteed a corridor to prevent infinite loop (e.g. random number is always greater than 3)
      if(needs_vert==False)
	{
	  det = rand()%10;
	}
      else
	{
	  det = 0;
	  needs_vert=False;
	}

      //0-3, makes a corridor
      if(det<4)
	{
	  //Starts somewhere along the bottom row of the room
	  int start_x = rand()%(rooms[k].width) + rooms[k].x;
	  int start_y = rooms[k].y + rooms[k].height;
	  int end_y = rooms[k+(room_total/2)].y+1;
	  enum boolean is_reached=False;

	  //Loops builds corridor down to the horizontal above the room below
	  for(j = start_y; j<end_y && is_reached==False; j++)
	    {
	      //Checks if a room tile was reached
	      //If so, exits loop
	      if(cells[j][start_x].type==Tile_Room)
	        {
		 is_reached=True;
		 has_vert=True;
	        }
	      //If not, replaces current tile (should be Tile_Rock) with Tile_Corridor
	      else
	        {
		 cells[j][start_x].hardness=0;
		 cells[j][start_x].type=Tile_Corridor;
	        }
	    }

	  //Checks if the room has yet to be reached
	  //If not, the corridor continues building horizontally
	  
	  //Determines the x value the corridor will be heading to and whether it is left or right
	  int iter, end_x;

	  //Checks if to the left, if so will move left
	  if(start_x>rooms[k+1].x)
	    {
             iter=-1;
             end_x=rooms[k+(room_total/2)].x+rooms[k+(room_total/2)].width-1;
            }
          //Otherwise moves right
          else
            {
             iter=1;
             end_x=rooms[k+(room_total/2)].x+1;
            }
          //Continues the corridor up/down
          for(i=start_x; i!=end_x && is_reached==False; i+=iter)
            {
            //Checks if a room tile was reached
            //If so, exits loop
            if(cells[end_y][i].type==Tile_Room)
              {
		is_reached=True;
		has_vert=True;
	      }
	    //If not, replaces current tile (should be Tile_Rock) with Tile_Corridor
	    else
	      {
	       cells[end_y][i].hardness=0;
	       cells[end_y][i].type=Tile_Corridor;
	      }
	    }
	}

      //Checks if the last room has been passed and there is no connecting verticals
      //If so, a random room will be chosen and the loop will be gone through again
      if(has_vert==False)
	{
	  k=rand()%(room_total/2)-1;
	  needs_vert=True;
	}
    }
}
