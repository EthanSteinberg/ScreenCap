#include "signalHandlerImpl.hpp"

#include <boost/make_shared.hpp>

#include <boost/bind.hpp>
boost::shared_ptr<SignalHandler> SignalHandler::create()
{
   return boost::make_shared<SignalHandlerImpl>();
}

SignalHandlerImpl::SignalHandlerImpl()
{
   sigemptyset(&set);
   sigaddset(&set,SIGINT);

   stoppedCapturer = false;
}


void SignalHandlerImpl::blockSignals()
{
   if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0)
   {
      perror("pthread_sigmask error");
      exit(1);
   }
}

void SignalHandlerImpl::setMessageQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   myQueue = theQueue;
}

void SignalHandlerImpl::setScreenCapturerQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   capturerQueue = theQueue;
}

void SignalHandlerImpl::setScreenCapturer(boost::shared_ptr<ScreenCapturer> theCapturer)
{
   capturer = theCapturer;
}

void SignalHandlerImpl::handleSignal()
{
   int recievedSignal;
   int error = sigwait(&set,&recievedSignal);
   if (error != 0)
   {
      char buffer[40];
      strerror_r(error,buffer,sizeof(buffer));

      printf("it was invalid set %d %s\n",error,buffer);
      exit(1);
   }

   switch(recievedSignal)
   {
      case SIGINT:
         printf("I have been cntr-c ed\n");
         if (!stoppedCapturer)
         {
            capturerQueue->pushIn(boost::bind(&ScreenCapturer::stopCapture,capturer));
            stoppedCapturer = true;
         }
         else
         {
            exit(1);
         }
         break;

      default:
         printf("Unknown signal\n");
         exit(1);
   };

   myQueue->pushIn(boost::bind(&SignalHandler::handleSignal,this));
}

