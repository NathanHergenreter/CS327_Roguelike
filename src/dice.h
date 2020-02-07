#ifndef DICE_H
#define DICE_H

class dice {
 public:
  int base, number, sides;
  void set(int base, int number, int sides) { this->base=base; this->number=number; this->sides=sides; }
};

#endif
