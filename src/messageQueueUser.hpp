#ifndef MESSAGEQUEUEUSER_HPP
#define MESSAGEQUEUEUSER_HPP 




#include "messageQueue.hpp"

template <typename T>
class MessageQueueUser
{
public:
   MessageQueueUser(std::shared_ptr<MessageQueue> queue) :
      myQueue(queue)
   {
   }

   void pushIn(std::function<void (T*)> func)
   {
      myQueue->pushIn(std::bind(func, static_cast<T*>(this)));
   }

   void kill()
   {
      myQueue->pushIn(std::function<void()>());
   }

private:
   const std::shared_ptr<MessageQueue> myQueue;
};



#endif /* MESSAGEQUEUEUSER_HPP */
