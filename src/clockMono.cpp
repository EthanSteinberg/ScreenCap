#include "clockMono.hpp"

#include <boost/make_shared.hpp>
#include <cstring>

boost::shared_ptr<Clock> Clock::create()
{
   return boost::make_shared<ClockMono>();
}

void ClockMono::init()
{
   clock_gettime(CLOCK_MONOTONIC,&start);
   amountSlept = 0;
}

double ClockMono::getSeconds()
{
   timespec current;
   clock_gettime(CLOCK_MONOTONIC, &current);
   
   double seconds = current.tv_sec - start.tv_sec;
   seconds+= (current.tv_nsec - start.tv_nsec) / 1000000000.0;
   return seconds;
}

void ClockMono::sleepUntilNext(double interval)
{
   amountSlept += interval;
   
   timespec request;
   int seconds = amountSlept;
   int nanosecs = (amountSlept - seconds) * 1000000000;

   request.tv_nsec = nanosecs + start.tv_nsec;
   if (request.tv_nsec > 999999999)
   {
      request.tv_nsec -= 999999999;
      seconds++;
   }
   request.tv_sec = seconds + start.tv_sec;

   int error = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request,NULL);
   if (error != 0)
   {
      char buffer[60];
      strerror_r(error,buffer,sizeof(buffer));
      printf("The error was %s\n",buffer);
      exit(1);
   }
}

   



