#ifndef SCREEN_DUMPER_HPP_INCLUDED
#define SCREEN_DUMPER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <cstdint>


#include "messageQueue.hpp"
#include "imageManager.hpp"

#include "messageQueueUser.hpp"
#include "configurationManager.hpp"


class ScreenDumper : public MessageQueueUser<ScreenDumper>
{
public:
   ScreenDumper(boost::shared_ptr<MessageQueue> queue) : MessageQueueUser(queue)
   {}

   static boost::shared_ptr<ScreenDumper> create(boost::shared_ptr<MessageQueue>, boost::shared_ptr<ConfigurationManager> );
   virtual void setImageManager(boost::shared_ptr<ImageManager>) = 0;
   virtual void dumpImage(boost::shared_ptr<ConvertedImage> ) = 0;
   virtual void setSize(int width, int height) = 0;
   virtual void finish() = 0;

protected:
   ~ScreenDumper()
   {}
};

#endif
