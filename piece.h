#ifndef PIECE_H
#define PIECE_H
#include "subject.h"
#include "info.h"
#include "moverequest.h"
#include <vector>
#include <utility>
#include <memory>

class Piece: public Subject {
protected:
    size_t row, col;
    Colour colour;
    bool hasMoved;
public:
    // Constructor for the piece
    Piece(size_t row, size_t col, Colour colour);

    // Function to move and remove the Piece
    void movePiece(size_t newRow, size_t newCol, bool notify = true);
    void removePiece();

    //Function to check if the new place is allowed
    virtual std::vector<std::pair<int, int>> generateMoves(const std::vector<std::vector<std::unique_ptr<Piece>>> *pieces, const MoveRequest &lastMove) const = 0;

    virtual std::vector<std::pair<int,int>> requestLoc(size_t newRow, size_t newCol) const = 0;
    
    // Getter for movement tracking
    bool getHasMoved() const { return hasMoved; }
    
    virtual Info getInfo() const = 0;
};

#endif
