#ifndef SIGNALHANDLERIMPL_HPP
#define SIGNALHANDLERIMPL_HPP 

#include "signalHandler.hpp"
#include <signal.h>

class SignalHandlerImpl : public SignalHandler
{
public:
   SignalHandlerImpl();
   virtual void blockSignals();
   virtual void setMessageQueue(boost::shared_ptr<MessageQueue>);
   virtual void handleSignal();

   virtual void setScreenCapturer( boost::shared_ptr<ScreenCapturer>);
   virtual void setScreenCapturerQueue(boost::shared_ptr<MessageQueue>);
protected:
   boost::shared_ptr<MessageQueue> myQueue;
   sigset_t set;

   boost::shared_ptr<ScreenCapturer> capturer;
   boost::shared_ptr<MessageQueue> capturerQueue;

   bool stoppedCapturer;
};

#endif /* SIGNALHANDLERIMPL_HPP */
