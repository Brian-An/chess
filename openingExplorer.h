#ifndef OPENINGEXPLORER_H
#define OPENINGEXPLORER_H
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <span>
#include "observer.h"
#include "info.h"
#include "state.h"
#include "subject.h"
#include "util.h"

using namespace std;

class OpeningExplorer: public Observer {
    string curUci;
    int plyNumber;
    int openingIndex;
    vector<string> ucis; // UCI is a computer-friendly chess notation format
    vector<map<string, string>> lines;

    public:
        // Sets up file reading
        OpeningExplorer();

        // Set new opening index within the UCIs given a new move
        void setOpeningIndex(const string uciMove);

        // Getters
        map<string, string> getOpeningInfo() const;
        int getPlyNumber() const;
        string getCurUci() const;

        // Change internal info when notified
        virtual void notify(Subject &whoFrom) override;
};

#endif
