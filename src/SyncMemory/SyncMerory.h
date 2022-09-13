#ifndef SYNC_MEMORY_H
#define SYNC_MEMORY_H

#include "GrafcetBase\GrafcetBase.h"

class SyncMerory : public Receptivity
{
    public:
    SyncMerory(Step& step) : stepSynced(&step)
    {}
    void operator=(Step& step)
    {
        stepSynced = &step;
    }

    private:
    bool validate()
    {
        if (!stepSynced) return false;
        return stepSynced->isActive();
    }
    Step* stepSynced;
};




#endif //SYNC_MEMORY_H