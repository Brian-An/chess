#include "human.h"
#include <sstream>

MoveRequest Human::move() {
    // Getting the line
    string line;
    getline(cin, line);
    istringstream iss{line};
    string start, end, promote;

    //if they provided less than two input, we give invalid input
    if (!(iss >> start >> end)) return {"0", "0", false, ""};
    iss >> promote;
    return {start, end, promote != "", promote};
}
