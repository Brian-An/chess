#include "piece.h"

Piece::Piece(size_t row, size_t col, Colour colour): row{row}, col{col}, colour{colour}, hasMoved{false} {
    setState({row, col, Type::Add});
}

void Piece::movePiece(size_t newRow, size_t newCol, bool notify) {
    setState({row, col, Type::Move});
    row = newRow;
    col = newCol;
    if (notify) {
        hasMoved = true;
        notifyObservers();
    }
}

void Piece::removePiece() {
    setState({row, col, Type::Remove});
    notifyObservers();
}
