#include "Counter.h"

Counter::~Counter()
{//delete data only if this is the main instance, not a copy
    if ( this == data->ptrCounter ) delete data;
}

Counter& Counter::operator++(int)
{ 
    data->count++;
    return *this;
}

Counter& Counter::operator--(int)
{
    data->count--;
    return *this;
}

bool Counter::operator==(int compare) const
{
    return count() == compare;
}

bool Counter::operator!=(int compare) const
{
    return count() != compare;
}

bool Counter::operator<(int compare) const
{
    return count() < compare;
}

bool Counter::operator<=(int compare) const
{
    return count() <= compare;
}

bool Counter::operator>=(int compare) const
{
    return count() >= compare;
}

bool Counter::operator>(int compare) const
{
    return count() > compare;
}


bool operator==(const int lhs, const Counter& rhs)
{
    return  lhs == rhs.count();
}

bool operator!=(const int lhs, const Counter& rhs)
{
    return  lhs != rhs.count();
}

bool operator<(const int lhs, const Counter& rhs)
{
    return  lhs < rhs.count();
}

bool operator<=(const int lhs, const Counter& rhs)
{
    return  lhs <= rhs.count();
}

bool operator>=(const int lhs, const Counter& rhs)
{
    return  lhs >= rhs.count();
}

bool operator>(const int lhs, const Counter& rhs)
{
    return  lhs > rhs.count();
}


int Counter::count() const
{
    return data->count;
}

void Counter::set(int val)
{
    data->count = val;
}

void Counter::reset()
{
    data->count = 0;
}
