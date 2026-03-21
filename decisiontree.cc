#include "decisiontree.h"


DecisionTree::DecisionTree(
    vector<vector<unique_ptr<Piece>>> *pieces,
    MoveRequest moveRequest,
    Colour colour): pieces{pieces}, moveRequest{moveRequest}, colour{colour}, captured{""} {
    const pair<int, int> prevLoc = translateToNum(moveRequest.prevLoc);
    const pair<int, int> newLoc = translateToNum(moveRequest.newLoc);

    if (newLoc != prevLoc) {
        // if it's capturing
        if ((*pieces)[newLoc.first][newLoc.second]) {
            captured = (*pieces)[newLoc.first][newLoc.second]->getInfo().type;
        }
    }
}

void DecisionTree::generateNextLevel(const int level) {
    if (level >= MAX_LEVEL) return ;

    // If it's not the first level - e.g. the board has already made the move for you =>
    //   => we will move the last piece so that the childrern know what moves are made. 
    const pair<int, int> prevLoc = translateToNum(moveRequest.prevLoc);
    const pair<int, int> newLoc = translateToNum(moveRequest.newLoc);
    unique_ptr<Piece> taken;
    unique_ptr<Piece> promotedPiece;
    bool enPassant = false;
    bool castling = false;
    if (newLoc != prevLoc) {
        taken = std::move((*pieces)[newLoc.first][newLoc.second]);

        // if it's en passant, then we get rid of the person that was removed
        if ((*pieces)[prevLoc.first][prevLoc.second]->getInfo().type == "p" && 
            newLoc.second != prevLoc.second &&
            !taken) {
            taken = std::move((*pieces)[prevLoc.first][newLoc.second]);
            enPassant = true;
        }

        (*pieces)[newLoc.first][newLoc.second] = std::move((*pieces)[prevLoc.first][prevLoc.second]);
        (*pieces)[newLoc.first][newLoc.second]->movePiece(newLoc.first, newLoc.second, false);

        // if it's casteling
        if ((*pieces)[newLoc.first][newLoc.second]->getInfo().type == "k" && 
            std::abs(newLoc.second - prevLoc.second) == 2) {
                castling = true;
                // if it's moving right, castle should move left
                // if it moves left, castle should move right
                bool kingSide = newLoc.second > prevLoc.second; // it's one if it's kingside - 0 if queenside
                int castlepos = kingSide ? SIZE - 1 : 0;
                int castledest = kingSide ? castlepos - 2 : castlepos + 3;
                (*pieces)[newLoc.first][castlepos]->movePiece(newLoc.first, castledest, false);
                (*pieces)[newLoc.first][castledest] = std::move((*pieces)[newLoc.first][castlepos]);
        }

        // promoting:
        if (moveRequest.promoting) {
            promotedPiece = std::move((*pieces)[newLoc.first][newLoc.second]);
            Colour promotingColour = promotedPiece->getInfo().colour;
            if (moveRequest.promoteTo == "r") {
                (*pieces)[newLoc.first][newLoc.second] = make_unique<Rook>(newLoc.first, newLoc.second, promotingColour);
            } else if (moveRequest.promoteTo == "b") {
                (*pieces)[newLoc.first][newLoc.second] = make_unique<Bishop>(newLoc.first, newLoc.second, promotingColour);
            } else if (moveRequest.promoteTo == "n") {
                (*pieces)[newLoc.first][newLoc.second] = make_unique<Knight>(newLoc.first, newLoc.second, promotingColour);
            } else if (moveRequest.promoteTo == "q") {
                (*pieces)[newLoc.first][newLoc.second] = make_unique<Queen>(newLoc.first, newLoc.second, promotingColour);
            }
        }

        // we make a dummy move where we don't actually move anything to check if the king is in danger
        MoveRequest mr{"a0", "a0", false, ""};
        DecisionTree dummy = DecisionTree{ pieces, mr, negateColour(colour)};
        dummy.generateNextLevel(MAX_LEVEL - 1);
        if (dummy.canTakeOtherKing) inCheck = true;

    }

    // if we have already passed this level we won't do it again
    if (generated) {
        for (size_t i = 0; i < children.size();) {
            children[i]->generateNextLevel(level+1);
            // If this move puts you in immidiate danger, then you can't perform this move
            if (children[i]->canTakeOtherKing) {
                children.erase(children.begin() + i);
            } else {
                ++i;
            }
        }
    } else {
        // now we know we haven't passed this level
        generated = true;

        for (int r = 0; r < SIZE; ++r) {
            for (int c=0; c < SIZE; ++c) {
                // Going through each piece with the same colour and generating the moves
                if ((*pieces)[r][c] && (*pieces)[r][c]->getInfo().colour == colour) {
                    for (auto moves : (*pieces)[r][c]->generateMoves(pieces, moveRequest)) {
                        
                        int newRow = moves.first, newCol = moves.second;
                        if (0 <= newRow && newRow < SIZE && 0 <= newCol && newCol < SIZE && // Don't Put outside the board
                             (!(*pieces)[newRow][newCol] || (*pieces)[newRow][newCol]->getInfo().colour != colour) // Don't put it on yourself
                            ) {
                                // make sure there is no person in between
                                bool pieceInBetween = false;
                                for (auto loc: (*pieces)[r][c]->requestLoc(newRow, newCol)) {
                                    if ((*pieces)[loc.first][loc.second]) {
                                        pieceInBetween = true;
                                        break;
                                    }
                                }
    
                                // If there is no piece in between we will create the next layer
                                if (!pieceInBetween) {
                                    // If it can take the other king
                                    if ((*pieces)[newRow][newCol] && (*pieces)[newRow][newCol]->getInfo().type == "k") { canTakeOtherKing = true; }
                                    // If it's the special case where were just cehcking for check there are no children
                                    if (newLoc == prevLoc && level != 0)  {
                                        if (canTakeOtherKing) return ;
                                        continue;
                                    }

                                    // if it's promoting: 
                                    if ((*pieces)[r][c]->getInfo().type == "p" && (newRow == 0 || newRow == SIZE-1)) {
                                        vector<string> options{"r","b","n","q"};
                                        for (auto option: options) {
                                            MoveRequest mr{translateToBoard(r,c), translateToBoard(newRow, newCol), true, option};
                                            children.emplace_back(make_unique<DecisionTree>(
                                                pieces, 
                                                mr, 
                                                negateColour(colour)
                                            ));
                                            children[children.size() - 1]->generateNextLevel(level + 1);
                                            // checking if were in check:
                                            // if there is a move that you do and then the oponent can win the game, then you are moving to a check and thats not allowed
                                            if (children[children.size()-1]->canTakeOtherKing) {
                                                children.pop_back(); // we remove it
                                            }
                                        }
                                    } else {
                                        MoveRequest mr{translateToBoard(r,c), translateToBoard(newRow, newCol), false, ""};
                                        children.emplace_back(make_unique<DecisionTree>(
                                            pieces, 
                                            mr, 
                                            negateColour(colour)
                                        ));
                                        children[children.size() - 1]->generateNextLevel(level + 1);
                                        // checking if were in check:
                                        // if there is a move that you do and then the oponent can win the game, then you are moving to a check and thats not allowed
                                        if (children[children.size()-1]->canTakeOtherKing) {
                                            children.pop_back(); // we remove it
                                        }
                                    }

                                }
                            }
                    }
                }
            }
        }
    }

    // Now it's time to move the pieces back
    if (newLoc != prevLoc) {
        // if promoting:
        if (moveRequest.promoting) {
            (*pieces)[newLoc.first][newLoc.second] = std::move(promotedPiece);
        }

        (*pieces)[newLoc.first][newLoc.second]->movePiece(prevLoc.first, prevLoc.second, false);
        (*pieces)[prevLoc.first][prevLoc.second] = std::move((*pieces)[newLoc.first][newLoc.second]);

        if (castling) {
            bool kingSide = newLoc.second > prevLoc.second; // it's one if it's kingside - 0 if queenside
            int castlepos = kingSide ? SIZE - 1 : 0;
            int castledest = kingSide ? castlepos - 2 : castlepos + 3;
            (*pieces)[newLoc.first][castledest]->movePiece(newLoc.first, castlepos, false);
            (*pieces)[newLoc.first][castlepos] = std::move((*pieces)[newLoc.first][castledest]);
        }
        
        // enpassant: 
        if (!enPassant) {
            (*pieces)[newLoc.first][newLoc.second] = std::move(taken);
        } else {
            (*pieces)[prevLoc.first][newLoc.second] = std::move(taken);
        }
    }
}

vector<unique_ptr<DecisionTree>> &DecisionTree::getChildren() { return children; }
const MoveRequest &DecisionTree::getMoveRequest() const { return moveRequest; }
bool DecisionTree::getInCheck() const { return inCheck; }
bool DecisionTree::getCanTakeOtherKing() const { return canTakeOtherKing; }
Colour DecisionTree::getColour() const { return colour; }
string DecisionTree::getCaptured() const { return captured; }
