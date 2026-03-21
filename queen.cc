#include "queen.h"

Queen::Queen(size_t row, size_t col, Colour colour): Piece(row, col, colour) {}

Info Queen::getInfo() const { return {row, col, colour, "q"}; }

std::vector<std::pair<int,int>> Queen::requestLoc(size_t newRow, size_t newCol) const {
    std::vector<std::pair<int,int>> res;
    if (newCol == col) { // Same column
        int start = (newRow < row) ? newRow : row;
        int end = (newRow < row) ? row : newRow;
        for (int i = start + 1; i < end; ++i) {
            res.emplace_back(std::pair<int,int>(i, newCol));
        }
    }
    else if (newRow == row) { // Same row
        int start = (newCol < col) ? newCol : col;
        int end = (newCol < col) ? col : newCol;
        for (int i = start + 1; i < end; ++i) {
            res.emplace_back(std::pair<int,int>(newRow, i));
        }
    }
    else if (newRow + newCol == row + col) { // Same upwards diagonal
        int start = (newRow < row) ? newRow : row;
        int end = (newRow < row) ? row : newRow;
        int sum = newRow + newCol; 
        for (int i = start + 1; i < end; ++i) {
            // Each square in between must have coordinates summed up to the same number
            res.emplace_back(std::pair<int,int>(i, sum - i));
        }
    }
    else { // Same downwards diagonal
        int start = (newCol < col) ? newCol : col;
        int end = (newCol < col) ? col : newCol;
        int diff = newRow - newCol;
        for (int i = start + 1; i < end; ++i) {
            // Each square in between must have coordinates with the same difference
            res.emplace_back(std::pair<int,int>(diff + i, i));
        }
    }
    return res;
}



std::vector<std::pair<int, int>> Queen::generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const {
    std::vector<std::pair<int, int>> res;
    // the rook move
    for (int i = 0; i < 8; ++i) {
        res.emplace_back(std::pair{i, col}); //horizantal
        res.emplace_back(std::pair{row, i}); //verticall
    }

    // the bishop move
    for (int r = 0; r < 8; ++r) {
        res.emplace_back(std::pair{r,col - (row - r)}); // line y = x
        res.emplace_back(std::pair{r,col + (row - r)}); // line y = -x
    }

    return res;
}

