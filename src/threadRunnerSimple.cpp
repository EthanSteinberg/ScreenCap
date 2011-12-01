#include "threadRunnerSimple.hpp"

#include <boost/make_shared.hpp>

boost::shared_ptr<ThreadRunner> ThreadRunner::create()
{
   return boost::make_shared<ThreadRunnerSimple>();
}

void ThreadRunnerSimple::run(boost::shared_ptr<MessageQueue> queue )
{
   for (;;)
   {
      boost::function<void(void)> func = queue->getNextOrWait();
      if (func.empty())
      {
         printf("This thread is shutting down\n");
         break;
      }

      func();
   }
}

