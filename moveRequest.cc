#include "moverequest.h"

bool MoveRequest::operator==(const MoveRequest &other) const {
    return this->prevLoc == other.prevLoc && this->newLoc == other.newLoc && this->promoting == other.promoting && this->promoteTo == other.promoteTo;
}
