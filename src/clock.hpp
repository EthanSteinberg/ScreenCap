#ifndef CLOCK_HPP_INCLUDED
#define CLOCK_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

class Clock
{
public:
   static boost::shared_ptr<Clock> create();
   virtual void init() = 0;
   virtual double getSeconds() = 0;
   virtual void sleepUntilNext(double howMuch) = 0;
};

#endif
