#include "util.h"

std::string translateToBoard(int row, int col) {
    std::string res = "a1";
    res[0]+=col;
    res[1]+=row;
    return res;
}

std::pair<int,int> translateToNum(std::string loc) {
    return {loc[1] - '1', loc[0] - 'a'};
}

Colour negateColour(Colour c) { 
    return c == Colour::White ? Colour::Black : Colour::White;
}

std::string colourTranslator(Colour colour) { return colour == Colour::White ? "White" : "Black"; }

void randNatSeed(int seed) {
    srand(seed);
}

int randNat(size_t lim) {
    if (lim == 0) return -1;
    return rand() % lim;
}

char toUpper(char c) { return (c >= 'A' && c <= 'Z') ? c : (c - 'a' + 'A'); }
char toLower(char c) { return (c >= 'a' && c <= 'z') ? c : (c - 'A' + 'a'); }
