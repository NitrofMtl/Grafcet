#ifndef FORK_H
#define FORK_H

#include <Arduino.h>
#include "GrafcetBase\GrafcetBase.h"


class Fork : public Receptivity
{
    public:
    Fork() : next(nullptr), validatedNextStep(nullptr), receptivity(nullptr) {}
    ~Fork();    
    void add();
    Fork& operator[](unsigned int index);
    void operator=(Receptivity& recep);
    private:
    bool validate() override;
    Step* getNextStep() override;
    Fork *next;
    Step* validatedNextStep;
    Receptivity* receptivity;
};

#endif //FORK_H