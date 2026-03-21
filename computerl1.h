#ifndef COMPUTERL1_H
#define COMPUTERL1_H
#include "player.h"

class ComputerL1: public Player {
public:
    // Decides the next move based on your level
    virtual MoveRequest move();
};

#endif
