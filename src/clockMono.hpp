#ifndef CLOCK_MONO_HPP_INCLUDED
#define CLOCK_MONO_HPP_INCLUDED

#include "clock.hpp"
#include <time.h>

class ClockMono : public Clock
{
public:
   virtual void init();
   virtual double getSeconds();
   virtual void sleepUntilNext(double howMuch);

private:
   timespec start;
   double amountSlept;
};

#endif
