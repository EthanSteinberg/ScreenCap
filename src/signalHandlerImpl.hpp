#ifndef SIGNALHANDLERIMPL_HPP
#define SIGNALHANDLERIMPL_HPP 

#include "signalHandler.hpp"
#include <signal.h>

class SignalHandlerImpl : public SignalHandler
{
public:
   SignalHandlerImpl(std::shared_ptr<MessageQueue> queue);
   virtual ~SignalHandlerImpl()
   {}

   virtual void blockSignals();
   virtual void handleSignal();

   virtual void setScreenCapturer( std::shared_ptr<ScreenCapturer>);
protected:
   sigset_t set;

   std::shared_ptr<ScreenCapturer> capturer;

   bool stoppedCapturer;
};

#endif /* SIGNALHANDLERIMPL_HPP */
