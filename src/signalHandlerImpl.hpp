#ifndef SIGNALHANDLERIMPL_HPP
#define SIGNALHANDLERIMPL_HPP 

#include "signalHandler.hpp"
#include <signal.h>

class SignalHandlerImpl : public SignalHandler
{
public:
   SignalHandlerImpl(boost::shared_ptr<MessageQueue> queue);
   virtual void blockSignals();
   virtual void handleSignal();

   virtual void setScreenCapturer( boost::shared_ptr<ScreenCapturer>);
protected:
   sigset_t set;

   boost::shared_ptr<ScreenCapturer> capturer;

   bool stoppedCapturer;
};

#endif /* SIGNALHANDLERIMPL_HPP */
