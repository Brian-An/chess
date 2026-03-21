#include "openingExplorer.h"

OpeningExplorer::OpeningExplorer(): plyNumber{0}, openingIndex{-1} {
    ifstream inFile{"opening-moves.tsv"};
    
    string uciSingle, pgn, name, eco;
    while (getline(inFile, uciSingle, '\t')) {
        // Add to UCIs
        ucis.emplace_back(uciSingle);

        // Add to each line's info
        getline(inFile, pgn, '\t');
        getline(inFile, name, '\t');
        getline(inFile, eco);
        lines.emplace_back(map<string, string>{{"pgn", pgn}, {"name", name}, {"eco", eco}});
    }
}

void OpeningExplorer::setOpeningIndex(const string uciMove) {
    if (curUci.length()) curUci += ' ';
    curUci += uciMove;

    // Binary search for a UCI match (the array ucis is presorted from its file)
    int l = 0;
    int r = ucis.size() - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (ucis[m] == curUci) {
            openingIndex = m; // Set openingIndex only if found, keep unchanged otherwise
            return;
        }
        else if (ucis[m] < curUci) {
            l = m + 1;
        }
        else {
            r = m - 1;
        }
    }
}

map<string, string> OpeningExplorer::getOpeningInfo() const {
    return (openingIndex == -1) ? map<string, string>{{"pgn", "N/A"}, {"name", "Start Position"}, {"eco", "N/A"}} : lines[openingIndex];
}

int OpeningExplorer::getPlyNumber() const {
    return plyNumber;
}

string OpeningExplorer::getCurUci() const {
    return curUci;
}

void OpeningExplorer::notify(Subject &whoFrom) {
    const Info &info = whoFrom.getInfo();
    const State &state = whoFrom.getState();

    // Check for starting position
    if (info.col == state.prevCol && info.row == state.prevRow) {
        openingIndex = -1;
        return;
    };

    // Convert internal data to parsible UCI format and update opening index
    string moveUci = translateToBoard(state.prevRow, state.prevCol) + translateToBoard(info.row, info.col);
    setOpeningIndex(moveUci);
    plyNumber++;
}
