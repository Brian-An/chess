#include "computerl1.h"

#include <iostream>
using namespace std;

MoveRequest ComputerL1::move() {
    const vector<unique_ptr<DecisionTree>> &availableMoves = Player::theGrid->getDecisionTree()->getChildren();
    return availableMoves[randNat(availableMoves.size())]->getMoveRequest();
}
