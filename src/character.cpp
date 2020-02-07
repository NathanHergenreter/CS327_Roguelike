using namespace std;

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "character.h"
#include "dungeon_gen.h"
#include "path_finder.h"
#include "commands.h"


//Character attributes
enum attributes{Erratic, Tunneler, Intelligent, Telepathic};

/******* CONTENTS ********/

//int32_t turn_cmp()
//int color_value()
//int type_value()

//(A) Superclass Character
//character_t()
//~character_t()
//void set_attributes()
//void set_pos()

//(B) Subclass PC
//PC_t()

//(C) Subclass Monster
//monster_t()
//int PC_known()
//void PC_found()
//void PC_lost()
//void update_symbol()

//(AA) Class Monster Template
//void print_template()

/******* CONTENTS ********/


//Comparator for open_path
//Returns -1 if key turn < with turn
//Returns 0 if key turn = with turn
//Returns 1 if key turn > with turn
int32_t turn_cmp(const void *key, const void *with)
{
  character_t *character_key = (character_t *) key;
  character_t *character_with = (character_t *) with;
  int turn_key = character_key->turn;
  int turn_with = character_with->turn;
  if(turn_key < turn_with)
    {
      return -1;
    }
  else if(turn_key > turn_with)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

//Gets the color value for given color
int color_value(std::string color)
{
       if(strcmp(color.c_str(), "RED")==0)     { return 1; }
  else if(strcmp(color.c_str(), "GREEN")==0)   { return 2; }
  else if(strcmp(color.c_str(), "YELLOW")==0)  { return 3; }
  else if(strcmp(color.c_str(), "BLUE")==0)    { return 4; }
  else if(strcmp(color.c_str(), "MAGENTA")==0) { return 5; }
  else if(strcmp(color.c_str(), "CYAN")==0)    { return 6; }
  else if(strcmp(color.c_str(), "WHITE")==0)   { return 7; }
  else { return 0; } //Invalid or BLACK
}

//Gets the type value for given item type
int type_value(std::string type)
{
       if(strcmp(type.c_str(), "WEAPON")==0)     { return 1; }
  else if(strcmp(type.c_str(), "OFFHAND")==0)   { return 2; }
  else if(strcmp(type.c_str(), "RANGED")==0)  { return 3; }
  else if(strcmp(type.c_str(), "ARMOR")==0)    { return 4; }
  else if(strcmp(type.c_str(), "HELMET")==0) { return 5; }
  else if(strcmp(type.c_str(), "CLOAK")==0)    { return 6; }
  else if(strcmp(type.c_str(), "GLOVES")==0)   { return 7; }
  else if(strcmp(type.c_str(), "BOOTS")==0)   { return 8; }
  else if(strcmp(type.c_str(), "RING")==0)   { return 9; }
  else if(strcmp(type.c_str(), "AMULET")==0)   { return 10; }
  else if(strcmp(type.c_str(), "LIGHT")==0)   { return 11; }
  else if(strcmp(type.c_str(), "SCROLL")==0)   { return 12; }
  else if(strcmp(type.c_str(), "BOOK")==0)   { return 13; }
  else if(strcmp(type.c_str(), "FLASK")==0)   { return 14; }
  else if(strcmp(type.c_str(), "GOLD")==0)   { return 15; }
  else if(strcmp(type.c_str(), "AMMUNITION")==0)   { return 16; }
  else if(strcmp(type.c_str(), "FOOD")==0)   { return 17; }
  else if(strcmp(type.c_str(), "WAND")==0)   { return 18; }
  else if(strcmp(type.c_str(), "CONTAINER")==0)   { return 19; }
  else if(strcmp(type.c_str(), "STACK")==0)   { return 20; }
  else { return 0; } //Invalid
}

/*************** (A) Superclass Character **************/
//Constructor
character_t :: character_t()
  {
    int k;
    for(k=0; k<NUM_ABILITIES; k++) { attributes[k]=0; }
    color=2;
    hitpoints=DEFAULT_HP;
    damage.set(DEFAULT_DAMAGE, 0, 0);
    speed=DEFAULT_SPEED;
    sight=DEFAULT_SIGHT;
    is_alive=1;
    pos[0]=0;
    pos[1]=0;
    turn=0;
    hn=NULL;
  }

//Destructor
character_t :: ~character_t(){}


//Sets all four attributes with given values
void character_t :: set_attributes(int erratic, int tunneler, int intelligence, int telepathy)
  {
    attributes[Erratic]=erratic;
    attributes[Tunneler]=tunneler;
    attributes[Intelligent]=intelligence;
    attributes[Telepathic]=telepathy;
  }

//Sets coordinates
void character_t :: set_pos(int x, int y)
  {
    pos[0]=x;
    pos[1]=y;
  }



/*************** (B) Subclass PC **************/
//Constructor
PC_t :: PC_t() : character_t()
  {
    type=CHAR_PC;
    name="You";
    description="This is you - a feeble, smelly little creature whose ugliness is at home\n with the most horrid and deformed of the monsters around you.\n If it weren't for all these items I happened to leave around for you,\n you probably wouldn't be reading this.\n\n\n You're welcome.";
    symbol='@';
    color=4;
    weapon=NULL;
    offhand=NULL;
    ranged=NULL;
    armor=NULL;
    helmet=NULL;
    cloak=NULL;
    gloves=NULL;
    boots=NULL;
    amulet=NULL;
    light=NULL;
    ring1=NULL;
    ring2=NULL;
  }

//Returns whether or not PC has given equipment type equipped
bool PC_t :: has_equipment(enum item_type type)
{
  switch(type)
    {
    case WEAPON:
      return weapon;
      break;
    case OFFHAND:
      return offhand;
      break;
    case RANGED:
      return ranged;
      break;
    case ARMOR:
      return armor;
      break;
    case HELMET:
      return helmet;
      break;
    case CLOAK:
      return cloak;
      break;
    case GLOVES:
      return gloves;
      break;
    case BOOTS:
      return boots;
      break;
    case RING:
      return ring1 && ring2;
      break;
    case AMULET:
      return amulet;
      break;
    case LIGHT:
      return light;
      break;
    default:
      return false;
      break;
    }
}

//Returns the slot 'number' for given item type
char PC_t :: get_slot(enum item_type type)
{
  switch(type)
    {
    case WEAPON:
      return 'a';
      break;
    case OFFHAND:
      return 'b';
      break;
    case RANGED:
      return 'c';
      break;
    case ARMOR:
      return 'd';
      break;
    case HELMET:
      return 'e';
      break;
    case CLOAK:
      return 'f';
      break;
    case GLOVES:
      return 'g';
      break;
    case BOOTS:
      return 'h';
      break;
    case RING:
      if(!ring2) { return 'k'; } 
      else { return 'l'; }
      break;
    case AMULET:
      return 'i';
      break;
    case LIGHT:
      return 'j';
      break;
    default:
      return 0;
      break;
    }
}

//Returns the slot 'number' for given item type
enum item_type PC_t :: get_slot_type(char slot)
{
  switch(slot)
    {
    case 'a':
      return WEAPON;
      break;
    case 'b':
      return OFFHAND;
      break;
    case 'c':
      return RANGED;
      break;
    case 'd':
      return ARMOR;
      break;
    case 'e':
      return HELMET;
      break;
    case 'f':
      return CLOAK;
      break;
    case 'g':
      return GLOVES;
      break;
    case 'h':
      return BOOTS;
      break;
    case 'k':
    case 'l':
      return RING;
      break;
    case 'i':
      return AMULET;
      break;
    case 'j':
      return LIGHT;
      break;
    default:
      return INVALID;
      break;
    }
}

void PC_t :: equip(object_t* item)
{
  switch(item->type)
    {
    case WEAPON:
      weapon=item;
      break;
    case OFFHAND:
      offhand=item;
      break;
    case RANGED:
      ranged=item;
      break;
    case ARMOR:
      armor=item;
      break;
    case HELMET:
      helmet=item;
      break;
    case CLOAK:
      cloak=item;
      break;
    case GLOVES:
      gloves=item;
      break;
    case BOOTS:
      boots=item;
      break;
    case RING:
      if(!ring1) { ring1=item; }
      else { ring2=item; }
      break;
    case AMULET:
      amulet=item;
      break;
    case LIGHT:
      sight+=item->attribute;
      light=item;
      break;
    default:
      break;
    }

  //Updates speed
  //if(item->speed>0) { speed += item->speed; } 
}

//Removes item from equipment, places back into inventory
//Updates stats
void PC_t :: dequip(char slot)
{
  switch(slot)
    {
    case 'a':
      dequip_update(weapon);
      inventory.push_back(weapon);
      weapon=NULL;
      break;
    case 'b':
      dequip_update(offhand); 
      inventory.push_back(offhand);
      offhand=NULL;
      break;
    case 'c':
      dequip_update(ranged);
      inventory.push_back(ranged);
      ranged=NULL;
      break;
    case 'd':
      dequip_update(armor); 
      inventory.push_back(armor);
      armor=NULL;
      break;
    case 'e':
      dequip_update(helmet); 
      inventory.push_back(helmet);
      helmet=NULL;
      break;
    case 'f':
      dequip_update(cloak);
      inventory.push_back(cloak);
      cloak=NULL;
      break;
    case 'g':
      dequip_update(gloves);
      inventory.push_back(gloves);
      gloves=NULL;
      break;
    case 'h':
      dequip_update(boots);
      inventory.push_back(boots);
      boots=NULL;
      break;
    case 'i':
      dequip_update(amulet);
      inventory.push_back(amulet);
      amulet=NULL;
      break;
    case 'j':
      sight-=light->attribute;
      dequip_update(light);
      inventory.push_back(light);
      light=NULL;
      break;
    case 'k':
      dequip_update(ring1);
      inventory.push_back(ring1);
      ring1=NULL;
      break;
    case 'l':
      dequip_update(ring2);
      inventory.push_back(ring2);
      ring1=NULL;
      break;
    default:
      break;
    }
}

//Updates modified stats
void PC_t :: dequip_update(object_t * item)
{
  //Updates speed
  speed-= item->speed; 
}

/*************** (C) Subclass Monster **************/
//Constructor
monster_t :: monster_t() : character_t()
  {
    type=CHAR_Monster;
    color=1;
    attributes[0] = rand()%2;
    attributes[1] = rand()%2;
    attributes[2] = rand()%2;
    attributes[3] = rand()%2;
    update_symbol();

    //Gets a speed value of one of 5, 10, or 20
    speed = 5;
    int j, k;
    int mult = 1;
    k=rand()%3+1;
    for(j=1; j<=k; j++)
      {
	speed=5*mult;
	mult*=2;
      }

    p_known[0]=-1;
    p_known[1]=-1;
  }

  //Returns integer boolean if PC location is known
  //Location is not known if both values are -1
int monster_t :: PC_known()
  {
    return (p_known[0]>=0 && p_known[1]>=0);
  }

  //Sets found PC coords
void monster_t :: PC_found(int x, int y)
  {
    p_known[0]=x;
    p_known[1]=y;
  }

  //Resets known PC coords
void monster_t :: PC_lost()
  {
    p_known[0]=-1;
    p_known[1]=-1;
  }

  //Updates symbol based on current attributes
void monster_t :: update_symbol()
  {
    //Translates binary type into decimal
    int mon_type = ( (attributes[0]*8)+(attributes[1]*4)+(attributes[2]*2)+(attributes[3]) );

    //Converts mon_type into hex
    if(mon_type<10)
      {
	symbol=mon_type + '0';
      }
    else
      {
	      switch(mon_type)
		{
		case 10:
		  symbol='a';
		  break;
		case 11:
		  symbol='b';
		  break;
		case 12:
		  symbol='c';
		  break;
		case 13:
		  symbol='d';
		  break;
		case 14:
		  symbol='e';
		  break;
		case 15:
		  symbol='f';
		  break;
		default:
		  symbol='Q';
		  break;
		}
      }
  }

/*************** (D) Class Object **************/
object_t :: object_t(){};
object_t :: ~object_t(){};


/*************** (AA) Class Monster Template **************/
//Prints out all the values in the monster template
void monster_template_t :: print_template()
{
  int i;
  std::cout << "Name: " << name << std::endl;
  std::cout << "Description: " << std::endl << description << std::endl;
  std::cout << "Symbol: " << symbol << std::endl;
  std::cout << "Color: " << color << std::endl;
  std::cout << "Speed: " << speed.base << "+" << speed.number << "d" << speed.sides << std::endl;
  std::cout << "Abilities:";
  for(i=0; i < (int) abilities.size(); i++)
    {
      std::cout << " " << abilities[i];
    }
  std::cout << std::endl;
  std::cout << "Hitpoints: " << hitpoints.base << "+" << hitpoints.number << "d" << hitpoints.sides << std::endl;
  std::cout << "Attack Damage: " << damage.base << "+" << damage.number << "d" << damage.sides << std::endl;
  std::cout << "Rarity: " << rarity << std::endl;
}

/*************** (AB) Monster Factory **************/
//Returns a monster with the templates data
monster_t * monster_template_t :: monster_factory()
{
  int k;
  monster_t *ret = new monster_t();
  ret->name = this->name;
  ret->description = this->description;
  ret->symbol = this->symbol;
  ret->color = this->color;

  //Sets abilities
  for(k=0; k < (int) abilities.size(); k++)
    {
      std::string cur = abilities[k];
      if( strcmp( cur.c_str(), "ERRATIC" ) == 0 ) { ret->attributes[0]=1; }
      else if( strcmp( cur.c_str(), "TUNNEL" ) == 0 )  { ret->attributes[1]=1; }
      else if( strcmp( cur.c_str(), "SMART" ) == 0 )   { ret->attributes[2]=1; }
      else if( strcmp( cur.c_str(), "TELE" ) == 0 )    { ret->attributes[3]=1; }
      else if( strcmp( cur.c_str(), "PASS" ) == 0 )    { ret->attributes[4]=1; }
      else if( strcmp( cur.c_str(), "PICKUP" ) == 0 )  { ret->attributes[5]=1; }
      else if( strcmp( cur.c_str(), "DESTROY" ) == 0 ) { ret->attributes[6]=1; }
      else if( strcmp( cur.c_str(), "UNIQ" ) == 0 )    { ret->attributes[7]=1; }
      else if( strcmp( cur.c_str(), "BOSS" ) == 0 )    { ret->attributes[8]=1; }
    }

  //Hitpoints
  ret->hitpoints = this->hitpoints.base;
  for(k=0; k<this->hitpoints.number; k++) { ret->hitpoints += rand()%this->hitpoints.sides + 1; }

  //Damage (Stays as dice)
  ret->damage.set(this->damage.base, this->damage.number, this->damage.sides);

  //Speed
  ret->speed = this->speed.base;
  for(k=0; k<this->speed.number; k++) { ret->speed += rand()%this->speed.sides + 1; }

  return ret;
}

/*************** (BA) Class Item Template **************/
//Prints out all the values in the item template
void object_template_t :: print_template()
{
  std::cout << "Name: " << name << std::endl;
  std::cout << "Description: " << std::endl << description << std::endl;
  std::cout << "Symbol: " << symbol << std::endl;
  std::cout << "Color: " << color << std::endl;
  std::cout << "Hit Bonus: " << hit.base << "+" << hit.number << "d" << hit.sides << std::endl;
  std::cout << "Damage Bonus: " << damage.base << "+" << damage.number << "d" << damage.sides << std::endl;
  std::cout << "Dodge Bonus: " << dodge.base << "+" << dodge.number << "d" << dodge.sides << std::endl;
  std::cout << "Defense Bonus: " << defense.base << "+" << defense.number << "d" << defense.sides << std::endl;
  std::cout << "Weight Bonus: " << weight.base << "+" << weight.number << "d" << weight.sides << std::endl;
  std::cout << "Speed Bonus: " << speed.base << "+" << speed.number << "d" << speed.sides << std::endl;
  std::cout << "Value: " << value.base << "+" << value.number << "d" << value.sides << std::endl;
  std::cout << "Attribute: " << attribute.base << "+" << attribute.number << "d" << attribute.sides << std::endl;
  std::cout << "Is Artifact: " << is_artifact << std::endl;
  std::cout << "Rarity: " << rarity << std::endl;
}

//Returns an object with the templates data
object_t * object_template_t :: object_factory()
{
  int k;
  object_t *ret = new object_t();
  ret->name = this->name;
  ret->description = this->description;
  ret->type = this->type;
  ret->is_equipment = this->is_equipment;
  ret->symbol = this->symbol;
  ret->color = this->color;

  //Hit
  ret->hit = this->hit.base;
  for(k=0; k<this->hit.number; k++) { ret->hit += rand()%this->hit.sides + 1; }

  //Damage (Stays as dice)
  ret->damage.set(this->damage.base, this->damage.number, this->damage.sides);

  //Dodge
  ret->dodge = this->dodge.base;
  for(k=0; k<this->dodge.number; k++) { ret->dodge += rand()%this->dodge.sides + 1; }

  //Defense
  ret->defense = this->defense.base;
  for(k=0; k<this->defense.number; k++) { ret->defense += rand()%this->defense.sides + 1; }

  //Weight
  ret->weight = this->weight.base;
  for(k=0; k<this->weight.number; k++) { ret->weight += rand()%this->weight.sides + 1; }

  //Speed
  ret->speed = this->speed.base;
  for(k=0; k<this->speed.number; k++) { ret->speed += rand()%this->speed.sides + 1; }

  //Attribute
  ret->attribute = this->attribute.base;
  for(k=0; k<this->attribute.number; k++) { ret->attribute += rand()%this->attribute.sides + 1; }

  //Value
  ret->value = this->value.base;
  for(k=0; k<this->value.number; k++) { ret->value += rand()%this->value.sides + 1; }

  ret->is_artifact = this->is_artifact;

  return ret;
}
