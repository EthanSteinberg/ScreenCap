#ifndef THREAD_RUNNER_HPP_INCLUDED
#define THREAD_RUNNER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include "messageQueue.hpp"

#include <boost/thread.hpp>

class ThreadRunner
{
public:
   static boost::shared_ptr<ThreadRunner> create();
   virtual void run(boost::shared_ptr<MessageQueue> queue) = 0;

   static boost::thread createThread(boost::shared_ptr<MessageQueue> queue)
   {
      return boost::thread(&ThreadRunner::run, create(),queue);
   }

protected:
   ~ThreadRunner(){}

};

#endif
