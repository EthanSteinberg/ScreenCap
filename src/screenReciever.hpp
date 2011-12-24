#ifndef SCREEN_RECIEVER_HPP_INCLUDED
#define SCREEN_RECIEVER_HPP_INCLUDED

#include <memory>

#include "imageManager.hpp"
#include "screenDumper.hpp"
#include "messageQueue.hpp"

#include "messageQueueUser.hpp"
#include "configurationManager.hpp"

class ScreenReciever : public MessageQueueUser<ScreenReciever>
{
public:
   ScreenReciever(std::shared_ptr<MessageQueue> queue) : MessageQueueUser(queue)
   {}

   static std::unique_ptr<ScreenReciever> create(std::shared_ptr<MessageQueue> queue,std::shared_ptr<ConfigurationManager>);

   virtual void setImageManager(std::shared_ptr<ImageManager> manager) = 0;
   virtual void processScreen( std::shared_ptr<ImageType> ) = 0;
   virtual void setSize(int width, int height) = 0;
   virtual void setScreenDumper(std::shared_ptr<ScreenDumper> ) = 0;
   virtual void stopProcess() = 0;

   virtual ~ScreenReciever()
   {}
};

#endif
