#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "info.h"

const int SIZE = 8;
const int PROCESS_ID = getpid();

// Helper functions to use across the project
std::string translateToBoard(int row, int col);
std::pair<int,int> translateToNum(std::string loc);
Colour negateColour(Colour c);
std::string colourTranslator(Colour colour);

// Sets random seed, default to process id
void randNatSeed(int seed=PROCESS_ID);
// Pseudo-random number generation
int randNat(size_t lim);

char toUpper(char c);
char toLower(char c);


#endif
