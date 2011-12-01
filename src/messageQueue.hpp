#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

class MessageQueue
{
public:
   static boost::shared_ptr<MessageQueue> create();
   virtual boost::function< void(void)> getNextOrWait() = 0;
   virtual void pushIn(boost::function< void(void)> func) = 0;

protected:
   ~MessageQueue(){}

};


#endif
