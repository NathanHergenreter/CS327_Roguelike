#include <ncurses.h>
#include <stdlib.h>
#include "dungeon_gen.h"

/******** CONTENTS *********/

//(A) PC Action
//void PC_action()

//(B) Monster List
void monster_list(dungeon_t *);

//(C) Display Monster List
void print_monster_list(WINDOW *, character_t[], int, int, dungeon_t *);

//(D) Teleport PC
void teleport_PC(dungeon_t *, int *, int *);

//(E) Display Inventory
void inventory_list(PC_t *);

//(F) Display Equipment
void equipment_list(PC_t *);

//(G) Display Item
void display_item(object_t *);

//(H) Character Select
void select_character(dungeon_t *);

//(I) Display Character
void display_character(character_t *);

/******** CONTENTS *********/


/********** (A) PC Action **********/
//Main function for PC action
void PC_action(int command, int* move_x, int* move_y, dungeon_t *dungeon)
{
  bool valid = false;
  int pc_x=dungeon->pc_x, pc_y=dungeon->pc_y;
  PC_t *PC = (PC_t*) dungeon->cells[pc_y][pc_x].character;
  *move_x=0, *move_y=0;
  init_pair(99, COLOR_WHITE, COLOR_BLACK);

  //Loops until valid input
  while(valid==false)
    {
      attron(COLOR_PAIR(99));
      switch(command)
	{
	  //Move Upper Left
	case KEY_HOME:
	case '7':
	case 'y':
	    if(dungeon->cells[pc_y-1][pc_x-1].type!=Tile_Rock)
	      {
		*move_x+=-1;
		*move_y+=-1;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y-1][pc_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up-Left)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Up-Left) through rock!");
		goto erroneous_input;
	      }

	  //Move Up
	case KEY_UP:
	case '8':
	case 'k':
	    if(dungeon->cells[pc_y-1][pc_x].type!=Tile_Rock)
	      {
		*move_x+=0;
		*move_y+=-1;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y-1][pc_x].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Up) through rock!");
		goto erroneous_input;
	      }

	  //Move Upper Right
	case KEY_PPAGE:
	case '9':
	case 'u':
	    if(dungeon->cells[pc_y-1][pc_x+1].type!=Tile_Rock)
	      {
		*move_x+=1;
		*move_y+=-1;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y-1][pc_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up-Right)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Up-Right) through rock!");
		goto erroneous_input;
	      }

	  //Move Right
	case KEY_RIGHT:
	case '6':
	case 'l':
	    if(dungeon->cells[pc_y][pc_x+1].type!=Tile_Rock)
	      {
		*move_x+=1;
		*move_y+=0;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y][pc_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Right)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Right) through rock!");
		goto erroneous_input;
	      }

	  //Move Down Right
	case KEY_NPAGE:
	case '3':
	case 'n':
	    if(dungeon->cells[pc_y+1][pc_x+1].type!=Tile_Rock)
	      {
		*move_x+=1;
		*move_y+=1;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y+1][pc_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down-Right)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Down-Right) through rock!");
		goto erroneous_input;
	      }

	  //Move Down
	case KEY_DOWN:
	case '2':
	case 'j':
	    if(dungeon->cells[pc_y+1][pc_x].type!=Tile_Rock)
	      {
		*move_x+=0;
		*move_y+=1;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y+1][pc_x].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Down) through rock!");
		goto erroneous_input;
	      }

	  //Move Down Left
	case KEY_END:
	case '1':
	case 'b':
	    if(dungeon->cells[pc_y+1][pc_x-1].type!=Tile_Rock)
	      {
		*move_x+=-1;
		*move_y+=1;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y+1][pc_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down-Left)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Down-Left) through rock!");
		goto erroneous_input;
	      }

	  //Move Left
	case KEY_LEFT:
	case '4':
	case 'h':
	    if(dungeon->cells[pc_y][pc_x-1].type!=Tile_Rock)
	      {
		*move_x+=-1;
		*move_y+=0;
		valid=true;
		break;
	      }
	    else if(dungeon->cells[pc_y][pc_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Left)");
		goto erroneous_input;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Cannot move (Left) through rock!");
		goto erroneous_input;
	      }

	  //Rest
	case KEY_B2:
	case '5':
	case ' ':
	  *move_x+=0;
	  *move_y+=0;
	  valid=true;
	  break;

	  //Go downstairs
	case '>':
	  if(dungeon->cells[pc_y][pc_x].type==Tile_Stairs_Down)
	    {
	      end_dungeon(2);
	      valid=true;
	      break;
	    }
	  else
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Must be on downward staircase (>) to go downstairs!");
	      goto erroneous_input;
	    }

	  //Go upstairs
	case '<':
	  if(dungeon->cells[pc_y][pc_x].type==Tile_Stairs_Up)
	    {
	      end_dungeon(2);
	      valid=true;
	      break;
	    }
	  else
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Must be on upward staircase (<) to go upstairs!");
	      goto erroneous_input;
	    }

	  //Displays list of monsters currently on map
	  //Move does not end
	case 'm':
	  monster_list(dungeon);
	  update_dungeon(dungeon);
	  refresh();
	  attron(COLOR_PAIR(99));
	  command=getch();
	  break;

	  //Disables/Reenables FOW
	  //Move does not end
	case 'f':
	  if(get_FOW())
	    {
	      update_FOW(0);
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "FOW turned off!");
	    }
	  else
	    {
	      update_FOW(1);
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "FOW turned on!");
	    }
	  update_dungeon(dungeon);
	  refresh();
	  command=getch();
	  break;

	  //Teleports PC to any mutable position
	  //Move does not end
	case'g':
	  teleport_PC(dungeon, move_x, move_y);
	  pc_x=dungeon->pc_x;
	  pc_y=dungeon->pc_y;

	  int i,j;
	  //Loop sets surrounding with LOS tiles as seen
	  for(i=pc_y - dungeon->cells[pc_y][pc_x].character->sight; i < pc_y + dungeon->cells[pc_y][pc_x].character->sight+1; i++)
	    {
	      for(j=pc_x - dungeon->cells[pc_y][pc_x].character->sight; j<pc_x + dungeon->cells[pc_y][pc_x].character->sight+1; j++)
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
	  break;

	  //Wear an item
	case 'w':
	  //No items in inventory
	  if(PC->inventory.empty())
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Inventory is empty, cannot equip item!");
	      goto erroneous_input;
	    }
	  //User selects which inventory slot to equip item from
	  else
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Select an inventory slot to equip (valid are 0-%i). Press ESC to exit.", MAX_INVENTORY-1);
	      refresh();
	      int slot;
	      //Loops until proper input
	      while(1)
		{
		  slot=getch();

		  //ESC to end attempt to equip
		  if(slot==27) { mvaddstr(0, 0, "\n"); refresh(); break; }

		  //Given slot contains an item
		  if(slot-'0' < (int) PC->inventory.size())
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Equipped %s!", PC->inventory[slot-'0']->name.c_str() );

		      //If equip slot is empty, equips, removes from inventory
		      if( !(PC->has_equipment(PC->inventory[slot-'0']->type)) )
			{
			  PC->equip(PC->inventory[slot-'0']);
			  PC->inventory.erase(PC->inventory.begin()+slot-'0');
			}
		      //Otherwise, swaps
		      else
			{
			  object_t *temp = PC->inventory[slot-'0'];
			  PC->inventory.erase(PC->inventory.begin()+slot-'0');
			  PC->dequip(PC->get_slot(temp->type));
			  PC->equip(temp);
			}

		      break;
		    }
		  //Valid slot number but slot is empty
		  else if(slot-'0'<MAX_INVENTORY)
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Inventory slot %c is empty! Press ESC to exit.", slot);
		    }
		  //Invalid input
		  else
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Invalid input: '%c' (valid are 0-%i). Press ESC to exit.", slot, MAX_INVENTORY-1);
		    }
		  refresh();
		}
	    }
	  command=getch();
	  break;

	  //Dequip equipment
	case 't':
	  //PC Inventory is full, cannot dequip
	  if(PC->inventory.size()==MAX_INVENTORY)
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Inventory is full, cannot dequip item!");
	      goto erroneous_input;
	    }
	  //Can dequip
	  else
	    {
	      int slot;
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Enter an equipment slot to dequip (a-l). Press ESC to exit. ");
	      refresh();
	      //Loops until proper input
	      while(1)
		{
		  slot=getch();

		  //ESC to exit
		  if(slot==27) { mvaddstr(0, 0, "\n"); refresh(); break; }

		  //If valid slot, dequips said slot
		  if(slot>=97 && slot<=108)
		    {
		      //Checks if PC has given equpiment slot equipped
		      if(PC->has_equipment( PC->get_slot_type(slot) ) )
			{
			  PC->dequip(slot);

			  mvaddstr(0, 0, "\n");
			  mvprintw(0, 0, "Unequipped %s!", PC->inventory[PC->inventory.size()-1]->name.c_str() );
			}
		      //Given slot is empty
		      else
			{
			  mvaddstr(0, 0, "\n");
			  mvprintw(0, 0, "Slot '%c' is empty, cannot dequip. Press ESC to exit.", slot);
			}
		      break;
		    }
		  //Invalid slot
		  else
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Invalid input '%c' (valid is a-l). Press ESC to exit,", slot);
		    }
		  refresh();
		}
	    }
	  command=getch();
	  break;

	  //Drops item
	case 'd':
	  //No object at feet, drops item
	  if( !(dungeon->cells[pc_y][pc_x].object) )
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Select an inventory slot to drop on floor (valid are 0-%i). Press ESC to exit.", MAX_INVENTORY-1);
	      refresh();
	      int slot;
	      //Loops until valid slot input
	      while(1)
		{
		  slot=getch();

		  //ESC to exit
		  if(slot==27) { mvaddstr(0, 0, "\n"); refresh(); break; }

		  //Valid carry slot
		  if(slot-'0' < (int) PC->inventory.size())
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Dropped %s from inventory!", PC->inventory[slot-'0']->name.c_str() );
		      dungeon->cells[pc_y][pc_x].object = PC->inventory[slot-'0'];
		      PC->inventory.erase(PC->inventory.begin()+slot-'0');
		      break;
		    }
		  //Valid slot number but slot is empty
		  else if(slot-'0'<MAX_INVENTORY)
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Inventory slot %c is empty! Press ESC to exit.", slot);
		    }
		  //Invalid input
		  else
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Invalid input: '%c' (valid are 0-%i). Press ESC to exit.", slot, MAX_INVENTORY-1);
		    }

		  refresh();
		}
	    }
	  //Item at feet
	  else
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Item at feet, cannot drop an item!");
	      goto erroneous_input;
	    }
	  command=getch();
	  break;

	  //Expunges item
	case 'x':
	  //PC Inventory is full, cannot dequip
	  if(PC->inventory.empty())
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Inventory is empty, cannot remove item!");
	      goto erroneous_input;
	    }
	  //Removes item
	  else
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Select an inventory slot to permanently remove (valid are 0-%i). Press ESC to exit.", MAX_INVENTORY-1);
	      refresh();
	      int slot;
	      //Loops until valid slot input
	      while(1)
		{
		  slot=getch();

		  //ESC to exit
		  if(slot==27) { mvaddstr(0, 0, "\n"); refresh(); break; }

		  //Valid carry slot
		  if(slot-'0' < (int) PC->inventory.size())
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Removed %s from inventory!", PC->inventory[slot-'0']->name.c_str() );
		      PC->inventory.erase(PC->inventory.begin()+slot-'0');
		      break;
		    }
		  //Valid slot number but slot is empty
		  else if(slot-'0'<MAX_INVENTORY)
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Inventory slot %c is empty! Press ESC to exit.", slot);
		    }
		  //Invalid input
		  else
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Invalid input: '%c' (valid are 0-%i). Press ESC to exit.", slot, MAX_INVENTORY-1);
		    }

		  refresh();
		}
	    }
	  command=getch();
	  break;

	  //Displays PC's inventory
	case 'i':
	  inventory_list(PC);
	  update_dungeon(dungeon);
	  refresh();
	  attron(COLOR_PAIR(99));
	  command=getch();
	  break;

	  //Displays PC's equipment
	case 'e':
	  equipment_list(PC);
	  update_dungeon(dungeon);
	  refresh();
	  attron(COLOR_PAIR(99));
	  command=getch();
	  break;

	  //Displays item description
	case 'I':
	  //PC Inventory is full, cannot dequip
	  if(PC->inventory.empty())
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Inventory is empty, cannot remove item!");
	      goto erroneous_input;
	    }
	  //Removes item
	  else
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Select an inventory slot to display (valid are 0-%i). Press ESC to exit.", MAX_INVENTORY-1);
	      refresh();
	      int slot;
	      //Loops until valid slot input
	      while(1)
		{
		  slot=getch();

		  //ESC to exit
		  if(slot==27) { mvaddstr(0, 0, "\n"); refresh(); break; }

		  //Valid carry slot, displays item description
		  if(slot-'0' < (int) PC->inventory.size())
		    {
		      display_item(PC->inventory[slot-'0']);
		      update_dungeon(dungeon);
		      break;
		    }
		  //Valid slot number but slot is empty
		  else if(slot-'0'<MAX_INVENTORY)
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Inventory slot %c is empty! Press ESC to exit.", slot);
		    }
		  //Invalid input
		  else
		    {
		      mvaddstr(0, 0, "\n");
		      mvprintw(0, 0, "Invalid input: '%c' (valid are 0-%i). Press ESC to exit.", slot, MAX_INVENTORY-1);
		    }

		  refresh();
		}
	    }
	  command=getch();
	  break;

	  //Selects a character whose information will be displayed
	case'L':
	  mvaddstr(0, 0, "\n");
	  mvprintw(0, 0, "Move the cursor over a visible character to display them. Press 'L' to exit.");
	  refresh();
	  select_character(dungeon);
	  update_dungeon(dungeon);
	  mvaddstr(0, 0, "\n");
	  refresh();
	  command=getch();
	  break;

	  //Exits Game
	case 'Q':
	  end_dungeon(-1);
	  valid=true;
	  break;

	  //Erroneous input
	default:
	  mvaddstr(0, 0, "\n");
	  mvprintw(0, 0, "Invalid Input: '%c'", command);
	  erroneous_input:
	  command=getch();
	  break;
	}
      if(command!='Q')
	{
	  refresh();
	}
    }
  if(command!='Q')
    {
      attroff(99);
    }
      
}

/********** (B) Monster List **********/
//Displays monsters in dungeon in a list with distance from PC
void monster_list(dungeon_t *dungeon)
{
  int nummon = dungeon->nummon;
  character_t monsters[nummon];

  int i,j, count=0;
  //Creates an array of the monsters
  for(i=1; i<DUNGEON_HEIGHT-1 && count<nummon; i++)
    {
      for(j=1; j<DUNGEON_WIDTH-1 && count<nummon; j++)
	{
	  if( dungeon->cells[i][j].character!=NULL && 
	      dungeon->cells[i][j].character->type!=CHAR_PC )
	    {
	      monsters[count]=*(dungeon->cells[i][j].character);
	      count++;
	    }
	}
    }

  //Makes window
  WINDOW *win_monster_list;
  int ctr_x, ctr_y, width, height, cur_height, max_height;

  height = 10;
  cur_height=0;
  max_height = count+1<height ? 0 : count+1-height;
  width = 40;
  ctr_y = (DUNGEON_HEIGHT - height)/2 + 1;
  ctr_x = (DUNGEON_WIDTH - width)/2 + 1;
  win_monster_list = newwin(height, width, ctr_y, ctr_x);
  box(win_monster_list, 0, 0);

  print_monster_list(win_monster_list, monsters, count, cur_height, dungeon);

  //Input loop
  int command=getch();
  int exit = 0;
  while(exit!=1)
    {
      mvwaddstr(win_monster_list, 0, 0, "\n");
      mvwprintw(win_monster_list, 0, 0, "Monsters List: Press <ESC> to exit");
      wrefresh(win_monster_list);
      switch(command)
	{
	  //Scrolls up
	case KEY_UP:
	  if(cur_height>0)
	    {
	      cur_height--;
	      print_monster_list(win_monster_list, monsters, count, cur_height, dungeon);
	    }
	  command=getch();
	  break;

	  //Scrolls down
	case KEY_DOWN:
	  if(cur_height<max_height)
	    {
	      cur_height++;
	      print_monster_list(win_monster_list, monsters, count, cur_height, dungeon);
	    }
	  command=getch();
	  break;

	  //Exits monster list (27 is ESC)
	case 27:
	  exit=1;
	  break;

	  //Erroneous input
	default:
	  mvwaddstr(win_monster_list, 0, 0, "\n");
	  mvwprintw(win_monster_list, 0, 0, "Invalid Input: %c", command);
	  wrefresh(win_monster_list);
	  command=getch();
	  break;
	}
    }
  delwin(win_monster_list);
}

/********** (C) Print Monster List **********/
//Prints out a list of all the monsters in a subwindow
void print_monster_list(WINDOW *win_monster_list, character_t monsters[], int nummon, int height, dungeon_t *dungeon)
{
  mvwprintw(win_monster_list, 0, 0, "Monsters List: Press <ESC> to exit");

  //Prints monsters onto window
  int rel_x, rel_y, k;
  //Prints 9 monsters starting from idx at height to height+8
  for(k=height; k<nummon && k<height+9; k++)
    {
      rel_x = dungeon->pc_x-monsters[k].pos[0];
      rel_y = dungeon->pc_y-monsters[k].pos[1];
      mvwprintw(win_monster_list, (k-height)+1, 0, "%c ", monsters[k].symbol);
      if(rel_y>0)
	{
	  wprintw(win_monster_list, "%d North ", abs(rel_y));
	}
      else if(rel_y<0)
	{
	  wprintw(win_monster_list, "%d South ", abs(rel_y));
	}

      if(rel_x>0)
	{
	  wprintw(win_monster_list, "%d West ", abs(rel_y));
	}
      else if(rel_x<0)
	{
	  wprintw(win_monster_list, "%d East ", abs(rel_y));
	}
    }
  wrefresh(win_monster_list);
}


/********** (D) Teleport PC **********/
//Teleports PC to a given location using a cursor
void teleport_PC(dungeon_t *dungeon, int *move_x, int *move_y)
{
  int pc_x = dungeon->pc_x, pc_y = dungeon->pc_y;
  int cur_x = pc_x, cur_y = pc_y;
  int command=0;

  //Loops getting inputs to move teleport position
  //Exits with g
  while(command!='g')
    {
      update_dungeon(dungeon);
      attron(COLOR_PAIR(99));
      mvaddch(cur_y+1, cur_x, '*');
      refresh();
      command=getch();

      switch(command)
	{
	  //Exit
	case 'g':
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Teleported to (%d, %d)!", cur_x, cur_y);
	  break;

	  //Move Upper Left
	case KEY_HOME:
	case '7':
	case 'y':
	    if(dungeon->cells[cur_y-1][cur_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up-Left)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=-1;
		cur_y+=-1;
		break;
	      }

	  //Move Up
	case KEY_UP:
	case '8':
	case 'k':
	    if(dungeon->cells[cur_y-1][cur_x].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=0;
		cur_y+=-1;
		break;
	      }

	  //Move Upper Right
	case KEY_PPAGE:
	case '9':
	case 'u':
	    if(dungeon->cells[cur_y-1][cur_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up-Right)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=1;
		cur_y+=-1;
		break;
	      }

	  //Move Right
	case KEY_RIGHT:
	case '6':
	case 'l':
	    if(dungeon->cells[cur_y][cur_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Right)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=1;
		cur_y+=0;
		break;
	      }

	  //Move Down Right
	case KEY_NPAGE:
	case '3':
	case 'n':
	    if(dungeon->cells[cur_y+1][cur_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down-Right)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=1;
		cur_y+=1;
		break;
	      }

	  //Move Down
	case KEY_DOWN:
	case '2':
	case 'j':
	    if(dungeon->cells[cur_y+1][cur_x].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=0;
		cur_y+=1;
		break;
	      }

	  //Move Down Left
	case KEY_END:
	case '1':
	case 'b':
	    if(dungeon->cells[cur_y+1][cur_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down-Left)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=-1;
		cur_y+=1;
		break;
	      }

	  //Move Left
	case KEY_LEFT:
	case '4':
	case 'h':
	    if(dungeon->cells[cur_y][cur_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Left)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=-1;
		cur_y+=0;
		break;
	      }

	  //Erroneous input
	default:
	  mvaddstr(0, 0, "\n");
	  mvprintw(0, 0, "Invalid Input: %c", command);
	  erroneous_input2:
	  command=getch();
	  break;
	}
    }

  //Updates PC position on dungeons
  character_t *PC = dungeon->cells[pc_y][pc_x].character;
  PC->pos[0]=cur_x;
  PC->pos[1]=cur_y;
  if(dungeon->cells[cur_y][cur_x].character)
    {
      dungeon->cells[cur_y][cur_x].character->is_alive=0;
      dungeon->nummon+=-1;
    }
  dungeon->cells[pc_y][pc_x].character=NULL;
  dungeon->cells[cur_y][cur_x].character=PC;
  dungeon->pc_x=cur_x;
  dungeon->pc_y=cur_y;
  *move_x+=cur_x-pc_x;
  *move_y+=cur_y-pc_y;
}


/********** (E) Display Inventory **********/
void inventory_list(PC_t *PC)
{
  //Makes window
  WINDOW *win_inventory_list;
  int ctr_x, ctr_y, width, height;

  height = 1+MAX_INVENTORY;
  width = 40;
  ctr_y = (DUNGEON_HEIGHT - height)/2 + 1;
  ctr_x = (DUNGEON_WIDTH - width)/2 + 1;
  win_inventory_list = newwin(height, width, ctr_y, ctr_x);
  box(win_inventory_list, 0, 0);

  //Top line
  wprintw(win_inventory_list, "Inventory (Enter 'i' to exit)\n");

  int k;
  //Prints items in inventory out
  for(k=0; k<MAX_INVENTORY; k++)
    {
      wprintw(win_inventory_list, "(%d) ", k);

      //If carry slot is not empty, will print name
      if(k < (int) PC->inventory.size())
	{
	  wprintw(win_inventory_list, "%s", PC->inventory[k]->name.c_str());
	}

      wprintw(win_inventory_list, "\n");
    }
  wrefresh(win_inventory_list);

  int command;

  //Loops until 'i' is input to exit
  while(command!='i')
    {
      command=getch();
    }

  delwin(win_inventory_list);
}


/********** (F) Display Equipment **********/
void equipment_list(PC_t *PC)
{
  //Makes window
  WINDOW *win_equipment_list;
  int ctr_x, ctr_y, width, height;

  height = 13;
  width = 40;
  ctr_y = (DUNGEON_HEIGHT - height)/2 + 1;
  ctr_x = (DUNGEON_WIDTH - width)/2 + 1;
  win_equipment_list = newwin(height, width, ctr_y, ctr_x);
  box(win_equipment_list, 0, 0);

  //Top line
  wprintw(win_equipment_list, "Equipment (Enter 'e' to exit)\n");

  //Prints equipment items out

  //Weapon
  wprintw(win_equipment_list, "Weapon (a): ");
  if(PC->weapon) { wprintw(win_equipment_list, "%s", PC->weapon->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Offhand
  wprintw(win_equipment_list, "Offhand (b): ");
  if(PC->offhand) { wprintw(win_equipment_list, "%s", PC->offhand->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Ranged
  wprintw(win_equipment_list, "Ranged (c): ");
  if(PC->ranged) { wprintw(win_equipment_list, "%s", PC->ranged->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Armor
  wprintw(win_equipment_list, "Armor (d): ");
  if(PC->armor) { wprintw(win_equipment_list, "%s", PC->armor->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Helmet
  wprintw(win_equipment_list, "Helmet (e): ");
  if(PC->helmet) { wprintw(win_equipment_list, "%s", PC->helmet->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Cloak
  wprintw(win_equipment_list, "Cloak (f): ");
  if(PC->cloak) { wprintw(win_equipment_list, "%s", PC->cloak->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Gloves
  wprintw(win_equipment_list, "Gloves (g): ");
  if(PC->gloves) { wprintw(win_equipment_list, "%s", PC->gloves->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Boots
  wprintw(win_equipment_list, "Boots (h): ");
  if(PC->boots) { wprintw(win_equipment_list, "%s", PC->boots->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Amulet
  wprintw(win_equipment_list, "Amulet (i): ");
  if(PC->amulet) { wprintw(win_equipment_list, "%s", PC->amulet->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Light
  wprintw(win_equipment_list, "Light (j): ");
  if(PC->light) { wprintw(win_equipment_list, "%s", PC->light->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Ring 1
  wprintw(win_equipment_list, "Right Ring (k): ");
  if(PC->ring1) { wprintw(win_equipment_list, "%s", PC->ring1->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  //Ring 2
  wprintw(win_equipment_list, "Left Ring (l): ");
  if(PC->ring2) { wprintw(win_equipment_list, "%s", PC->ring2->name.c_str()); }
  else { wprintw(win_equipment_list, "None"); }
  wprintw(win_equipment_list, "\n");

  wrefresh(win_equipment_list);

  int command;

  //Loops until 'e' is input to exit
  while(command!='e')
    {
      command=getch();
    }

  delwin(win_equipment_list);
}


/********** (G) Display Item **********/
void display_item(object_t *item)
{
  //Makes window
  WINDOW *win_item;
  int ctr_x, ctr_y, width, height;

  height = 11;
  width = 77;
  ctr_y = (DUNGEON_HEIGHT - height)/2 + 1;
  ctr_x = (DUNGEON_WIDTH - width)/2 + 1;
  win_item = newwin(height, width, ctr_y, ctr_x);
  box(win_item, 0, 0);

  //Top line
  wprintw(win_item, "%s (Enter 'I' to exit)\n", item->name.c_str());

  //Desc
  wprintw(win_item, "%s", item->description.c_str());

  wrefresh(win_item);

  int command;

  //Loops until 'I' is input to exit
  while(command!='I')
    {
      command=getch();
    }

  delwin(win_item);
}


/********** (H) Display Character - Select **********/
//Uses cursor to select a character who is visible
void select_character(dungeon_t *dungeon)
{
  int pc_x = dungeon->pc_x, pc_y = dungeon->pc_y;
  int cur_x = pc_x, cur_y = pc_y;
  int command=0;

  //Loops getting inputs to move select position
  //Exits with ESC
  while(command!=27)
    {
      mvaddstr(0, 0, "\n");
      mvprintw(0, 0, "Move the cursor on top of a character then enter 't' to display. ESC to exit.!");
      update_dungeon(dungeon);
      attron(COLOR_PAIR(99));
      mvaddch(cur_y+1, cur_x, '*');
      refresh();
      command=getch();

      switch(command)
	{
	  //ESC to escape
	case 27:
	  break;

	  //Displays character on current cursor pos
	case 't':
	  //If cursor is out of sight, cannot display
	  if( get_FOW()==1 &&
	      ( abs(cur_x - dungeon->pc_x) > dungeon->cells[pc_y][pc_x].character->sight || 
		abs(cur_y - dungeon->pc_y) > dungeon->cells[pc_y][pc_x].character->sight ) )
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "Cursor is in FOW!");
	      goto erroneous_input2;
	    }
	    //Checks if there is a character
	  else if(!(dungeon->cells[cur_y][cur_x].character))
	    {
	      mvaddstr(0, 0, "\n");
	      mvprintw(0, 0, "No character to display!");
	      goto erroneous_input2;
	    }
	  //Displays a character if there is a character at current pos
	  else { display_character(dungeon->cells[cur_y][cur_x].character); }
	break;

	  //Move Upper Left
	case KEY_HOME:
	case '7':
	case 'y':
	    if(dungeon->cells[cur_y-1][cur_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up-Left)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=-1;
		cur_y+=-1;
		break;
	      }

	  //Move Up
	case KEY_UP:
	case '8':
	case 'k':
	    if(dungeon->cells[cur_y-1][cur_x].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=0;
		cur_y+=-1;
		break;
	      }

	  //Move Upper Right
	case KEY_PPAGE:
	case '9':
	case 'u':
	    if(dungeon->cells[cur_y-1][cur_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Up-Right)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=1;
		cur_y+=-1;
		break;
	      }

	  //Move Right
	case KEY_RIGHT:
	case '6':
	case 'l':
	    if(dungeon->cells[cur_y][cur_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Right)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=1;
		cur_y+=0;
		break;
	      }

	  //Move Down Right
	case KEY_NPAGE:
	case '3':
	case 'n':
	    if(dungeon->cells[cur_y+1][cur_x+1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down-Right)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=1;
		cur_y+=1;
		break;
	      }

	  //Move Down
	case KEY_DOWN:
	case '2':
	case 'j':
	    if(dungeon->cells[cur_y+1][cur_x].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=0;
		cur_y+=1;
		break;
	      }

	  //Move Down Left
	case KEY_END:
	case '1':
	case 'b':
	    if(dungeon->cells[cur_y+1][cur_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Down-Left)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=-1;
		cur_y+=1;
		break;
	      }

	  //Move Left
	case KEY_LEFT:
	case '4':
	case 'h':
	    if(dungeon->cells[cur_y][cur_x-1].hardness==255)
	      {
		mvaddstr(0, 0, "\n");
		mvprintw(0, 0, "Immutable rock! (Left)");
		goto erroneous_input2;
	      }
	    else
	      {
		mvaddstr(0, 0, "\n");
		cur_x+=-1;
		cur_y+=0;
		break;
	      }

	  //Erroneous input
	default:
	  mvaddstr(0, 0, "\n");
	  mvprintw(0, 0, "Invalid Input: %c", command);
	  erroneous_input2:
	  command=getch();
	  break;
	}
    }
}


/********** (I) Display Character **********/
void display_character(character_t *character)
{
  //Makes window
  WINDOW *win_character;
  int ctr_x, ctr_y, width, height;

  height = 18;
  width = 77;
  ctr_y = (DUNGEON_HEIGHT - height)/2 + 1;
  ctr_x = (DUNGEON_WIDTH - width)/2 + 1;
  win_character = newwin(height, width, ctr_y, ctr_x);
  box(win_character, 0, 0);

  //Top line
  wprintw(win_character, "%s (Enter 't' to exit)\n", character->name.c_str());

  //Desc
  wprintw(win_character, "%s", character->description.c_str());

  wrefresh(win_character);

  int command=0;

  //Loops until 't' is input to exit
  while(command!='t')
    {
      command=getch();
    }

  delwin(win_character);
}
