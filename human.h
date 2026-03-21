#ifndef HUMAN_H
#define HUMAN_H
#include "player.h"

class Human: public Player {
public:
    // This just reads input from the std input in this case
    virtual MoveRequest move();
};

#endif
