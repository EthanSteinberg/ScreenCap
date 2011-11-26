#include "messageQueueLocked.hpp"

#include <boost/make_shared.hpp>

#include <sys/eventfd.h>

boost::shared_ptr<MessageQueue> MessageQueue::create()
{
   return boost::make_shared<MessageQueueLocked>();
}

MessageQueueLocked::MessageQueueLocked()
{
   readfd = eventfd(0,EFD_SEMAPHORE);
}

boost::function< void(void)> MessageQueueLocked::getNextOrWait()
{
   uint64_t num;
   read(readfd,&num, sizeof(num));
   //printf("The amount was %ld\n",num);
   boost::function<void (void)> func;

   {
      boost::lock_guard<boost::mutex> lock(mutex);
      func = queue.front();
      queue.pop();
   }

   return func;
}

void MessageQueueLocked::pushIn(boost::function< void(void)> func)
{
   {
      boost::lock_guard<boost::mutex> lock(mutex);
      queue.push(func);
   }

   uint64_t num =1;
   write(readfd,&num, sizeof(num));
}
