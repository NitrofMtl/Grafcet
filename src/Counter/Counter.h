#ifndef GRAFCET_COUNTER_H
#define GRAFCET_COUNTER_H

#include <Arduino.h>

enum op { EQUAL, NON_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL };

class Counter;

class CounterContainer
{    
    CounterContainer(Counter* c) : count(0), ptrCounter(c)
    {}
    int count;
    Counter* ptrCounter;
    friend class Counter;
};

class Counter
{
    public:
    Counter() : data(new CounterContainer(this))
    {}
    Counter(Counter& c) : data(c.data)
    {}
    ~Counter();
    Counter& operator++(int);
    Counter& operator--(int);

    bool operator==(int compare) const;
    bool operator!=(int compare) const;
    bool operator<(int compare) const;
    bool operator<=(int compare) const;
    bool operator>=(int compare) const;
    bool operator>(int compare) const;

    friend bool operator==(const int lhs, const Counter& rhs);
    friend bool operator!=(const int lhs, const Counter& rhs);
    friend bool operator<(const int lhs, const Counter& rhs);
    friend bool operator<=(const int lhs, const Counter& rhs);
    friend bool operator>(const int lhs, const Counter& rhs);
    friend bool operator>(const int lhs, const Counter& rhs);

    int count() const;
    void set(int val);
    void reset();
    private:
    CounterContainer *data;
};

#endif //GRAFCET_COUNTER_H
