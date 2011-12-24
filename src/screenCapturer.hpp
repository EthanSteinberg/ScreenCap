#ifndef SCREEN_CAPTURER_HPP_INCLUDED
#define SCREEN_CAPTURER_HPP_INCLUDED

#include "messageQueue.hpp"
#include "screenReciever.hpp"

#include "messageQueueUser.hpp"
#include "configurationManager.hpp"

class ScreenCapturer : public MessageQueueUser<ScreenCapturer>
{
public:
   ScreenCapturer(std::shared_ptr<MessageQueue> queue) : MessageQueueUser(queue)
   {}

   static std::unique_ptr<ScreenCapturer> create(std::shared_ptr<MessageQueue> queue, std::shared_ptr<ConfigurationManager>);
   virtual void setScreenReciever(std::shared_ptr<ScreenReciever> reciever) = 0;
   virtual void setImageManager() = 0;
   virtual void captureScreen() = 0;
   virtual void stopCapture() = 0;

   virtual ~ScreenCapturer()
   {}

};
   

#endif
