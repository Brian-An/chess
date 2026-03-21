#ifndef COMPUTERL4_H
#define COMPUTERL4_H
#include "player.h"

class ComputerL4: public Player {
public:
    // Decides the next move based on your level
    virtual MoveRequest move();
};

#endif
