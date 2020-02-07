#ifndef CHARACTER_H

#define CHARACTER_H

#include <string>
#include <vector>

#include "heap.h"
#include "dice.h"

#define NUM_ABILITIES 9
#define DEFAULT_SPEED 10
#define DEFAULT_SIGHT 3
#define DEFAULT_HP 500
#define DEFAULT_DAMAGE 5
#define MAX_INVENTORY 10

//Character Types - Each type has a corresponding char which overwrites tile type
enum CHAR_type{CHAR_PC, CHAR_Monster};

enum item_type{INVALID, WEAPON, OFFHAND, RANGED, ARMOR, HELMET, CLOAK, GLOVES, BOOTS, RING, AMULET, LIGHT, SCROLL, BOOK, FLASK, GOLD, AMMUNITION, FOOD, WAND, CONTAINER };


/******** Generic Functions **************/
int32_t turn_cmp(const void *, const void *);
int color_value(std::string);
int type_value(std::string);

class object_t;

/********* Class: Character Base *********/
class character_t
{
  //type: Character Type (Needed?)
  //symbol: Symbol representing character in output
  //attributes: (0) Erratic, (1) Tunneler, (2) Intelligent, (3) Telepathic, (4) Non-Corporeal, (5) Pickup, (6) Destroyer, (7) Unique, (8) Boss
  //speed: Determines how often character gets a turn (current turn cnt + 1000/speed)
  //sight: Determines how far character can see
  //pos: (x,y) coords on dungeon
  //is_alive: Flag for if living or not
  //*hn: Used for heap in turn determination
public:
  std::string name;
  std::string description;
  enum CHAR_type type;
  char symbol;
  int color;
  int attributes[NUM_ABILITIES], sight, is_alive, pos[2], turn;
  heap_node_t *hn;
  int hitpoints;
  dice damage;
  int speed;
  std::vector<object_t *> inventory;

  //Constructor
  character_t();

  //Destructor
  ~character_t();

  //Basic functions
  void set_attributes(int, int, int, int);
  void set_pos(int, int);

};

/********* Class: Character Player (PC_t) *******/
class PC_t: public character_t
{
public:
  //Equipment - adds to stats when put on, bonuses removed when removed
  object_t *weapon; //a
  object_t *offhand; //b
  object_t *ranged; //c
  object_t *armor; //d
  object_t *helmet; //e
  object_t *cloak; //f
  object_t *gloves; //g
  object_t *boots; //h
  object_t *amulet; //i
  object_t *light; //j 
  object_t *ring1; //k
  object_t *ring2; //l

  PC_t();
  bool has_equipment(enum item_type);
  char get_slot(enum item_type);
  enum item_type get_slot_type(char);
  void equip(object_t *);
  void dequip(char slot);
  void dequip_update(object_t *);
};

/******** Class: Character Monster (monster_t) *********/
class monster_t: public character_t
{
public:
  //p_known: Contains last known coordinates of PC ( (-1,-1) if unknown)
  int  p_known[2];

  monster_t();
  int PC_known();
  void PC_found(int, int);
  void PC_lost();
  void update_symbol();
};

/******** Class: Template Monster (monster_template_t) *********/
class monster_template_t
{
 public:
  //Contains all information for monster generation using template
  std::string name;
  std::string description;
  char symbol;
  int color;
  bool is_available;
  std::vector<std::string> abilities;
  //int attributes[NUM_ABILITIES];
  dice hitpoints;
  dice damage;
  dice speed;
  int rarity;

  void print_template();
  monster_t * monster_factory();
};

/******** Class: Object (object_t) *********/
class object_t
{
 public:
  std::string name;
  std::string description;
  enum item_type type;
  int is_equipment;
  char symbol;
  int color;
  int hit;
  dice damage;
  int dodge;
  int defense;
  int weight;
  int speed;
  int attribute;
  int value;
  bool is_artifact;
  int rarity;

  object_t();
  ~object_t();
};

/******** Class: Template Object (object_template_t) *********/
class object_template_t
{
 public:
  //Contains all information for item generation using template
  std::string name;
  std::string description;
  enum item_type type;
  int is_equipment;
  char symbol;
  int color;
  dice hit;
  dice damage;
  dice dodge;
  dice defense;
  dice weight;
  dice speed;
  dice attribute;
  dice value;
  bool is_artifact;
  int rarity;

  void print_template();
  object_t * object_factory();
};

#endif
