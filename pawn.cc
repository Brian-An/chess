#include "pawn.h"
#include "info.h"
#include <vector>
#include <utility>
#include <memory>

using namespace std;

Pawn::Pawn(size_t row, size_t col, Colour colour): Piece(row, col, colour) {}

Info Pawn::getInfo() const { return {row, col, colour, "p"}; }

// Function to return the locations between old and new location
vector<pair<int,int>> Pawn::requestLoc(size_t newRow, size_t newCol) const {
    vector<pair<int,int>> locs;
    if (colour == Colour::White) {
        if (row == 1 && newRow == row + 2 && newCol == col) {
            locs.emplace_back(row + 1, col); // Return the intermediate square (row + 1)
        }
    } else { // Colour::Black
        if (row == 6 && newRow == row - 2 && newCol == col) {
            locs.emplace_back(row - 1, col); // Return the intermediate square (row - 1)
        }
    }
    return locs;
}


std::vector<std::pair<int, int>> Pawn::generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const {
    std::vector<std::pair<int, int>> res;
    if (colour == Colour::White) {
        // Check forward move
        if (row + 1 < 8 && !(*pieces)[row + 1][col]) {
            res.emplace_back(row + 1, col);
        }
        // Check double forward move from starting position
        if (row == 1 && !(*pieces)[row + 1][col] && !(*pieces)[row + 2][col]) {
            res.emplace_back(row + 2, col);
        }
        // Check diagonal attacks
        if (col > 0 && row + 1 < 8 && (*pieces)[row + 1][col - 1] && (*pieces)[row + 1][col - 1]->getInfo().colour == Colour::Black) {
            res.emplace_back(row + 1, col - 1);
        }
        if (col + 1 < 8 && row + 1 < 8 && (*pieces)[row + 1][col + 1] && (*pieces)[row + 1][col + 1]->getInfo().colour == Colour::Black) {
            res.emplace_back(row + 1, col + 1);
        }
        
        // Check en passant captures
        if (row == 4) {
            // Check if last move was a black pawn moving two squares
            if (!lastMove.prevLoc.empty() && !lastMove.newLoc.empty()) {
                size_t lastPrevRow = lastMove.prevLoc[1] - '1';
                size_t lastNewRow = lastMove.newLoc[1] - '1';
                size_t lastNewCol = lastMove.newLoc[0] - 'a';
                
                // Check if it was a black pawn that moved two squares to our row
                if (lastPrevRow == 6 && lastNewRow == 4 && 
                    (*pieces)[lastNewRow][lastNewCol] && 
                    (*pieces)[lastNewRow][lastNewCol]->getInfo().type == "p" &&
                    (*pieces)[lastNewRow][lastNewCol]->getInfo().colour == Colour::Black) {
                    
                    // Check if the pawn is adjacent to us
                    if ((lastNewCol == col - 1 && col > 0) || (lastNewCol == col + 1 && col + 1 < 8)) {
                        res.emplace_back(row + 1, lastNewCol);
                    }
                }
            }
        }
    } else { // Colour::Black
        // Check forward move
        if (row > 0 && !(*pieces)[row - 1][col]) {
            res.emplace_back(row - 1, col);
        }
        // Check double forward move from starting position
        if (row == 6 && !(*pieces)[row - 1][col] && !(*pieces)[row - 2][col]) {
            res.emplace_back(row - 2, col);
        }
        // Check diagonal attacks
        if (col > 0 && row > 0 && (*pieces)[row - 1][col - 1] && (*pieces)[row - 1][col - 1]->getInfo().colour == Colour::White) {
            res.emplace_back(row - 1, col - 1);
        }
        if (col + 1 < 8 && row > 0 && (*pieces)[row - 1][col + 1] && (*pieces)[row - 1][col + 1]->getInfo().colour == Colour::White) {
            res.emplace_back(row - 1, col + 1);
        }
        
        // Check en passant captures
        if (row == 3) {
            // Check if last move was a white pawn moving two squares
            if (!lastMove.prevLoc.empty() && !lastMove.newLoc.empty()) {
                size_t lastPrevRow = lastMove.prevLoc[1] - '1';
                size_t lastNewRow = lastMove.newLoc[1] - '1';
                size_t lastNewCol = lastMove.newLoc[0] - 'a';
                
                // Check if it was a white pawn that moved two squares to our row
                if (lastPrevRow == 1 && lastNewRow == 3 && 
                    (*pieces)[lastNewRow][lastNewCol] && 
                    (*pieces)[lastNewRow][lastNewCol]->getInfo().type == "p" &&
                    (*pieces)[lastNewRow][lastNewCol]->getInfo().colour == Colour::White) {
                    
                    // Check if the pawn is adjacent to us
                    if ((lastNewCol == col - 1 && col > 0) || (lastNewCol == col + 1 && col + 1 < 8)) {
                        res.emplace_back(row - 1, lastNewCol);
                    }
                }
            }
        }
    }
    return res;
}
