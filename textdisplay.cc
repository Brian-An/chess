#include "textdisplay.h"
#include "util.h"

TextDisplay::TextDisplay() {
    for (int i = 0; i < SIZE; ++i) {
        theGrid.emplace_back(vector<char>(SIZE));
        for (int j = 0; j < SIZE; ++j) {
            remove(i, j);
        }
    }
}

void TextDisplay::remove(const int i, const int j) {
    theGrid[i][j] = ((i + j) % 2 == 1) ? ' ': '_';
}
 
void TextDisplay::replace(const int i, const int j, const char s) {
    theGrid[i][j] = s;
}

char TextDisplay::upper(const char s) const {
    return s - 'a' + 'A';
}

void TextDisplay::notify(Subject &whoFrom) {
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
    // Update logic
    switch (state.type) {
        case Type::Move:
            remove(state.prevRow, state.prevCol);
            replace(info.row, info.col, (info.colour == Colour::Black) ? c : upper(c));
            break;
        case Type::Add:
            replace(info.row, info.col, (info.colour == Colour::Black) ? c : upper(c));
            break;
        case Type::Remove:
            remove(state.prevRow, state.prevCol);
            break;
    }
}

ostream &operator<<(ostream &out, TextDisplay &td) {
    for (int i = SIZE - 1; i > -1; --i) { // Go from Row 8 to Row 1
        int boardRowNum = i + 1;
        out << boardRowNum << ' ';
        for (int j = 0; j < SIZE; ++j) {
            out << td.theGrid[i][j];
        }
        out << endl;
    }
    out << "  ";
    for (int i = 0; i < SIZE; ++i) {
        out << static_cast<char>('a' + i);
    }
    out << endl;
    return out;
}
