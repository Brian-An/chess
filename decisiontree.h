#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "piece.h"

#include "pawn.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"
#include "knight.h"

#include "moverequest.h"
#include "util.h"

#include <iostream>
#include <memory>

using namespace std;
const int MAX_LEVEL = 3;

// Lists possible moves and responses as a tree
class DecisionTree {
    vector<vector<unique_ptr<Piece>>> *pieces;
    MoveRequest moveRequest;
    Colour colour;
    bool canTakeOtherKing = false;
    bool inCheck = false;
    string captured; // "" - "p" "q"
    vector<unique_ptr<DecisionTree>> children;
    bool generated = false;

    public:
        DecisionTree(vector<vector<unique_ptr<Piece>>> *pieces, MoveRequest moveRequest, Colour colour);
        void generateNextLevel(const int level = 0);

        //Accessor function
        vector<unique_ptr<DecisionTree>> &getChildren();
        const MoveRequest &getMoveRequest() const;
        bool getInCheck() const;
        bool getCanTakeOtherKing() const;
        Colour getColour() const;
        string getCaptured() const;

    friend ostream &operator<<(ostream &out, DecisionTree &dt);
};

#endif
