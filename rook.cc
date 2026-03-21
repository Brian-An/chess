#include "rook.h"
#include "info.h"
#include <vector>

using namespace std;

Rook::Rook(size_t row, size_t col, Colour colour): Piece(row, col, colour) {}

Info Rook::getInfo() const { return {row, col, colour, "r"}; }

// Function to return the locations between old and new location
vector<pair<int,int>> Rook::requestLoc(size_t newRow, size_t newCol) const {
    vector<pair<int,int>> locs;
    if (newRow == row) {
        // Moving along the same rank
        for (size_t col = min(this->col, newCol) + 1; col < max(this->col, newCol); col++) {
            locs.emplace_back(row, col);
        }
    } else if (newCol == col) {
        // Moving along the same file
        for (size_t row = min(this->row, newRow) + 1; row < max(this->row, newRow); row++) {
            locs.emplace_back(row, col);
        }
    }
    return locs;
}


std::vector<std::pair<int, int>> Rook::generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const {
    std::vector<std::pair<int, int>> res;
    for (int i = 0; i < 8; ++i) {
        res.emplace_back(std::pair{i, col}); //horizantal
        res.emplace_back(std::pair{row, i}); //verticall
    }

    return res;
}

