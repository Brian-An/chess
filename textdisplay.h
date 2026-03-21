#ifndef TEXTD_H
#define TEXTD_H
#include "observer.h"
#include "info.h"
#include "state.h"
#include "subject.h"
#include "util.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class TextDisplay: public Observer {
    vector<vector<char>> theGrid;
    public:
        TextDisplay();
        // Function to update the text output for when printing it
        virtual void notify(Subject &whoFrom) override;
    private:
        void remove(const int i, const int j);
        void replace(const int i, const int j, const char c);
        // Only accepts a-z
        char upper(const char s) const;
    friend ostream &operator<<(ostream &out, TextDisplay &td);
};

ostream &operator<<(ostream &out, TextDisplay &td);

#endif
