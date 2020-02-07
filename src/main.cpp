//main.c - Contains main function, calls either gen_dungeon or save/loader depending on arguments

//MISC
#include <cstdlib>
#include "dungeon_gen.h"
#include "dungeon.h"
#include "path_finder.h"
#include "heap.h"

//Main Function for Roguelike
int main(int argc, char *argv[])
{
  int nummon=0;


  //Checks for --nummon and value
  if(argc==3 && argv[1][0] == '-' && argv[1][1] == '-')
    {
      if(argv[1][2] == 'n')
	{
	  nummon = atoi(argv[2]);
	}
    }

  //No load, runs completely random dungeon generation
  dungeon_main(0, nummon);


  //End main
  return 0;
}
