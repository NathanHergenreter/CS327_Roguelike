//Misc
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include "object_parser.h"

#define object_file "/.rlg327/object_desc.txt"
#define object_version "RLG327 OBJECT DESCRIPTION 1"
#define object_start "BEGIN OBJECT"
#define object_end "END"
#define max_width 77

char item_symbol[] = { '*', '|', ')', '}', '[', ']', '(', '{', '/', '=', '"', '_', '~', '?', '!', '$', '\\', ',', '-', '%', '&' };

/********* CONTENTS *********/
//(A) Object Parser
//void object_parser()
/********* CONTENTS *********/

// (A) Object Parser
//Parses through predesignated txt file object_desc.txt
//Makes new object templates if valid and places them into list
void object_parser(std::vector<object_template_t> &list)
{
  int i;
  std::string s="";

  //Sets the input stream path to predetermined file
  char *path = (char *) malloc( strlen(getenv("HOME")) + strlen(object_file)+1 );
  strcpy(path, getenv("HOME"));
  strcat(path, object_file);
  std::ifstream in(path);
  free(path);

  //Checks if first line is required metadata (object_version)
  getline(in,s);

  if( strcmp(s.c_str(), object_version) == 0 )
    {
      //Loop goes through entire file until file end
      while( !in.eof() )
	{
	  std::string name = "";
	  std::string description = "";
	  int is_equipment=-1;
	  char symbol = ' ';
	  int type=0;
	  int color=-1;
	  int hit_base=-1, hit_dice=-1, hit_sides=-1;
	  int damage_base=-1, damage_dice=-1, damage_sides=-1;
	  int dodge_base=-1, dodge_dice=-1, dodge_sides=-1;
	  int defense_base=-1, defense_dice=-1, defense_sides=-1;
	  int weight_base=-1, weight_dice=-1, weight_sides=-1;
	  int speed_base=-1, speed_dice=-1, speed_sides=-1;
	  int attribute_base=-1, attribute_dice=-1, attribute_sides=-1;
	  int value_base=-1, value_dice=-1, value_sides=-1;
	  int is_artifact=-1;
	  int rarity=-1;

	  bool valid = true;

	  //Searches for new object
	  for(getline(in, s); !in.eof() && strcmp(s.c_str(), object_start)!=0; getline(in, s)){}

	  //New template begun, creates new template
	  for(getline(in, s); !in.eof() && strcmp(s.c_str(), object_end) != 0; getline(in, s))
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
	      //Type
	      else if( strcmp(param.c_str(), "TYPE") == 0 && type == 0 )
		{
		  type = type_value(strings[1]);
		  is_equipment = type<12 ? 1 : 0;
		  symbol = item_symbol[type];
		}
	      //Color
	      else if( strcmp(param.c_str(), "COLOR") == 0 && color == -1 )
		{
		  color = color_value(strings[1]);
		}
	      //Hit Bonus
	      else if( strcmp(param.c_str(), "HIT") == 0 && hit_base == -1 )
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

		  hit_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  hit_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  hit_sides=atoi(current.c_str());
		}
	      //Damage Bonus
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
	      //Dodge Bonus
	      else if( strcmp(param.c_str(), "DODGE") == 0 && dodge_base == -1 )
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

		  dodge_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  dodge_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  dodge_sides=atoi(current.c_str());
		}
	      //Defense Bonus
	      else if( strcmp(param.c_str(), "DEF") == 0 && defense_base == -1 )
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

		  defense_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  defense_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  defense_sides=atoi(current.c_str());
		}
	      //Weight Bonus
	      else if( strcmp(param.c_str(), "WEIGHT") == 0 && weight_base == -1 )
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

		  weight_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  weight_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  weight_sides=atoi(current.c_str());
		}
	      //Speed Bonus
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
	      //Attribute
	      else if( strcmp(param.c_str(), "ATTR") == 0 && attribute_base == -1 )
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

		  attribute_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  attribute_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  attribute_sides=atoi(current.c_str());
		}
	      //Value
	      else if( strcmp(param.c_str(), "VAL") == 0 && value_base == -1 )
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

		  value_base=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number of dice rolls
		  while(k < (int) values.length())
		    {
		      if(values[k] != 'd') { current+=values[k]; }
		      else { break; }
		      k++;
		    }

		  value_dice=atoi(current.c_str());
		  current="";
		  k++;

		  //Gets number sides on dice
		  while(k < (int) values.length())
		    {
		      current+=values[k];
		      k++;
		    }

		  value_sides=atoi(current.c_str());
		}
	      //Artifact Status
	      else if( strcmp(param.c_str(), "ART") == 0 && is_artifact == -1)
		{
		  std::string current = strings[1];
		  if( strcmp(current.c_str(), "FALSE") == 0 ) { is_artifact=0; }
		  else if( strcmp(current.c_str(), "TRUE") == 0 ) { is_artifact=1; }
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

	  //If object definition was valid, template added to list
	  if(!in.eof() && valid==true)
	    {
	      object_template_t next;
	      next.name = name;
	      next.description = description;
	      next.type = (item_type) type;
	      next.is_equipment = is_equipment;
	      next.symbol = symbol;
	      next.color = color;
	      next.hit.set(hit_base, hit_dice, hit_sides);
	      next.damage.set(damage_base, damage_dice, damage_sides);
	      next.dodge.set(dodge_base, dodge_dice, dodge_sides);
	      next.defense.set(defense_base, defense_dice, defense_sides);
	      next.weight.set(weight_base, weight_dice, weight_sides);
	      next.speed.set(speed_base, speed_dice, speed_sides);
	      next.attribute.set(attribute_base, attribute_dice, attribute_sides);
	      next.value.set(value_base, value_dice, value_sides);
	      next.is_artifact = is_artifact == 1 ? true : false;
	      next.rarity = rarity;
	      list.push_back(next);
	    }
	}
    }
  //File lacks metadata on first line
  else
    {
      std::cout << "INVALID FILE FORMAT IN " << object_file << std::endl;
    }

  in.close();
}
