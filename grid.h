#ifndef GRID_H
#define GRID_H

#include "piece.h"

#include "pawn.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "king.h"

#include "moverequest.h"
#include "textdisplay.h"
#include "graphicdisplay.h"
#include "decisiontree.h"
#include "openingExplorer.h"
#include "util.h"
#include "moverequest.h"
#include "state.h"
#include <vector>
#include <string>
#include <utility>
#include <memory>

using namespace std;

class DecisionTree;

class Grid {
    std::vector<std::vector<unique_ptr<Piece>>> pieces;
    std::vector<string> moveMap; // a field for taking care of infinite undos
    unique_ptr<TextDisplay> td;
    unique_ptr<DecisionTree> dt;
    unique_ptr<GraphicDisplay> gd;
    unique_ptr<OpeningExplorer> openingExplorer;
    MoveRequest lastMoveRequest;

    // private helper function to translate boardstate to string
    string transLateBoardState();


public:
    Grid();

    //Function to initialize the grid
    void init(bool setupMode = false);

    // Function to move a piece from prevLocation to newLocation
    //  Returns whether the move was successfull (allowed) or not
    bool move(MoveRequest &mr, Colour currentTurn);

    //The following are used in setup mode.

    // Function to add a piece to the location specified
    void add(char piece, string location);

    // Function to remove a piece from the location specified
    void remove(string location);

    // to undo - returns the status of the undo
    bool undo(const Colour currentTurn);
    // function to save the board satate
    void saveBoard();

    // The following are accessor functions

    //Function to check if the game is in check
    Colour isCheck() const;

    //Function to check if the game is in checkmate
    Colour isCheckmate() const;

    //Function to check if the game is in stalemate
    Colour isStalemate() const;

    //Function to check whether the setup (made in setup mode) is valid or not
    bool isSetupValid() const;
    
    // Function to update decision tree after setup
    void updateDecisionTree(Colour currentTurn);

    // Decision Tree getter
    DecisionTree *getDecisionTree() const;

    // Pieces getter
    Piece *getPiece(const string &location) const;

    // Opening Explorer getter
    OpeningExplorer *getOpeningExplorer() const;

    // Check if the grid has insufficient material to continue the game
    bool isInsufficientMaterial() const;

    friend ostream &operator<<(ostream &out, Grid &g);
};

ostream &operator<<(ostream &out, Grid &g);

#endif
