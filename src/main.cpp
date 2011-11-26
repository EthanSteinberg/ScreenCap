#include "messageQueue.hpp"
#include "threadRunner.hpp"
#include "screenCapturer.hpp"
#include "screenReciever.hpp"
#include "signalHandler.hpp"


int main()
{
   auto signalHandlerQueue = MessageQueue::create();
   auto screenCapturerQueue = MessageQueue::create();
   auto screenRecieverQueue = MessageQueue::create();
   auto screenDumperQueue = MessageQueue::create();

   auto signalHandler = SignalHandler::create();
   auto screenCapturer = ScreenCapturer::create();
   auto screenReciever = ScreenReciever::create();
   auto screenDumper   = ScreenDumper::create();


   signalHandler->blockSignals();
   signalHandler->setMessageQueue(signalHandlerQueue);
   signalHandler->setScreenCapturer(screenCapturer);
   signalHandler->setScreenCapturerQueue(screenCapturerQueue);

   screenDumper->setMessageQueue(screenDumperQueue);   

   screenReciever->setMessageQueue(screenRecieverQueue);
   screenReciever->setScreenDumperQueue(screenDumperQueue);
   screenReciever->setScreenDumper(screenDumper);
   
   screenCapturer->setMessageQueue(screenCapturerQueue);
   screenCapturer->setScreenRecieverQueue(screenRecieverQueue);
   screenCapturer->setScreenReciever(screenReciever);
   screenCapturer->setImageManager();

   boost::thread signalThread   = ThreadRunner::createThread(signalHandlerQueue);
   boost::thread capturerThread = ThreadRunner::createThread(screenCapturerQueue);
   boost::thread recieverThread = ThreadRunner::createThread(screenRecieverQueue);
   boost::thread dumperThread = ThreadRunner::createThread(screenDumperQueue);

   signalHandlerQueue->pushIn(boost::bind(&SignalHandler::handleSignal,signalHandler));

   screenCapturerQueue->pushIn(boost::bind(&ScreenCapturer::captureScreen,screenCapturer));
   

   capturerThread.join();
   recieverThread.join();
   dumperThread.join();
   return 0;
}
