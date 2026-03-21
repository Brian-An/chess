#ifndef MOVER_H
#define MOVER_H
#include <string>

struct MoveRequest {
    std::string prevLoc;
    std::string newLoc;
    bool promoting = false;
    std::string promoteTo;

    // Function to compare
    bool operator==(const MoveRequest &other) const;
};

#endif
