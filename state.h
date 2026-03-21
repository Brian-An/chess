#ifndef STATE_H
#define STATE_H
#include <cstddef>

enum class Type {Move, Add, Remove};

struct State {
    size_t prevRow;
    size_t prevCol;
    Type type;
};

#endif
