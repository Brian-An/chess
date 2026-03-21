#ifndef INFO_H
#define INFO_H
#include <cstddef>
#include <string>
#include <iostream>

enum class Colour {Black, White, Neither};

// Info are the information of each piece with their current location and colour and type

struct Info {
  size_t row, col;
  Colour colour;
  std::string type;
};

#endif
