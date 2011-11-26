#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP 

#include <boost/shared_ptr.hpp>
#include "messageQueue.hpp"

// NOTE : Create me before starting any threads

#include "screenCapturer.hpp"

class SignalHandler
{
public:
   static boost::shared_ptr<SignalHandler> create();
   virtual void blockSignals() = 0;
   virtual void setMessageQueue(boost::shared_ptr<MessageQueue>) = 0;
   virtual void handleSignal() = 0;

   virtual void setScreenCapturer( boost::shared_ptr<ScreenCapturer>) = 0;
   virtual void setScreenCapturerQueue(boost::shared_ptr<MessageQueue>) = 0;

protected:
   ~SignalHandler()
   {}
};


#endif /* SIGNALHANDLER_HPP */
