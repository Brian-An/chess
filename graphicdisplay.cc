#include "graphicdisplay.h"

#include <utility>
#include <map>
#include "piecesImgBits.h"
#include "util.h"

const bool SQUARE_BLACK = true;
const bool SQUARE_WHITE = false;

// Maps the type and square of the piece to intger bytes representing an image
const map<pair<string, bool>, int*> PIECE_STR_TO_IMG_REP = {
{{"P", SQUARE_WHITE}, whitePawnBits}, {{"p", SQUARE_BLACK}, whitePawnBits}, 
{{"P",  SQUARE_BLACK}, blackPawnBits}, {{"p",  SQUARE_WHITE}, blackPawnBits},
{{"B", SQUARE_WHITE}, whiteBishopBits}, {{"b", SQUARE_BLACK}, whiteBishopBits}, 
{{"B",  SQUARE_BLACK}, blackBishopBits}, {{"b",  SQUARE_WHITE}, blackBishopBits},
{{"R", SQUARE_WHITE}, whiteRookBits}, {{"r", SQUARE_BLACK}, whiteRookBits}, 
{{"R",  SQUARE_BLACK}, blackRookBits}, {{"r",  SQUARE_WHITE}, blackRookBits},
{{"N", SQUARE_WHITE}, whiteKnightBits}, {{"n", SQUARE_BLACK}, whiteKnightBits}, 
{{"N",  SQUARE_BLACK}, blackKnightBits}, {{"n",  SQUARE_WHITE}, blackKnightBits},
{{"K", SQUARE_WHITE}, whiteKingBits}, {{"k", SQUARE_BLACK}, whiteKingBits}, 
{{"K",  SQUARE_BLACK}, blackKingBits}, {{"k",  SQUARE_WHITE}, blackKingBits},
{{"Q", SQUARE_WHITE}, whiteQueenBits}, {{"q", SQUARE_BLACK}, whiteQueenBits}, 
{{"Q",  SQUARE_BLACK}, blackQueenBits}, {{"q",  SQUARE_WHITE}, blackQueenBits}};

// Offsets for GUI components
const int PIECE_X_OFFSET = 0;
const int PIECE_Y_OFFSET = 0;
const int ROW_LABEL_X_OFFSET = -10;
const int ROW_LABEL_Y_OFFSET = 3;
const int COL_LABEL_X_OFFSET = -2;
const int COL_LABEL_Y_OFFSET = 70;

GraphicDisplay::GraphicDisplay(size_t padding): xwin{make_unique<Xwindow>()}, padding{padding} {
    xwin->fillRectangle(0, 0, xwin->getWidth(), xwin->getHeight(), Xwindow::Green);

    // Draw squares
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            drawBoardSquare(i, j);
        }
    }
    // Draw board labels
    map<string, int> list;
    int x, y;
    for (int i = 0; i < SIZE; ++i) {
        // Draw row labels
        list = getSquareInfo(i, 0);
        x = list["xStart"] + ROW_LABEL_X_OFFSET;
        y = list["yStart"] + list["height"] / 2 + ROW_LABEL_Y_OFFSET;
        xwin->drawString(x, y, to_string(i + 1));

        // Draw col labels
        list = getSquareInfo(0, i);
        x = list["xStart"] + list["width"] / 2 + COL_LABEL_X_OFFSET;
        y = list["yStart"] + COL_LABEL_Y_OFFSET;
        xwin->drawString(x, y, string{static_cast<char>('a' + i)});
    }
}

map<string, int> GraphicDisplay::getSquareInfo(int row, int col) const {
    const int w = xwin->getWidth() - 2 * padding;
    const int h = xwin->getHeight() - 2 * padding;
    int squareWidth = w / SIZE;
    int squareHeight = h / SIZE;
    int xStart = padding + col * squareWidth;
    int yStart = xwin->getHeight() - padding - (row + 1) * squareHeight;

    // Handle off-by-one pixels
    if (w % SIZE > col) {
        xStart += col;
        squareWidth++;
    }
    else {
        xStart += w % SIZE;
    }
    if (h % SIZE > row) {
        yStart -= row + 1;
        squareHeight++;
    }
    else {
        yStart -= h % SIZE;
    }

    return {{"xStart", xStart}, {"yStart", yStart}, {"width", squareWidth}, {"height", squareHeight}};
}

void GraphicDisplay::drawBoardSquare(int row, int col) {
    map<string, int> list = getSquareInfo(row, col);
    
    // Choose colour
    if ((row + col) % 2 == 0) xwin->fillRectangle(list["xStart"], list["yStart"], list["width"], list["height"], Xwindow::Black);
    else xwin->fillRectangle(list["xStart"], list["yStart"], list["width"], list["height"], Xwindow::White);
}

void GraphicDisplay::drawPiece(string piece, int row, int col) {
    map<string, int> list = getSquareInfo(row, col);
    int xStart = list["xStart"];
    int yStart = list["yStart"];
    int squareWidth = list["width"];
    int squareHeight = list["height"];

    // Draw image
    bool darkBG = (row + col) % 2 - 1;
    if (!PIECE_STR_TO_IMG_REP.count({piece, darkBG})) {
        cerr << "Piece with str rep " << piece << " does not exist within PIECE_STR_TO_IMG_REP!" << endl;
        return;
    }
    int *srcBytes = PIECE_STR_TO_IMG_REP.at({piece, darkBG}); // the pointer variables are found in piecesImgBits.h
    // Convert to char arrays
    char bytes[BIT_LENGTHS];
    for (int i = 0; i < BIT_LENGTHS; ++i) {
        bytes[i] = srcBytes[i];
    }
    // XWindow draw image function
    xwin->drawImage(xStart, yStart, bytes, squareWidth - PIECE_X_OFFSET, squareHeight - PIECE_Y_OFFSET, darkBG);
}

void GraphicDisplay::notify(Subject &whoFrom) {
    const Info &info = whoFrom.getInfo();
    const State &state = whoFrom.getState();

    // Error checking
    if (!("a" <= info.type && info.type <= "z")) {
        cerr << "TextDisplay::notify() - whoFrom.getState().type must be a-z!" << endl;
        return;
    }
    if (!(info.type.length() == 1)) {  
        cerr << "TextDisplay::notify() - whoFrom.getState().type must have length 1!" << endl;
        return;
    }

    char c = info.type[0];
    c = (info.colour == Colour::Black) ? c : c - 'a' + 'A';
    string s{c};
    // Update logic
    switch (state.type) {
        case Type::Move:
            drawBoardSquare(state.prevRow, state.prevCol);
            drawPiece(s, info.row, info.col);
            break;
        case Type::Add:
            drawPiece(s, info.row, info.col);
            break;
        case Type::Remove:
            drawBoardSquare(state.prevRow, state.prevCol);
            break;
    }
}
