#ifndef COMPUTERL2_H
#define COMPUTERL2_H
#include "player.h"

class ComputerL2: public Player {
public:
    // Decides the next move based on your level
    virtual MoveRequest move();
};

#endif
