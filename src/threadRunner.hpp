#ifndef THREAD_RUNNER_HPP_INCLUDED
#define THREAD_RUNNER_HPP_INCLUDED

#include <memory>
#include "messageQueue.hpp"

#include <thread>

class ThreadRunner
{
public:
   static std::unique_ptr<ThreadRunner> create();
   virtual void run(std::shared_ptr<MessageQueue> queue) = 0;

   static std::thread createThread(std::shared_ptr<MessageQueue> queue)
   {
      return std::thread(&ThreadRunner::run, create(),queue);
   }

   virtual ~ThreadRunner()
   {}

};

#endif
