#ifndef SUBJECT_H
#define SUBJECT_H
#include "state.h"
#include "info.h"
#include <vector>

class Observer;

class Subject {
    std::vector<Observer*> observers;
    State state;
  protected:
    void setState(State newS);
  public:
    void attach(Observer *o);  
    void notifyObservers();
    virtual Info getInfo() const = 0;
    State getState() const;
};

#endif
