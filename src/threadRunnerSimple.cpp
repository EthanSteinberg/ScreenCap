#include "threadRunnerSimple.hpp"



std::unique_ptr<ThreadRunner> ThreadRunner::create()
{
   return std::unique_ptr<ThreadRunnerSimple>(new ThreadRunnerSimple());
}

void ThreadRunnerSimple::run(std::shared_ptr<MessageQueue> queue )
{
   for (;;)
   {
      std::function<void(void)> func = queue->getNextOrWait();
      if (!func)
      {
         printf("This thread is shutting down\n");
         break;
      }

      func();
   }
}

