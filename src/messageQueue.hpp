#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <functional>
#include <memory>

class MessageQueue
{
public:
   static std::unique_ptr<MessageQueue> create();
   virtual std::function< void()> getNextOrWait() = 0;
   virtual void pushIn(std::function< void()> func) = 0;

   virtual ~MessageQueue()
   {}

};


#endif
