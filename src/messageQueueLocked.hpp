#ifndef MESSAGE_QUEUE_LOCKED_H_INCLUDED
#define MESSAGE_QUEUE_LOCKED_H_INCLUDED

#include "messageQueue.hpp"

#include <queue>
#include <boost/thread.hpp>

class MessageQueueLocked :public MessageQueue
{
public:
   MessageQueueLocked();
   virtual boost::function< void(void)> getNextOrWait();
   virtual void pushIn(boost::function< void(void)> func);

private:
   int readfd;
   std::queue<boost::function<void (void)> > queue;
   boost::mutex mutex;
};



#endif
