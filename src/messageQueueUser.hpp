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

   template<typename Func, typename ...Args>
   void pushIn(Func func,Args&& ... args )
   {
      myQueue->pushIn(std::bind(func, static_cast<T*>(this),std::forward<Args>(args)...));
   }

   void kill()
   {
      myQueue->pushIn(std::function<void()>());
   }

private:
   const std::shared_ptr<MessageQueue> myQueue;
};



#endif /* MESSAGEQUEUEUSER_HPP */
