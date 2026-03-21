#include "knight.h"

Knight::Knight(size_t row, size_t col, Colour colour): Piece(row, col, colour) {}

Info Knight::getInfo() const { return {row, col, colour, "n"}; }

std::vector<std::pair<int,int>> Knight::requestLoc(size_t newRow, size_t newCol) const {
    return std::vector<std::pair<int,int>>();
}


std::vector<std::pair<int, int>> Knight::generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const {
    std::vector<std::pair<int, int>> res;
    for (int i = -2; i < 3; i+=4) {
        res.emplace_back(std::pair{row+i, col+1});
        res.emplace_back(std::pair{row+i, col-1});
        res.emplace_back(std::pair{row+1, col+i});
        res.emplace_back(std::pair{row-1, col-i});
    }
    return res;
}

