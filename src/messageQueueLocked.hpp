#ifndef MESSAGE_QUEUE_LOCKED_H_INCLUDED
#define MESSAGE_QUEUE_LOCKED_H_INCLUDED

#include "messageQueue.hpp"

#include <queue>
#include <thread>


class MessageQueueLocked :public MessageQueue
{
public:
   MessageQueueLocked();
   virtual ~MessageQueueLocked()
   {}

   virtual std::function< void(void)> getNextOrWait();
   virtual void pushIn(std::function< void(void)> func);

private:
   int readfd;
   std::queue<std::function<void (void)> > queue;
   std::mutex mutex;
};



#endif
