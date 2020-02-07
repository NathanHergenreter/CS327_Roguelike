//dungeon_gen.c - Dungeon Generator, creates a dungeon with dimensions and room as defined below.
//_ = Rock Tile ;; . = Room Tile ;; # = Corridor Tile

//MISC
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include "dungeon_gen.h"
#include "colors.h"
#include "heap.h"
#include "path_finder.h"
#include "commands.h"
#include "monster_parser.h"
#include "object_parser.h"

//TILE
//Char array of the tile_types output
char tile_char[5] = {' ', '.', '#', '<', '>'};

//boolean
enum boolean{False, True};

//Denotes if PC's current dungeon is up to date
enum boolean new_dungeon=False;

//Denotes if game is over or not
enum boolean end=False;

//Denotes if FOW is enabled
int is_FOW=1;


/******** CONTENTS *****************************/

/* (A) DUNGEON MAIN ****************************/
//void dungeon_main

/* (B) GEN MAIN ********************************/
void gen_main(int, int, character_t *, std::vector<monster_template_t> &, std::vector<object_template_t> &);

/* (C) RUN DUNGEON *****************************/
void run_dungeon(dungeon_t *, heap_t *, std::vector<monster_template_t> &);

/* (D) CHARACTER MOVE **************************/
void character_move(character_t *, dungeon_t *, int *, int *, int);
void PC_move(PC_t *, dungeon_t *, int *, int *, int);
void monster_move(monster_t *, dungeon_t *, int *, int *);

/* (E) UPDATE DUNGEON **************************/
//void update_dungeon()

/* (F) END DUNGEON *****************************/
void end_dungeon(int);


/* (G) Monster LOS *****************************/
int monster_los(monster_t *c, dungeon_t *dungeon);

/* (H) Combat **********************************/
void combat(character_t *, character_t *);

/* (AA) Lesser Functions ************************/
//void print_dungeon()
//int get_FOW()
//void update_FOW(int)

/******** CONTENTS *****************************/

// (A) DUNGEON MAIN
//Main dungeon function
void dungeon_main(int arg_seed, int nummon)
{
  //Gets monster templates
  std::vector<monster_template_t> monster_templates;
  monster_parser(monster_templates);

  //Gets object templates
  std::vector<object_template_t> object_templates;
  object_parser(object_templates);

  PC_t *PC = new PC_t();

  //Initializes screen then begins running dungeon
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  start_color();

  //Runs until game is over
  //gen_main is exited when game is ended or a new dungeon is entered
  while(end==False)
    {
      gen_main(arg_seed, nummon, PC, monster_templates, object_templates);
      arg_seed=0;
      nummon=0;
    }

  delete PC;
}

// (B) GEN MAIN
//Main generation function
void gen_main(int arg_seed, int nummon, character_t *PC, std::vector<monster_template_t> &monster_templates, std::vector<object_template_t> &object_templates)
{
  //Iterators
  int i, j;
  int start_x, start_y, start_room;

  //Randomness Seeding
  int seed = time(NULL);

  //Checks if parameter set and not a save/load switch
  if(arg_seed!=0)
    {
      seed = arg_seed;
    }

  //Seeds rng
  srand(seed);

  //Creates new dungeon
  dungeon_t *dungeon = new dungeon_t(nummon);
  
  //Loop sets surrounding with LOS tiles as seen
  for(i=dungeon->pc_y - PC->sight; i < dungeon->pc_y + PC->sight+1; i++)
    {
      for(j=dungeon->pc_x - PC->sight; j < dungeon->pc_x + PC->sight+1; j++)
	{
	  if(i>=0 && i<DUNGEON_HEIGHT &&
	     j>=0 && j<DUNGEON_WIDTH)
	    {
	      dungeon->cells[i][j].seen=1;
	      dungeon->cells[i][j].seen_type=dungeon->cells[i][j].type;
	    }
	}
    }

  //Begins creation of characters and turn heap
  heap_t heap;
  heap_t *h = &heap;
  heap_init(h, turn_cmp, NULL);

  //Vector of all characters on floor
  std::vector<character_t *> characters;

  //Constructs PC
  characters.push_back(PC);
  characters.front()->turn=0;
  characters.front()->pos[0]=dungeon->pc_x;
  characters.front()->pos[1]=dungeon->pc_y;
  characters.front()->hn = heap_insert(h, characters.front());
  dungeon->cells[dungeon->pc_y][dungeon->pc_x].character = characters.front();


  //Generates a number of monsters as given by nummon
  //First randomly chooses type
  //Then chooses a start position
  for(i=1; i<dungeon->nummon+1; i++)
    {
      int desc;
      //Loops until good roll for template
      while(1)
	{
	  //Gets a random template
	  desc = rand() % monster_templates.size();

	  //Checks if good roll for random template rarity
	  //If so, exits loop
	  if( rand()%100 < monster_templates[desc].rarity  &&
	      monster_templates[desc].is_available )
	    { 
	      break;
	    }
	}

      monster_t *mon = monster_templates[desc].monster_factory();

      //Checks if monster is unique, if so sets as unavailable
      if(mon->attributes[7]==1) { monster_templates[desc].is_available=false; }

      //Loops until valid starting point found
      while(1)
	{
	  //Checks if monster is tunneler and can start in walls
	  if(mon->attributes[1]==1)
	    {
	      start_x = rand()%78+1;
	      start_y = rand()%19+1;
	    }
	  //Monster is not a tunneler and must start in room
	  else
	    {
	      start_room = rand()%dungeon->room_count;
	      start_x = rand()%dungeon->rooms[start_room].width + dungeon->rooms[start_room].x;
	      start_y = rand()%dungeon->rooms[start_room].height + dungeon->rooms[start_room].y;
	    }

	  //Checks if starting point is empty 
	  //Otherwise moves starts random placement over
	  if(dungeon->cells[start_y][start_x].character==NULL) { break; }
	}

      characters.push_back(mon);
      characters.back()->pos[0]=start_x;
      characters.back()->pos[1]=start_y;
      characters.back()->hn = heap_insert(h, characters.back());
      dungeon->cells[start_y][start_x].character = characters.back();
    }

  //Vector of items on floor
  std::vector<object_t *> objects;
  int numobj = rand()%dungeon->room_count + MIN_OBJECTS;

  //Generates a number of monsters as given by nummon
  //First randomly chooses type
  //Then chooses a start position
  for(i=1; i<numobj; i++)
    {
      int desc;
      //Loops until good roll for template
      while(1)
	{
	  //Gets a random template
	  desc = rand() % object_templates.size();

	  //Checks if good roll for random template rarity
	  //If so, exits loop
	  if( rand()%100 < object_templates[desc].rarity) { break; }
	}

      object_t *obj = object_templates[desc].object_factory();

      //Checks if object is artifact, if so removes from templates vector
      if(obj->is_artifact) { object_templates.erase(object_templates.begin()+desc); }

      //Loops until valid starting point found
      while(1)
	{
	  //Gets random point in a room
	  start_room = rand()%dungeon->room_count;
	  start_x = rand()%dungeon->rooms[start_room].width + dungeon->rooms[start_room].x;
	  start_y = rand()%dungeon->rooms[start_room].height + dungeon->rooms[start_room].y;

	  //Checks if starting point is empty room tile
	  //Otherwise moves starts random placement over
	  if(dungeon->cells[start_y][start_x].object==NULL && dungeon->cells[start_y][start_x].type==Tile_Room) { break; }
	}

      objects.push_back(obj);
      dungeon->cells[start_y][start_x].object = objects.back();
    }

  //Runs new dungeon floor
  new_dungeon=True;
  update_dungeon(dungeon);
  run_dungeon(dungeon, h, monster_templates);

  //Resets monster availability
  for(i=0; i < (int) monster_templates.size(); i++) { monster_templates[i].is_available=true; }

  //Dungeon no longer need
  delete dungeon;
}

// (C) RUN DUNGEON
//Runs the dungeon using the given dungeon layout and character priority queue
//Prints after every PC turn
void run_dungeon(dungeon_t *dungeon, heap_t *h, std::vector<monster_template_t> &monster_templates)
{
  character_t *c;
  int command;

 //Goes through the priority queue, makes moves for characters and updates their information
  //Loops until heap is emptied, PC moves to a new dungeon, and the game has not ended
  while((c = (character_t *) heap_remove_min(h)) && new_dungeon==True && end==False)
   { 
     if(c->type==CHAR_PC)
       {
	     //Loop sets surrounding with LOS tiles as seen
	     int i,j;
	     for(i = c->pos[1] - c->sight; i < c->pos[1] + c->sight+1; i++)
	       {
		 for(j = c->pos[0] - c->sight; j < c->pos[0] + c->sight+1; j++)
		   {
		     if(i>=0 && i<DUNGEON_HEIGHT &&
			j>=0 && j<DUNGEON_WIDTH)
		       {
			 dungeon->cells[i][j].seen=1;
			 dungeon->cells[i][j].seen_type=dungeon->cells[i][j].type;
		       }
		   }
	       }

	     update_dungeon(dungeon);
	     refresh();
	     command=getch();
       }

     //Checks if only one character remains
     //If PC died, game would have already been lossed
     //If size is 1, then PC is only survivor and game is won
     if(h->size == 1 || dungeon->nummon==0)
       {
	 update_dungeon(dungeon);
	 end_dungeon(1);
	 break;
       }

     int is_alive = c->is_alive;

     //If c is alive, determines move and updates turn
     //Otherwise, c is remove from heap
     if(is_alive==1)
       {
	 //Removes character from current position

	 int pos_x = c->pos[0];
	 int pos_y = c->pos[1];

	 character_move(c, dungeon, &pos_x, &pos_y, command);


	 //If game exited or over, exits loop
	 if(end==True)
	   {
	     break;
	   }

	 //Checks if there is not a character in the moved-to tile
	 //If so, moves c to that tile
	 if(!(dungeon->cells[pos_y][pos_x].character))
	   {
	     dungeon->cells[ c->pos[1] ] [ c->pos[0] ].character = NULL;
	     dungeon->cells[pos_y][pos_x].character=c;
	     c->pos[0]=pos_x;
	     c->pos[1]=pos_y;
  
	     //Updates PC coords in dungeon
	     if(c->type==CHAR_PC)
	       {
		 dungeon->pc_x = c->pos[0];
		 dungeon->pc_y = c->pos[1];
	       }
	   }
	 else
	   {
	     //Calls combat between current character and target if one is PC
	     if( c->type==CHAR_PC || dungeon->cells[pos_y][pos_x].character->type==CHAR_PC)
	       {
		 combat(c, dungeon->cells[pos_y][pos_x].character);
	       }

	     //Character was killed, updates as required
	     if(dungeon->cells[pos_y][pos_x].character->is_alive==0)
	       {
		 //If target monster can pickup items, drops item on floor
		 if( !(dungeon->cells[pos_y][pos_x].character->inventory.empty()) )
		   {
		     if( !(dungeon->cells[pos_y][pos_x].object) ) 
		       { 
			 dungeon->cells[pos_y][pos_x].object = 
			   dungeon->cells[pos_y][pos_x].character->inventory[0];
		       }
		     else
		       {
			 int q;
			 bool exit=false;
			 //Moves monster's item onto nearest empty tile
			 for(q=1; q<DUNGEON_HEIGHT/2; q++)
			   {
			     int qy;
			     for(qy=-q; qy<q; qy++)
			       {
				 int qx;
				 for(qx=-q; qx<q; qx++)
				   {
				     //Checks if current tile is room and can hold item
				     //If so, places item there
				     if(qx+pos_x>=0 && qx+pos_x<DUNGEON_WIDTH && 
					qy+pos_y>=0 && qy+pos_y<DUNGEON_HEIGHT && 
					(dungeon->cells[pos_y+qy][pos_x+qx].type == Tile_Room ||
					 dungeon->cells[pos_y+qy][pos_x+qx].type == Tile_Corridor) && 
					!(dungeon->cells[pos_y+qy][pos_x+qx].object) )
				       {
					 dungeon->cells[pos_y+qy][pos_x+qx].object = 
					   dungeon->cells[pos_y][pos_x].character->inventory[0];
					 exit=true;
					 break;
				       }
				   }
				 if(exit) { break; }
			       }
			     if(exit) { break; }
			   }
		       }

		     //Clears target's inventory
		     dungeon->cells[pos_y][pos_x].character->inventory.clear();
		   }
	     
		 //If monster is unique and killed, removes from templates vector
		 if(dungeon->cells[pos_y][pos_x].character->attributes[7] == 1)
		   {
		     int q;
		     for(q=0; q < (int) monster_templates.size(); q++)
		       {
			 //Removes killed unique monster from templates list, cannot spawn again
			 if( dungeon->cells[pos_y][pos_x].character->name.compare( monster_templates[q].name)==0)
			   {
			     monster_templates.erase(monster_templates.begin()+q);
			     break;
			   }
		       }
		   }

		 //If PC, game is over
		 if(dungeon->cells[pos_y][pos_x].character->type == CHAR_PC)
		   {
		     dungeon->nummon+=-1;
		     dungeon->cells[pos_y][pos_x].character=c;
		     update_dungeon(dungeon);
		     end_dungeon(0);
		     break;
		   }

		 //Monster killed is a boss, game is ended
		 if(dungeon->cells[pos_y][pos_x].character->attributes[8] == 1)
		   {
		     update_dungeon(dungeon);
		     end_dungeon(1);
		     break;
		   }

		 dungeon->cells[c->pos[1]][c->pos[0]].character = NULL;
		 c->pos[0]=pos_x;
		 c->pos[1]=pos_y;
		 dungeon->cells[pos_y][pos_x].character=c;

		 //Updates PC coords in dungeon
		 if(c->type==CHAR_PC)
		   {
		     dungeon->pc_x = c->pos[0];
		     dungeon->pc_y = c->pos[1];
		   }
	       }
	   }
	 //Checks if object on tile
	 if(dungeon->cells[ c->pos[1] ] [ c->pos[0] ].object)
	   {
	     //If destroyer monster, removes object
	     if(c->attributes[6]==1) { dungeon->cells[ c->pos[1] ] [ c->pos[0] ].object=NULL; }

	     //If PC or pickup monster, adds to inventory
	     if( ( c->type==CHAR_PC && c->inventory.size() < MAX_INVENTORY )
		 || ( c->attributes[5]==1 && c->inventory.size()==0 ) )
	       { 
		 c->inventory.push_back(dungeon->cells[ c->pos[1] ] [ c->pos[0] ].object);

		 //Outputs item that was picked up
		 if(c->type==CHAR_PC)
		   {
		     mvaddstr(0, 0, "\n");
		     mvprintw(0, 0, "Picked up %s!", c->inventory[c->inventory.size()-1]->name.c_str() );
		     refresh();
		   }

		 //Removes item from floor
		 dungeon->cells[ c->pos[1] ] [ c->pos[0] ].object=NULL;
	       }
	   }
	   
	 //Updates turn count and reinserts back into heap
	 c->turn += 1000/c->speed;
	 heap_insert(h, c);
       }
   }

  //Heap is no longer needed
  heap_delete(h);

}

// (D) CHARACTER MOVE
//Calculates character move
//Looks at character type, attributes, and moved to cells harndess
void character_move(character_t *c, dungeon_t *dungeon, int *c_pos_x, int *c_pos_y, int command)
{
   int type = c->type;

   //PC move
   if(type==CHAR_PC)
     {
       PC_move( (PC_t *) c, dungeon, c_pos_x, c_pos_y, command);
     }

   //Monster Move
   else
     {
       monster_move( (monster_t *) c, dungeon, c_pos_x, c_pos_y);
     }
}

//PC move
void PC_move(PC_t *c, dungeon_t *dungeon, int *c_pos_x, int *c_pos_y, int command)
{
  int move_x, move_y;
  mvaddstr(0,0,"\n");
  PC_action(command, &move_x, &move_y, dungeon);

  //Updates character's internal position
  *c_pos_x+=move_x;
  *c_pos_y+=move_y;
}

//Monster move
void monster_move(monster_t *c, dungeon_t *dungeon, int *c_pos_x, int *c_pos_y)
{
  int move_x, move_y;
  int char_x = *c_pos_x;
  int char_y = *c_pos_y;
  int attributes[NUM_ABILITIES];
  int q;
  for(q=0; q<NUM_ABILITIES; q++) { attributes[q] = c->attributes[q]; }
  int PC_known = monster_los(c, dungeon);
  int known_x = c->p_known[0];
  int known_y = c->p_known[1];

  //Checks for random movement
  //Monsters with erratic attribute have 50% of random movement
  if( ( (attributes[0]==1 && rand()%2==1) ||
	PC_known == 0 ) )
    {
      move_x = rand()%3-1;
      move_y = rand()%3-1;
      //Makes sure a move is made
      if(move_x==0 && move_y==0)
	{
	  int add;
	  if(rand()%2==0)
	    {
	      add=1;
	    }
	  else
	    {
	      add=-1;
	    }
	  if(rand()%2==0)
	    {
	      move_x+=add;
	    }
	  else
	    {
	      move_y+=add;
	    }
	}
    }
  //If not intelligent, will move directly towards PC
  else if (attributes[3]==0)
    {
      //Simplified Bresenham's Line Algorithm
      move_x=0, move_y=0;
      move_x = char_x<known_x ? 1 : char_x>known_x ? -1 : 0;
      move_y = char_y<known_y ? 1 : char_y>known_y ? -1 : 0; 
    }
  //Intelligent and PC pos is known
  //Gets a path map of either open or tunnel paths
  else if(attributes[3]==1)
    {
      int new_x = char_x, new_y = char_y;

      //Can tunnel
      if(attributes[1]==1 || attributes[4]==1)
	{
	  dijkstra_path(dungeon, Path_Tunnel, &new_x, &new_y, known_x, known_y);
	}
      //Cannot tunnel
      else
	{
	  dijkstra_path(dungeon, Path_Open, &new_x, &new_y, known_x, known_y);
	}
      move_x = new_x - char_x;
      move_y = new_y - char_y;
    }

  //Checks if can tunnel/non-corporeal and if tile is not outer rock
  if( (attributes[1]==1 || attributes[4]==1)   &&
       char_y+move_y > 0 && char_y+move_y < 20 &&
       char_x+move_x > 0 && char_x+move_x < 79 )
    {
      //Checks if predetermined move is into rock
      if(dungeon->cells[ char_y+move_y ][ char_x+move_x ].type == Tile_Rock)
	{
	  //If hardness is greater than 85, one turn of tunneling must be done to reduce it by 85 if non-corporeal
	  if( attributes[4]==0 && 
	      (dungeon->cells[ char_y+move_y ][ char_x+move_x ].hardness>=85) )
	    {
	      dungeon->cells[ char_y+move_y ][ char_x+move_x ].hardness+=-85;
	      move_x=0;
	      move_y=0;
	    }
	  //Otherwise, if tunneler, hardness set to 0, tile made into corridor
	  //If non-corporeal, no change
	  else if(attributes[4]==0)
	    {
	      dungeon->cells[ char_y+move_y ][ char_x+move_x ].hardness=0;
	      dungeon->cells[ char_y+move_y ][ char_x+move_x ].type=Tile_Corridor;
	    }
	}
    }
  //If can't tunnel, resets individual random moves to zero if leads to wall
  else
    {
      //Checks if predetermined move is into rock
      if(dungeon->cells[ char_y+move_y ][ char_x+move_x ].type == Tile_Rock)
	{
	  //Checks if horizontal move of move_x is not into rock
	  if(dungeon->cells[ char_y ][ char_x+move_x ].type != Tile_Rock)
	    {
	      move_y=0;
	    }
	  //Otherwise, Checks if vertical move of move_y is into rock
	  else if(dungeon->cells[ char_y+move_y ][ char_x ].type != Tile_Rock)
	    {
	      move_x=0;
	    }
	  //Neither direction with move_d is valid
	  else
	    {
	      move_x=0;
	      move_y=0;
	    }
	}
    }

  //Updates character's internal position
  *c_pos_x+=move_x;
  *c_pos_y+=move_y;
}

// (E) UPDATE DUNGEON
//Updates output of current dungeon state
void update_dungeon(dungeon_t *dungeon)
{
  //Iterators
  int i,j;

  int sight = dungeon->cells[dungeon->pc_y][dungeon->pc_x].character->sight;

  //Changes default colors
  //RGB values multiplied by as ncurses goes from 0-1000 instead of 0-255
  //New colors are temporarily modified COLOR_RED
  init_color(COLOR_RED, (255*200)/51, (20*200)/51, (28*200)/51);
  init_color(COLOR_GREEN, (81*200)/51, (161*200)/51, (72*200)/51);
  init_color(COLOR_YELLOW, (248*200)/51, (248*200)/51, (46*200)/51);
  init_color(COLOR_BLUE, (44*200)/51, (66*200)/51, (177*200)/51);
  init_color(COLOR_MAGENTA, (155*200)/51, (51*200)/51, (145*200)/51);
  init_color(COLOR_CYAN, (66*200)/51, (155*200)/51, (255*200)/51);

  /****** NOTE: Pairs are color value + 1 + background value (ie black is 0, grey is 10) *********/

  //Color Pairs - Default
  init_pair(1, COLOR_BLACK, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);
  init_pair(8, COLOR_WHITE, COLOR_BLACK);

  //Adds more colors if possible
  //10's - Grey
  //20's - Light Grey
  //77 and above reserved for blank spaces
  if(COLORS>8)
    {
      //8 - Rock Grey
      init_color(8, (56*200)/51, (46*200)/51, (42*200)/51);

      init_pair(10, 8, 8);
      init_pair(11, COLOR_BLACK, 8);
      init_pair(12, COLOR_RED, 8);
      init_pair(13, COLOR_GREEN, 8);
      init_pair(14, COLOR_YELLOW, 8);
      init_pair(15, COLOR_BLUE, 8);
      init_pair(16, COLOR_MAGENTA, 8);
      init_pair(17, COLOR_CYAN, 8);
      init_pair(18, COLOR_WHITE, 8);

      //9 - Light Rock Grey
      init_color(9, (107*200)/51, (98*200)/51, (94*200)/51);

      init_pair(20, 9, 9);
      init_pair(21, COLOR_BLACK, 9);
      init_pair(22, COLOR_RED, 9);
      init_pair(23, COLOR_GREEN, 9);
      init_pair(24, COLOR_YELLOW, 9);
      init_pair(25, COLOR_BLUE, 9);
      init_pair(26, COLOR_MAGENTA, 9);
      init_pair(27, COLOR_CYAN, 9);
      init_pair(28, COLOR_WHITE, 9);

      //10 - Dark Rock Grey
      init_color(10, (40*200)/51, (30*200)/51, (27*200)/51);
      init_pair(77, COLOR_BLACK, 10);

      //11 - FOW Rock Grey
      init_color(11, (46*200)/51, (38*200)/51, (34*200)/51);
      init_pair(78, COLOR_BLACK, 11);

      //12 - FOW Light Rock Grey
      init_color(12, (86*200)/51, (74*200)/51, (70*200)/51);
      init_pair(79, 9, 12);
    }


  for(i=0; i<DUNGEON_HEIGHT; i++)
    {
      for(j=0; j<DUNGEON_WIDTH; j++)
	{
	  tile_t tile = dungeon->cells[i][j];
	  char c;
	  int color=0;
	  int color_pair=0;

	  //Sets c, char to output
	  //Out of sight with FOW on
	  if(  is_FOW==1 && 
	       ((abs(dungeon->pc_y-i) > sight) || 
		(abs(dungeon->pc_x-j) > sight)) ) { c = tile_char[tile.seen_type]; }
	  //Character on tile
	  else if(tile.character!=NULL) { c = tile.character->symbol; color=tile.character->color; }
	  //No character but object
	  else if(tile.object!=NULL) { c = tile.object->symbol; color=tile.object->color; }
	  //Empty tile
	  else { c = tile_char[tile.type]; color = 7; }

	  //Sets color
	  //Immutable Rock
	  if(tile.hardness == 255) { color_pair = 77; }

	  //Out of sight with FOW on
	  else if(  is_FOW==1 && 
	       ((abs(dungeon->pc_y-i) > sight) || 
		(abs(dungeon->pc_x-j) > sight)) )
	    { 
	      //Gets background color
	      switch(tile.seen_type)
		{
		case 0:
		  color_pair = 78;
		  break;
		case 1:
		case 2:
		case 3:
		case 4:
		  color_pair = 79;
		  break;
		}
	    }

	  //Not in FOW
	  else 
	    {
	      color_pair = color+1;
	      if(COLORS>8)
		{
		  //Gets background color
		  switch(tile.type)
		    {
		    case 0:
		      color_pair += 10;
		      break;
		    case 1:
		    case 2:
		    case 3:
		    case 4:
		      color_pair += 20;
		      break;
		    }
		}
	    }

	  attron(COLOR_PAIR(color_pair));
	  mvaddch(i+1, j, c);
	  attroff(color_pair);
	}
    }
}

// (F) END DUNGEON
//Exits the game and ends ncurses
void end_dungeon(int outcome)
{
  //Dungeon state change

  //New Dungeon
  if(outcome==2)
    {
      new_dungeon=False;
    }
  //Game is ended
  else
    {
      init_pair(99, COLOR_WHITE, COLOR_BLACK);
      attron(COLOR_PAIR(99));
      end=True;
      if(outcome==1)
	{
	  //Print win screen
	  mvaddstr(0, 0, "\n");
	  mvaddstr(0, 0, "Win");
	}
      //Defeat
      else if(outcome==0)
	{
	  //Print loss screen
	  mvaddstr(0, 0, "\n");
	  mvaddstr(0, 0, "Loss");
	}
      //Game exited
      else
	{
	  //Exits
	  mvaddstr(0, 0, "\n");
	  mvaddstr(0, 0, "Exited");
	}

      attroff(COLOR_PAIR(99));
      refresh();
      getch();
      endwin();
    }
}


// (G) MONSTER LOS
//Calculates if monster can see PC
int monster_los(monster_t *c, dungeon_t *dungeon)
{
  //Iterators
  int k;

   int attributes[4];
   attributes[0]=c->attributes[0];
   attributes[1]=c->attributes[1];
   attributes[2]=c->attributes[2];
   attributes[3]=c->attributes[3];
   int type = c->type;
   int sight = c->sight;

   int char_x = c->pos[0];
   int char_y = c->pos[1];
   int pc_x = dungeon->pc_x;
   int pc_y = dungeon->pc_y;
   int pc_room=-1;

   //If PC is in room, gets room index in array
   if(dungeon->cells[pc_y][pc_x].type==Tile_Room)
     {
       for(k=0; k<dungeon->room_count && pc_room<0; k++)
	    {
	      //Checks if character is within the boundaries of room k
	      if( pc_x>=dungeon->rooms[k].x && 
		 (pc_x<=dungeon->rooms[k].x + dungeon->rooms[k].width) &&
		  pc_y>=dungeon->rooms[k].y && 
		 (pc_y<=dungeon->rooms[k].y + dungeon->rooms[k].height) )
		{
		  pc_room=k;
		}
	    }
     }

  //Checks if PC pos is known
  //Telepathic always know
  if(attributes[2]==1)
    {
      c->PC_found(pc_x, pc_y);
    }
  //Checks if PC is nearby, in same room, in same corridor, or corridor connecting to room, updates known coords
  else if(type != CHAR_PC)
    {
      //First checks f nearby
      if( abs(pc_x-char_x) <= sight && abs(pc_y-char_y) <= sight )
	{
	  c->PC_found(pc_x, pc_y);
	}
      //Then checks if in same room or nearby corridor
      else if(dungeon->cells[char_y][char_x].type == Tile_Room)
	{
	  int mon_room=-1;
	  //Gets monster room
	  for(k=0; k<dungeon->room_count && mon_room<0; k++)
	    {
	      //Checks if character is within the boundaries of room k
	      if( char_x>=dungeon->rooms[k].x && 
		 (char_x<=dungeon->rooms[k].x + dungeon->rooms[k].width) &&
		  char_y>=dungeon->rooms[k].y && 
		 (char_y<=dungeon->rooms[k].y + dungeon->rooms[k].height) )
		{
		  mon_room=k;
		}
	    }
	  //PC is in a room
	  if(pc_room>=0)
	    {
	      //Checks if monster is in same room
	      if(pc_room==mon_room)
		{
		  c->PC_found(pc_x, pc_y);
		}
	    }
	  //Checks if PC is in a connecting corridor
	  else if(dungeon->cells[pc_y][pc_x].type==Tile_Corridor)
	    {
	      int found=0, cur_x=pc_x, cur_y=pc_y;
	      //Loops until found or no connecting paths exist
	      while(found==0)
		{
		  int check_x=0, check_y=0;
		  check_x = pc_x<char_x ? 1 : pc_x>char_x ? -1 : 0;
		  check_y = pc_y<char_y ? 1 : pc_y>char_y ? -1 : 0; 

		  //Checks if most direct path is not corridor
		  if(dungeon->cells[cur_y+check_y][cur_x+check_x].type!=Tile_Corridor)
		    {
		      //Checks if next tile is room, stopping if it is
		      //If so, does one final check to see if its monster room
		      if(dungeon->cells[cur_y+check_y][cur_x+check_x].type==Tile_Room)
			{
			  //Checks if monster is in same room
			  if( cur_x>=dungeon->rooms[mon_room].x && 
			      (cur_x<=dungeon->rooms[mon_room].x + dungeon->rooms[mon_room].width) &&
			      cur_y>=dungeon->rooms[mon_room].y && 
			      (cur_y<=dungeon->rooms[mon_room].y + dungeon->rooms[mon_room].height) )
			    {
			      found=1;
			      c->PC_found(pc_x, pc_y);
			    }
			  else
			    {
			      found=-1;
			    }
			}
		      //If so, checks if neighboring tiles are corridors
		      //If path can continue through this tile, check_c is updated
		      else if(dungeon->cells[cur_y+check_y][cur_x].type==Tile_Corridor)
			{
			  check_x=0;
			}
		      else if(dungeon->cells[cur_y][cur_x+check_x].type==Tile_Corridor)
			{
			  check_y=0;
			}
		      //No possible path to monster room
		      else
			{
			  found=-1;
			}
		    }
		  //Prevents infinite loop
		  if(check_x == 0 && check_y == 0)
		    {
		      found=-1;
		    }
		  cur_x+=check_x, cur_y+=check_y;
		}
	    }
	}
      //Checks if monster is in corridor
      else if(dungeon->cells[char_y][char_x].type == Tile_Corridor)
	{
	  //Checks if PC is in the same corridor
	  if(dungeon->cells[pc_y][pc_x].type==Tile_Corridor)
	    {
	      int found=0, cur_x=pc_x, cur_y=pc_y;
	      //Loops until found or no connecting paths exist
	      while(found==0)
		{
		  int check_x=0, check_y=0;
		  check_x = char_x<pc_x ? 1 : char_x>pc_x ? -1 : 0;
		  check_y = char_y<pc_y ? 1 : char_y>pc_y ? -1 : 0; 

		  //Checks if most direct path is not corridor
		  if(dungeon->cells[cur_y+check_y][cur_x+check_x].type!=Tile_Corridor)
		    {
		      //If so, checks if neighboring tiles are corridors
		      //If path can continue through this tile, check_c is updated
		      if(dungeon->cells[cur_y+check_y][cur_x].type==Tile_Corridor)
			{
			  check_x=0;
			}
		      else if(dungeon->cells[cur_y][cur_x+check_x].type==Tile_Corridor)
			{
			  check_y=0;
			}
		      //No possible path to PC position
		      else
			{
			  found=-1;
			}
		    }
		  cur_x+=check_x, cur_y+=check_y;
		  //Checks if next tile is PC's tile
		  if(cur_x==pc_x && cur_y==pc_y)
		    {
		      found=1;
		      c->PC_found(pc_x, pc_y);
		    }
		  //Prevents infinite loop
		  else if(check_x == 0 && check_y == 0)
		    {
		      found=-1;
		    }
		}
	    }
	  //Checks if PC is in room connecting to corridor
	  else if(dungeon->cells[pc_y][pc_x].type==Tile_Room)
	    {
	      int found=0, cur_x=pc_x, cur_y=pc_y;
	      //Loops until found or no connecting paths exist
	      while(found==0)
		{
		  int check_x=0, check_y=0;
		  check_x = char_x<pc_x ? 1 : char_x>char_x ? -1 : 0;
		  check_y = char_y<pc_y ? 1 : char_y>char_y ? -1 : 0; 

		  //Checks if most direct path is not corridor
		  if(dungeon->cells[cur_y+check_y][cur_x+check_x].type!=Tile_Corridor)
		    {
		      //Checks if next tile is room, stopping loop if it is
		      //If so, does one final check to see if its monster room
		      if(dungeon->cells[cur_y+check_y][cur_x+check_x].type==Tile_Room)
			{
			  //Checks if monster is in same room
			  if( cur_x>=dungeon->rooms[pc_room].x && 
			  (cur_x<=dungeon->rooms[pc_room].x + dungeon->rooms[pc_room].width) &&
			      cur_y>=dungeon->rooms[pc_room].y && 
			      (cur_y<=dungeon->rooms[pc_room].y + dungeon->rooms[pc_room].height) )
			    {
			      found=1;
			      c->PC_found(pc_x, pc_y);
			    }
			  else
			    {
			      found=-1;
			    }
			}
		      //If so, checks if neighboring tiles are corridors
		      //If path can continue through this tile, check_c is updated
		      else if(dungeon->cells[cur_y+check_y][cur_x].type==Tile_Corridor)
			{
			  check_x=0;
			}
		      else if(dungeon->cells[cur_y][cur_x+check_x].type==Tile_Corridor)
			{
			  check_y=0;
			}
		      //No possible path to monster room
		      else
			{
			  found=-1;
			}
		    }

		  //Prevents infinite loop
		  if(check_x == 0 && check_y == 0)
		    {
		      found=-1;
		    }
		  cur_x+=check_x, cur_y+=check_y;
		}
	    }
	}
      //If monster is unintelligent and PC is not seen, forgets PC position
      else if(attributes[3]==0)
	{
	  c->PC_lost();
	}
    }

  return c->PC_known();
}

// (H) Combat
void combat(character_t *att, character_t *def)
{
  int damage=0;

  //Damage calc for PC
  if(att->type==CHAR_PC)
    {
      PC_t *PC = (PC_t *) att;

      //If no weapon, damage is default unarmed damage
      if(!(PC->weapon)) { damage = PC->damage.base; }

      //Otherwise, rolls damage for all equipment
      else
	{
	  int k;

	  //Weapon
	  damage=PC->weapon->damage.base;
	  for(k=0; k < PC->weapon->damage.number; k++) { damage += rand()%PC->weapon->damage.sides + 1; }

	  //Offhand
	  if(PC->offhand)
	    {
	      damage+=PC->offhand->damage.base;
	      for(k=0; k < PC->offhand->damage.number; k++) { damage += rand()%PC->offhand->damage.sides + 1; }
	    }
	  //Ranged
	  if(PC->ranged)
	    {
	      damage+=PC->ranged->damage.base;
	      for(k=0; k < PC->ranged->damage.number; k++) { damage += rand()%PC->ranged->damage.sides + 1; }
	    }
	  //Armor
	  if(PC->armor)
	    {
	      damage+=PC->armor->damage.base;
	      for(k=0; k < PC->armor->damage.number; k++) { damage += rand()%PC->armor->damage.sides + 1; }
	    }
	  //Helmet
	  if(PC->helmet)
	    {
	      damage+=PC->helmet->damage.base;
	      for(k=0; k < PC->helmet->damage.number; k++) { damage += rand()%PC->helmet->damage.sides + 1; }
	    }
	  //Cloak
	  if(PC->cloak)
	    {
	      damage+=PC->cloak->damage.base;
	      for(k=0; k < PC->cloak->damage.number; k++) { damage += rand()%PC->cloak->damage.sides + 1; }
	    }
	  //Gloves
	  if(PC->gloves)
	    {
	      damage+=PC->gloves->damage.base;
	      for(k=0; k < PC->gloves->damage.number; k++) { damage += rand()%PC->gloves->damage.sides + 1; }
	    }
	  //Boots
	  if(PC->boots)
	    {
	      damage+=PC->boots->damage.base;
	      for(k=0; k < PC->boots->damage.number; k++) { damage += rand()%PC->boots->damage.sides + 1; }
	    }
	  //Amulet
	  if(PC->amulet)
	    {
	      damage+=PC->amulet->damage.base;
	      for(k=0; k < PC->amulet->damage.number; k++) { damage += rand()%PC->amulet->damage.sides + 1; }
	    }
	  //Light
	  if(PC->light)
	    {
	      damage+=PC->light->damage.base;
	      for(k=0; k < PC->light->damage.number; k++) { damage += rand()%PC->light->damage.sides + 1; }
	    }
	  //Ring 1
	  if(PC->ring1)
	    {
	      damage+=PC->ring1->damage.base;
	      for(k=0; k < PC->ring1->damage.number; k++) { damage += rand()%PC->ring1->damage.sides + 1; }
	    }
	  //Ring 2
	  if(PC->ring2)
	    {
	      damage+=PC->ring2->damage.base;
	      for(k=0; k < PC->ring2->damage.number; k++) { damage += rand()%PC->ring2->damage.sides + 1; }
	    }
	}
    }
  //Rolls a random damage for monster using damage dice
  else
    {
      damage = att->damage.base;
      int k;
      for(k=0; k < att->damage.number; k++) { damage += rand()%att->damage.sides + 1; }
    }

  //Applies damage to target
  def->hitpoints-=damage;
  mvprintw(0, 0, "\n");
  mvprintw(0, 0, "%s took %d damage (%d HP)!", def->name.c_str(), damage, def->hitpoints);


  //If hitpoints drop to 0 or below, target is killed
  if(def->hitpoints<=0) 
    { 
      def->is_alive=0;
      if(def->type==CHAR_Monster)
	{
	  printw(" %s was killed!", def->name.c_str());
	}
      else
	{
	  printw(" %s were killed!", def->name.c_str());
	}
    }
  getch();
}

/* -------------(AA) LESSER FUNCTIONS----------------*/

//Prints the given dungeon by its tiles
//No longer needed?
void print_dungeon(dungeon_t dungeon)
{

  //Iterators
  int i,j;

  //Prints Dungeon
  for(i=0; i<DUNGEON_HEIGHT; i++)
    {
      for(j=0; j<DUNGEON_WIDTH; j++)
	{
	  tile_t tile = dungeon.cells[i][j];
	  char c;

	  //Checks if a character isn't on the tile
	  //If no charcter, prints tile type
	  if(tile.character==NULL)
	    {
	      c = tile_char[tile.type];
	    }
	  //Otherwise prints character
	  else
	    {
	      c = tile.character->symbol;
	      if(tile.character->type==CHAR_PC)
		{
		  printf(Bo_YEL);
		}
	      else
		{
		  printf(Bo_RED);
		}
	    }
	  printf("%c", c);
	  printf(RESET);
	}
      printf("\n");
    }
}

//Returns state of FOW
int get_FOW()
{
  return is_FOW;
}

//Updates FOW state
void update_FOW(int state)
{
  is_FOW=state;
}
