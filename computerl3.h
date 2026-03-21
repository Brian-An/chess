#ifndef COMPUTERL3_H
#define COMPUTERL3_H
#include "player.h"
#include "util.h"
#include <string>
#include <utility>
#include <cmath>

using namespace std;

class ComputerL3: public Player {
public:
    // Decides the next move based on your level
    virtual MoveRequest move();
};

#endif
