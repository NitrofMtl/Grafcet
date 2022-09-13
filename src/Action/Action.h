#ifndef ACTION_H
#define ACTION_H

#include <Arduino.h>

class Action
{
    protected:
    friend class Step;
    friend class StepRising;
    friend class StepFalling;
    friend class StepOnChange;
    Action() = default;
    virtual ~Action() = default;
    virtual void doAction() = 0;
};

template<class Callable>
class ActionDerived : public Action{
    private:
    friend class Step;
    friend class StepOnChange;
    ActionDerived(Callable callable) : callback(callable) { }
    ~ActionDerived() = default;
    Callable callback;
    void doAction() {callback();}
};


#endif //ACTION_H