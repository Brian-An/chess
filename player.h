#ifndef PLAYER_H
#define PLAYER_H
#include "grid.h"
#include "state.h"
#include "moverequest.h"

class Player {
protected:
    inline static Grid *theGrid;
public:
    // This function is used for each child class to decide where to go.
    virtual MoveRequest move() = 0;

    // Grid setter
    static void setGrid(Grid *g);
};

#endif 
