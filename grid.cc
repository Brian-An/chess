#include "grid.h"

Grid::Grid(): pieces(8) {
    for (int i = 0; i < SIZE; ++i) { pieces[i].resize(8); }
}

void Grid::init(bool setupMode) {
    moveMap.clear();
    //Resetting the pieces first
    for (int i = 0; i < SIZE; ++i) { 
        for (int j = 0; j < SIZE; ++j) {
            pieces[i][j] = unique_ptr<Piece>{nullptr};
        }
    }
    //Reseting the display
    td = make_unique<TextDisplay>();
    gd = make_unique<GraphicDisplay>();
    openingExplorer = make_unique<OpeningExplorer>();

    if (!setupMode) {
        // Adding Rooks:
        pieces[0][0] = make_unique<Rook>(0,0,Colour::White);
        pieces[0][SIZE-1] = make_unique<Rook>(0,SIZE-1,Colour::White);

        pieces[SIZE-1][0] = make_unique<Rook>(SIZE-1,0,Colour::Black);
        pieces[SIZE-1][SIZE-1] = make_unique<Rook>(SIZE-1,SIZE-1,Colour::Black);

        // Adding Knights:
        pieces[0][1] = make_unique<Knight>(0,1,Colour::White);
        pieces[0][SIZE-2] = make_unique<Knight>(0,SIZE-2,Colour::White);

        pieces[SIZE-1][1] = make_unique<Knight>(SIZE-1,1,Colour::Black);
        pieces[SIZE-1][SIZE-2] = make_unique<Knight>(SIZE-1,SIZE-2,Colour::Black);

        // Bishops:
        pieces[0][2] = make_unique<Bishop>(0,2,Colour::White);
        pieces[0][SIZE-3] = make_unique<Bishop>(0,SIZE-3,Colour::White);

        pieces[SIZE-1][2] = make_unique<Bishop>(SIZE-1,2,Colour::Black);
        pieces[SIZE-1][SIZE-3] = make_unique<Bishop>(SIZE-1,SIZE-3,Colour::Black);

        // Queen:
        pieces[0][3] = make_unique<Queen>(0,3,Colour::White);
        pieces[SIZE-1][3] = make_unique<Queen>(SIZE-1,3,Colour::Black);

        pieces[0][4] = make_unique<King>(0,4,Colour::White);
        pieces[SIZE-1][4] = make_unique<King>(SIZE-1,4,Colour::Black);

        // Pawns:
        for (int i = 0; i < SIZE; ++i) {
            pieces[1][i] = make_unique<Pawn>(1,i,Colour::White);
            pieces[SIZE-2][i] = make_unique<Pawn>(SIZE-2,i,Colour::Black);
        }
        
        // Add the observers to text display!
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                if (pieces[r][c]) {
                    pieces[r][c]->attach(td.get());
                    pieces[r][c]->attach(gd.get());
                    pieces[r][c]->attach(openingExplorer.get());
                    pieces[r][c]->notifyObservers();
                }
            }
        }
    }

    lastMoveRequest = {"a0", "a0", false, ""};
    dt = make_unique<DecisionTree>(&pieces, lastMoveRequest, Colour::White);
    dt->generateNextLevel();
}

// helper function to check if each location is on the board
bool onBoard(string loc) {
    return loc.length() == 2 && loc[0] <= 'h' && loc[0] >= 'a' && loc[1] >= '1' && loc[1] <= '8';
}

bool Grid::move(MoveRequest &mr, Colour currentTurn) {
    size_t i = 0;
    // Finidng if it's a move
    for (; i < dt->getChildren().size(); ++i) {
        if (mr == dt->getChildren()[i]->getMoveRequest()) break;
    }
    if (i != dt->getChildren().size()) {

        // Moving to the next level
        unique_ptr<DecisionTree> child = std::move(dt->getChildren()[i]);
        dt = std::move(child);

        // generating the next level
        dt->generateNextLevel();

        // Actually moving the piece and taking the piece
        pair<int,int> prevLoc = translateToNum(mr.prevLoc);
        int prevRow = prevLoc.first, prevCol = prevLoc.second;
        pair<int,int> newLoc = translateToNum(mr.newLoc);
        int newRow = newLoc.first, newCol = newLoc.second;
        
        // if it's en passant, then we get rid of the person that was removed
        if (pieces[prevRow][prevCol]->getInfo().type == "p" &&  // we are a pawn
            newCol != prevCol &&  // we are moving diagonally
            !pieces[newRow][newCol] // We weren't attacking anyone
        ) {
            pieces[prevRow][newCol]->removePiece();
            pieces[prevRow][newCol] = nullptr;
        }

        
        // if a piece is being taken, we will remove it first
        if (pieces[newRow][newCol].get() != nullptr){
            pieces[newRow][newCol]->removePiece();
        }
        
        
        // then we move the piece
        pieces[newRow][newCol] = std::move(pieces[prevRow][prevCol]);
        pieces[newRow][newCol]->movePiece(newRow, newCol);
        
        // If it's casteling - it's a king moving 2 horizantly
        if (pieces[newRow][newCol]->getInfo().type == "k" && std::abs(newCol - prevCol) == 2) {
            bool kingSide = newCol > prevCol;
            int castlepos = kingSide ? SIZE - 1 : 0;
            int castledest = kingSide ? castlepos - 2 : castlepos + 3;
            pieces[newRow][castledest] = std::move(pieces[newRow][castlepos]);
            pieces[newRow][castledest]->movePiece(newRow, castledest);
            // move the castle to the correct  place:
            
        }

        // If it's promoting
        if (mr.promoting) {
            // we first remove the piece, then we add the new piece
            pieces[newRow][newCol]->removePiece();
            if (mr.promoteTo == "r") {
                pieces[newRow][newCol] = make_unique<Rook>(newRow, newCol, currentTurn);
            } else if (mr.promoteTo == "b") {
                pieces[newRow][newCol] = make_unique<Bishop>(newRow, newCol, currentTurn);
            } else if (mr.promoteTo == "n") {
                pieces[newRow][newCol] = make_unique<Knight>(newRow, newCol, currentTurn);
            } else if (mr.promoteTo == "q") {
                pieces[newRow][newCol] = make_unique<Queen>(newRow, newCol, currentTurn);
            } else {
                return false; // Invalid piece type
            }
            // added the observers
            pieces[newRow][newCol]->attach(td.get());
            pieces[newRow][newCol]->attach(gd.get());
            pieces[newRow][newCol]->attach(openingExplorer.get());
            pieces[newRow][newCol]->notifyObservers();
        }

        moveMap.emplace_back(transLateBoardState()); // adding position to moveMap
        return true;
    }
    return false;
}

void Grid::add(char piece, string location) {
    if (!onBoard(location)) {
        return; // Invalid location
    }

    // Convert location to row and column index
    size_t row = location[1] - '1';
    size_t col = location[0] - 'a';

    // Check if the location is already occupied
    if (pieces[row][col]) {
        pieces[row][col]->removePiece();
    }

    // Determine color based on case (uppercase = white, lowercase = black)
    Colour colour = (piece >= 'A' && piece <= 'Z') ? Colour::White : Colour::Black;
    
    // Convert to lowercase for piece type checking
    char pieceType = tolower(piece);
    
    // Create the appropriate piece based on type
    if (pieceType == 'p') {
        pieces[row][col] = make_unique<Pawn>(row, col, colour);
    } else if (pieceType == 'r') {
        pieces[row][col] = make_unique<Rook>(row, col, colour);
    } else if (pieceType == 'b') {
        pieces[row][col] = make_unique<Bishop>(row, col, colour);
    } else if (pieceType == 'n') {
        pieces[row][col] = make_unique<Knight>(row, col, colour);
    } else if (pieceType == 'q') {
        pieces[row][col] = make_unique<Queen>(row, col, colour);
    } else if (pieceType == 'k') {
        pieces[row][col] = make_unique<King>(row, col, colour);
    } else {
        return; // Invalid piece type
    }
    
    // Attach observers and notify about the new piece
    if (td && gd) {
        pieces[row][col]->attach(td.get());
        pieces[row][col]->attach(gd.get());
        pieces[row][col]->attach(openingExplorer.get());
        pieces[row][col]->notifyObservers();
    }
}

void Grid::remove(string location) {
    if (!onBoard(location)) {
        return; // Invalid location
    }

    // Convert location to row and column index
    size_t row = location[1] - '1';
    size_t col = location[0] - 'a';

    // Check if the location is occupied
    if (!pieces[row][col]) {
        return; // No piece to remove
    }

    pieces[row][col]->removePiece(); // Notify observers about the removal
    pieces[row][col].reset(); // Remove the piece
}


bool Grid::isSetupValid() const {
    // Check for exactly one white king and one black king
    int whiteKingCount = 0;
    int blackKingCount = 0;
    for (const auto &row : pieces) {
        for (const auto &piece : row) {
            if (piece) {
                if (piece->getInfo().type == "k") {
                    if (piece->getInfo().colour == Colour::White) {
                        whiteKingCount++;
                    } else {
                        blackKingCount++;
                    }
                }
            }
        }
    }

    if (whiteKingCount != 1 || blackKingCount != 1) {
        return false;
    }

    // Check that no pawns are on the first or last row
    for (size_t col = 0; col < SIZE; ++col) {
        if (pieces[0][col] && pieces[0][col]->getInfo().type == "p") return false;
        if (pieces[SIZE-1][col] && pieces[SIZE-1][col]->getInfo().type == "p") return false;
    }
    // Check that neither king is in check
    return isCheck() == Colour::Neither;
}


Colour Grid::isCheck() const {
    // Check if the current player is in check
    // This function should check if the current player's king is under attack
    if (dt->getInCheck()) return dt->getColour();
    if (dt->getCanTakeOtherKing()) return negateColour(dt->getColour());
    return Colour::Neither;
}

Colour Grid::isCheckmate() const {
    Colour checkStatus = isCheck();
    if (checkStatus == Colour::Neither) {
        return Colour::Neither; // No one is in check, so no checkmate
    }
    
    // If a player is in check and has no legal moves, it's checkmate
    if (dt->getChildren().empty()) {
        return checkStatus;
    }
    
    return Colour::Neither; // Player is in check but has legal moves
}

Colour Grid::isStalemate() const {
    Colour checkStatus = isCheck();
    if (checkStatus != Colour::Neither) {
        return Colour::Neither; // If in check, it's not stalemate
    }
    // Check if there are no legal moves for the current player
    if (dt->getChildren().empty()) {
        // No legal moves available, it's stalemate
        return dt->getColour();
    }
    // If there are legal moves, it's not stalemate
    return Colour::Neither;
}

ostream &operator<<(ostream &out, Grid &g) {
    return out << *g.td;
}

// Function to update decision tree after setup
void Grid::updateDecisionTree(Colour currentTurn) {
    lastMoveRequest = {"a0", "a0", false, ""};
    dt = make_unique<DecisionTree>(&pieces, lastMoveRequest, currentTurn);
    dt->generateNextLevel();
}

DecisionTree *Grid::getDecisionTree() const {
    return dt.get();
}

OpeningExplorer *Grid::getOpeningExplorer() const {
    return openingExplorer.get();
}

bool Grid::isInsufficientMaterial() const {
    int whiteBishops = 0;
    int blackBishops = 0;

    int whiteKnights = 0;
    int blackKnights = 0;

    int otherPieces = 0;

    for (const auto &row : pieces) {
        for (const auto &piece : row) {
            if (piece && piece->getInfo().type != "k") {
                if (piece->getInfo().type == "b") {
                    piece->getInfo().colour == Colour::White ? whiteBishops++ : blackBishops++;
                } else if (piece->getInfo().type == "n") {
                    piece->getInfo().colour == Colour::White ? whiteKnights++ : blackKnights++;
                } else {
                    otherPieces++; // Pawns, rooks, queens
                }
            }
        }
    }

    // If there are pawns, rooks, or queens, there's sufficient material
    if (otherPieces > 0) return false;

    int totalBishops = whiteBishops + blackBishops;
    int totalKnights = whiteKnights + blackKnights;
    int totalMinorPieces = totalBishops + totalKnights;

    // King vs King
    if (totalMinorPieces == 0) return true;
    
    // King + minor piece vs King
    if (totalMinorPieces == 1) return true;
    
    // King + Knight vs King + Knight
    if (totalMinorPieces == 2 && whiteKnights == 1 && blackKnights == 1) return true;
    
    // King + two knights vs King (cannot checkmate)
    if (totalMinorPieces == 2 && (whiteKnights == 2 || blackKnights == 2)) return true;

    return false;
}


Piece *Grid::getPiece(const string &location) const {
    pair<int,int> loc = translateToNum(location);
    return onBoard(location) ? pieces[loc.first][loc.second].get() : nullptr;
}

string Grid::transLateBoardState() {
    string res = "";
    for (const auto &row : pieces) {
        for (const auto &cell : row) {
            if (cell) {
                res += cell->getInfo().colour == Colour::White ? toUpper((cell->getInfo().type)[0]) : (cell->getInfo().type)[0];
            } else {
                res+="-";
            }
        }
    }
    return res;
}

bool Grid::undo(const Colour currentTurn) {
    if (moveMap.size() <= 1) return false;
    // getting boards state and prevState
    const string boardState = moveMap.back();
    const string prevBoardState = moveMap.at(moveMap.size() - 2);
    // putting the pieces
    for (int r = 0; r < SIZE; ++r) {
        for (int c =0; c < SIZE; ++c) {
            // if there is a difference we adress the difference:
            if (boardState[r* SIZE + c] != prevBoardState[r * SIZE + c]) {
                if (prevBoardState[r * SIZE + c] != '-') {
                    if (pieces[r][c]) {
                        // if there is a piece here we remove it first
                        pieces[r][c]->removePiece();
                        pieces[r][c] = nullptr;
                    }
                    // either a piece moved to here - or a piece changed how it is: 
                    char letter = prevBoardState[r* SIZE + c];
                    Colour colour = 'A' <= letter && letter <= 'Z' ? Colour::White : Colour::Black;
                    letter = toLower(letter);
                    if (letter == 'p') {
                        pieces[r][c] = make_unique<Pawn>(r,c, colour);
                    } else if (letter == 'r') {
                        pieces[r][c] = make_unique<Rook>(r,c,colour);
                    } else if (letter == 'n') {
                        pieces[r][c] = make_unique<Knight>(r,c,colour);
                    } else if (letter == 'b') {
                        pieces[r][c] = make_unique<Bishop>(r,c,colour);
                    } else if (letter == 'q') {
                        pieces[r][c] = make_unique<Queen>(r,c,colour);
                    } else {
                        pieces[r][c] = make_unique<King>(r,c,colour);
                    }
                    pieces[r][c]->attach(td.get());
                    pieces[r][c]->attach(gd.get());
                    pieces[r][c]->notifyObservers();
                } else {
                    // a piece was removed:
                    pieces[r][c]->removePiece();
                    pieces[r][c] = nullptr;
                }
            }
        }
    }
    
    // removing the last piece
    moveMap.pop_back();
    updateDecisionTree(negateColour(currentTurn));
    return true;
}

void Grid::saveBoard() { moveMap.emplace_back(transLateBoardState()); }
