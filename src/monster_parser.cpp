//Misc
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include "monster_parser.h"

#define monster_file "/.rlg327/monster_desc.txt"
#define monster_version "RLG327 MONSTER DESCRIPTION 1"
#define monster_start "BEGIN MONSTER"
#define monster_end "END"
#define max_width 77

/********* CONTENTS *********/
//(A) Monster Parser
//void monster_parser()
/********* CONTENTS *********/

// (A) Monster Parser
//Parses through predesignated txt file monster_desc.txt
//Makes new monster templates if valid and places them into list
void monster_parser(std::vector<monster_template_t> &list)
{
  int i;
  std::string s="";

  //Sets the input stream path to predetermined file
  char *path = (char *) malloc( strlen(getenv("HOME")) + strlen(monster_file)+1 );
  strcpy(path, getenv("HOME"));
  strcat(path, monster_file);
  std::ifstream in(path);
  free(path);

  //Checks if first line is required metadata (monster_version)
  getline(in,s);

  if( strcmp(s.c_str(), monster_version) == 0 )
    {
      //Loop goes through entire file until file end
      while( !in.eof() )
	{
	  std::string name = "";
	  std::string description = "";
	  char symbol = ' ';
	  int color=-1;
	  std::vector<std::string> abilities;
	  int hitpoints_base=-1, hitpoints_dice=-1, hitpoints_sides=-1;
	  int damage_base=-1, damage_dice=-1, damage_sides=-1;
	  int speed_base=-1, speed_dice=-1, speed_sides=-1;
	  int rarity=-1;

	  bool valid = true;

	  //Searches for new monster
	  for(getline(in, s); !in.eof() && strcmp(s.c_str(), monster_start)!=0; getline(in, s)){}

	  //New template begun, creates new template
	  for(getline(in, s); !in.eof() && strcmp(s.c_str(), monster_end) != 0; getline(in, s))
	    {
	      std::vector<std::string> strings;

	      std::string cur = "";
	      //Places each separated string into vector
	      for(i=0; i < (int) s.length(); i++)
		{
		  if(s[i]!=' ')
		    {
		      cur+=s[i];
		    }
		  else
		    {
		      strings.push_back(cur);
		      cur = "";
		    }
		}
	      strings.push_back(cur);

	      //Checks what parameter is at the start of the line
	      //If the parameter has not already been added, then it is added to current template
	      std::string param = strings.front();

	      //Name
	      if( strcmp(param.c_str(), "NAME") == 0 && name.length() == 0 )
		{
		  for(i=1; i < (int) strings.size(); i++) { name += strings[i]; name+=" "; }
		}
	      //Description
	      else if( strcmp(param.c_str(), "DESC") == 0 && description.length() == 0 )
		{
		  //Need to check for new line followed by period
		  std::string current="";
		  for(getline(in, current); !in.eof() && current.length()!=1; getline(in, current))
		    {
		      //Checks if width is longer than max
		      if(current.length() > max_width)
			{
			  valid = false;
			  break;
			}
		      description+=current;
		      description+="\n";
		    }
		  description[description.length()-1]='\0';
		}
	      //Color
	      else if( strcmp(param.c_str(), "COLOR") == 0 && color == -1 )
		{
		  color = color_value(strings[1]);
		}
	      //Abilities
	      else if( strcmp(param.c_str(), "ABIL") == 0 && abilities.size() == 0 )
		{
		  int k;
		  for(k=1; k < (int) strings.size(); k++) { abilities.push_back(strings[k]); }
		}
	      //Hitpoints
	      else if( strcmp(param.c_str(), "HP") == 0 && hitpoints_base == -1 )
		{
		  std::string values = strings[1];
		  std::string current;
		  int k=0;

		  //Gets base
		  while(k < (int) values.length())
		    {
		      if(values[k] != '+') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  hitpoints_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  hitpoints_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  hitpoints_sides=atoi(current.c_str());
		}
	      //Damage
	      else if( strcmp(param.c_str(), "DAM") == 0 && damage_base == -1 )
		{
		  std::string values = strings[1];
		  std::string current;
		  int k=0;

		  //Gets base
		  while(k < (int) values.length())
		    {
		      if(values[k] != '+') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  damage_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  damage_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  damage_sides=atoi(current.c_str());
		}
	      //Speed
	      else if( strcmp(param.c_str(), "SPEED") == 0 && speed_base == -1 )
		{
		  std::string values = strings[1];
		  std::string current;
		  int k=0;

		  //Gets base
		  while(k < (int) values.length())
		    {
		      if(values[k] != '+') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  speed_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  speed_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  speed_sides=atoi(current.c_str());
		}
	      //Symbol
	      else if( strcmp(param.c_str(), "SYMB") == 0 && symbol == ' ' )
		{
		  std::string current = strings[1];
		  symbol=current[0];
		}
	      //Rarity
	      else if( strcmp(param.c_str(), "RRTY") == 0 && rarity == -1 )
		{
		  std::string current = strings[1];
		  rarity=atoi(current.c_str());
		}
	      //Invalid format, not added
	      else
		{
		  //REMOVE
		  std::cout<<"Invalid: "<<param<<std::endl;
		  valid=false;
		  break;
		}
	    }

	  //If monster definition was valid, template added to list
	  if(!in.eof() && valid==true)
	    {
	      monster_template_t next;
	      next.name = name;
	      next.description = description;
	      next.symbol = symbol;
	      next.color = color;
	      next.is_available=true;
	      next.abilities = abilities;

	      /*
	      int k;
	      //Sets abilities
	      for(k=0; k < (int) abilities.size(); k++)
		{
		  std::string cur = abilities[k];
		       if( strcmp( cur.c_str(), "ERRATIC" ) == 0 ) { next.attributes[0]=1; }
		  else if( strcmp( cur.c_str(), "TUNNEL" ) == 0 )  { next.attributes[1]=1; }
		  else if( strcmp( cur.c_str(), "SMART" ) == 0 )   { next.attributes[2]=1; }
		  else if( strcmp( cur.c_str(), "TELE" ) == 0 )    { next.attributes[3]=1; }
		  else if( strcmp( cur.c_str(), "PASS" ) == 0 )    { next.attributes[4]=1; }
		  else if( strcmp( cur.c_str(), "PICKUP" ) == 0 )  { next.attributes[5]=1; }
		  else if( strcmp( cur.c_str(), "DESTROY" ) == 0 ) { next.attributes[6]=1; }
		  else if( strcmp( cur.c_str(), "UNIQ" ) == 0 )    { next.attributes[7]=1; }
		  else if( strcmp( cur.c_str(), "BOSS" ) == 0 )    { next.attributes[8]=1; }
		}
	      */

	      next.hitpoints.set(hitpoints_base, hitpoints_dice, hitpoints_sides);
	      next.damage.set(damage_base, damage_dice, damage_sides);
	      next.speed.set(speed_base, speed_dice, speed_sides);
	      next.rarity = rarity;
	      list.push_back(next);
	    }
	}
    }
  //File lacks metadata on first line
  else
    {
      std::cout << "INVALID FILE FORMAT IN " << monster_file << std::endl;
    }

  in.close();
}
