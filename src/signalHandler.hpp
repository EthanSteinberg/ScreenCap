#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP 

#include <boost/shared_ptr.hpp>
#include "messageQueue.hpp"

// NOTE : Create me before starting any threads

#include "screenCapturer.hpp"

#include "messageQueueUser.hpp"

class SignalHandler : public MessageQueueUser<SignalHandler>
{
public:
   SignalHandler(boost::shared_ptr<MessageQueue> queue) : MessageQueueUser(queue)
   {}

   static boost::shared_ptr<SignalHandler> create(boost::shared_ptr<MessageQueue> queue);
   virtual void blockSignals() = 0;
   virtual void handleSignal() = 0;

   virtual void setScreenCapturer( boost::shared_ptr<ScreenCapturer>) = 0;

protected:
   ~SignalHandler()
   {}
};


#endif /* SIGNALHANDLER_HPP */
