#include "messageQueueLocked.hpp"



#include <sys/eventfd.h>

std::unique_ptr<MessageQueue> MessageQueue::create()
{
   return std::unique_ptr<MessageQueueLocked>(new MessageQueueLocked());
}

MessageQueueLocked::MessageQueueLocked()
{
   readfd = eventfd(0,EFD_SEMAPHORE);
}

std::function< void(void)> MessageQueueLocked::getNextOrWait()
{
   uint64_t num;
   read(readfd,&num, sizeof(num));
   //printf("The amount was %ld\n",num);
   std::function<void (void)> func;

   {
      std::lock_guard<std::mutex> lock(mutex);
      func = queue.front();
      queue.pop();
   }

   return func;
}

void MessageQueueLocked::pushIn(std::function< void(void)> func)
{
   {
      std::lock_guard<std::mutex> lock(mutex);
      queue.push(func);
   }

   uint64_t num =1;
   write(readfd,&num, sizeof(num));
}
