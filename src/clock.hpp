#ifndef CLOCK_HPP_INCLUDED
#define CLOCK_HPP_INCLUDED

#include <memory>


class Clock
{
public:
   static std::unique_ptr<Clock> create();
   virtual void init() = 0;
   virtual double getSeconds() = 0;
   virtual void sleepUntilNext(double howMuch) = 0;

   virtual ~Clock()
   {}
};

#endif
