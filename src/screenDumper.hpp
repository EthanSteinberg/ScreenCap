#ifndef SCREEN_DUMPER_HPP_INCLUDED
#define SCREEN_DUMPER_HPP_INCLUDED

#include <memory>
#include <cstdint>


#include "messageQueue.hpp"
#include "imageManager.hpp"

#include "messageQueueUser.hpp"
#include "configurationManager.hpp"


class ScreenDumper : public MessageQueueUser<ScreenDumper>
{
public:
   ScreenDumper(std::shared_ptr<MessageQueue> queue) : MessageQueueUser(queue)
   {}

   static std::unique_ptr<ScreenDumper> create(std::shared_ptr<MessageQueue>, std::shared_ptr<ConfigurationManager> );
   virtual void setImageManager(std::shared_ptr<ImageManager>) = 0;
   virtual void dumpImage(std::shared_ptr<ConvertedImage> ) = 0;
   virtual void setSize(int width, int height) = 0;
   virtual void finish() = 0;

   virtual ~ScreenDumper()
   {}
};

#endif
