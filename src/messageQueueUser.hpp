#ifndef MESSAGEQUEUEUSER_HPP
#define MESSAGEQUEUEUSER_HPP 

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "messageQueue.hpp"

template <typename T>
class MessageQueueUser
{
public:
   MessageQueueUser(boost::shared_ptr<MessageQueue> queue) :
      myQueue(queue)
   {
   }

   void pushIn(boost::function<void (T*)> func)
   {
      myQueue.pushIn(boost::bind(func, static_cast<T*>(this)));
   }

private:
   const boost::shared_ptr<MessageQueue> myQueue;
};



#endif /* MESSAGEQUEUEUSER_HPP */
