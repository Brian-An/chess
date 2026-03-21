#include "bishop.h"
#include <cmath>

Bishop::Bishop(size_t row, size_t col, Colour colour): Piece(row, col, colour) {}

Info Bishop::getInfo() const { return {row, col, colour, "b"}; }

std::vector<std::pair<int,int>> Bishop::requestLoc(size_t newRow, size_t newCol) const {
    int which = (row - newRow)*(col - newCol);
    std::vector<std::pair<int, int>> res;
    if (which > 0) {
        // if it's on the y = x diagonal
        size_t minrow = std::min(row, newRow);
        size_t mincol = std::min(col, newCol);
        size_t maxrow = std::max(row, newRow);
        // adding elements to the end
        for (size_t i = 1; i < maxrow - minrow; ++i) {
            res.emplace_back(std::pair(minrow+i, mincol+i));
        }
        return res;
    }
    // if it's on the y = -x diagonal
    size_t minrow = std::min(row, newRow);
    size_t maxcol = std::max(col, newCol);
    size_t maxrow = std::max(row, newRow);
    // adding elements to the end
    for (size_t i = 1; i < maxrow - minrow; ++i) {
        res.emplace_back(std::pair(minrow+i, maxcol-i));
    }
    return res;
}

std::vector<std::pair<int, int>> Bishop::generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const {
    std::vector<std::pair<int, int>> res;
    // Since we are checking for extra input in 
    for (int r = 0; r < 8; ++r) {
        res.emplace_back(std::pair{r,col - (row - r)}); // line y = x
        res.emplace_back(std::pair{r,col + (row - r)}); // line y = -x
    }
    
    return res;
}
