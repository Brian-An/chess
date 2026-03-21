#ifndef GRAPHIC_H
#define GRAPHIC_H
#include "info.h"
#include "observer.h"
#include "window.h"
#include "state.h"
#include "subject.h"
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class GraphicDisplay: public Observer {
    unique_ptr<Xwindow> xwin;
    size_t padding;
    private:
        // Finds the top-left coordinate of the square and square widths and lengths
        map<string, int> getSquareInfo(int row, int col) const;

        // Draws checker board
        void drawBoardSquare(int row, int col);

        // Draws a piece
        void drawPiece(string piece, int row, int col);

    public:
        // Initialize window
        GraphicDisplay(size_t padding=20);

        // Function to update the window
        virtual void notify(Subject &whoFrom) override;
};

#endif
