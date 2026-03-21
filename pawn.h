 #ifndef PAWN_H
#define PAWN_H
#include "piece.h"
#include "moverequest.h"
#include <vector>

class Pawn: public Piece {
// The pawn piece
public:
    Pawn(size_t row, size_t col, Colour colour);

    // Function to get the information of each piece - used for observers to know where you are.
    virtual Info getInfo() const override;

    virtual std::vector<std::pair<int, int>> generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const override;
    virtual std::vector<std::pair<int,int>> requestLoc(size_t newRow, size_t newCol) const override;
};

#endif
