#ifndef SCREEN_RECIEVER_HPP_INCLUDED
#define SCREEN_RECIEVER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

#include "imageManager.hpp"
#include "screenDumper.hpp"
#include "messageQueue.hpp"

#include "messageQueueUser.hpp"
#include "configurationManager.hpp"

class ScreenReciever : public MessageQueueUser<ScreenReciever>
{
public:
   ScreenReciever(boost::shared_ptr<MessageQueue> queue) : MessageQueueUser(queue)
   {}

   static boost::shared_ptr<ScreenReciever> create(boost::shared_ptr<MessageQueue> queue,boost::shared_ptr<ConfigurationManager>);

   virtual void setImageManager(boost::shared_ptr<ImageManager> manager) = 0;
   virtual void processScreen( boost::shared_ptr<ImageType> ) = 0;
   virtual void setSize(int width, int height) = 0;
   virtual void setScreenDumper(boost::shared_ptr<ScreenDumper> ) = 0;
   virtual void stopProcess() = 0;

protected:
   ~ScreenReciever()
   {}
};

#endif
