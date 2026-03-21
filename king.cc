#include "king.h"

King::King(size_t row, size_t col, Colour colour): Piece(row, col, colour) {}

Info King::getInfo() const { return {row, col, colour, "k"}; }

std::vector<std::pair<int,int>> King::requestLoc(size_t newRow, size_t newCol) const {
    return std::vector<std::pair<int,int>>{};
}

std::vector<std::pair<int, int>> King::generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const {
    std::vector<std::pair<int, int>> res;
    for (int r =-1; r <= 1; ++r) {
        for (int c = -1; c <= 1; ++c) {
            res.emplace_back(std::pair{row+r, col+c});
        }
    }
    // Castling logic
    if (!hasMoved) {
        // Kingside castling
        if (col == 4 && 
            (*pieces)[row][col + 3] &&
            (*pieces)[row][col + 3]->getInfo().type == "r" &&
            (*pieces)[row][col + 3]->getInfo().colour == colour &&
            !(*pieces)[row][col + 3]->getHasMoved() &&
            !(*pieces)[row][col + 1] &&
            !(*pieces)[row][col + 2]) {
                res.emplace_back(row, col + 2);
        }
        
            // Queenside castling
        if (col == 4 &&
            (*pieces)[row][col - 4] &&
            (*pieces)[row][col - 4]->getInfo().type == "r" &&
            (*pieces)[row][col - 4]->getInfo().colour == colour &&
            !(*pieces)[row][col - 4]->getHasMoved() &&
            !(*pieces)[row][col - 1] &&
            !(*pieces)[row][col - 2] &&
            !(*pieces)[row][col - 3]) {
                res.emplace_back(row, col - 2);
        }
    }
    return res;
}
