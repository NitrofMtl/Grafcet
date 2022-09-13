#include "Timer.h"

void GrafcetTimer::set(Step& upstream, u_int32_t delay)
{
    previousStep = &upstream;
    _delay = delay;
}

bool GrafcetTimer::validate()
{	
    enum timerState {NONSET, END, SET };
    if ( validation == SET ) return false;
    if ( validation == END) {
        validation = NONSET;
        return true;
    }
    //on first time, set timer, next return false, then reset and return true
    //is timer have acces to inner class ??
    //all call could be done into lambdas
    validation = SET;//mark timer set
    timer.cancel(); //cancel timer to avoid double call
    timer.timeOut(_delay, [this](){
        if ( !previousStep->isActive() ) {
            validation = NONSET;
            return;
        }
        validation = END;
    });
    return false;
}

void GrafcetTimer::reset()
{
    enum timerState {NONSET, END, SET };
    if ( validation != SET ) return;
    validation = NONSET;
    timer.cancel();
}

