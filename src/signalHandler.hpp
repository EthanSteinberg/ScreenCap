#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP 


#include "messageQueue.hpp"

// NOTE : Create me before starting any threads

#include "screenCapturer.hpp"

#include "messageQueueUser.hpp"

class SignalHandler : public MessageQueueUser<SignalHandler>
{
public:
   SignalHandler(std::shared_ptr<MessageQueue> queue) : MessageQueueUser(queue)
   {}

   static std::unique_ptr<SignalHandler> create(std::shared_ptr<MessageQueue> queue);
   virtual void blockSignals() = 0;
   virtual void handleSignal() = 0;

   virtual void setScreenCapturer( std::shared_ptr<ScreenCapturer>) = 0;

   virtual ~SignalHandler()
   {}
};


#endif /* SIGNALHANDLER_HPP */
