#ifndef SCREEN_RECIEVER_X264_HPP_INCLUDED
#define SCREEN_RECIEVER_X264_HPP_INCLUDED

#include "screenReciever.hpp"

class x264_t;
class ScreenRecieverX264 : public ScreenReciever
{
   public:
   ScreenRecieverX264(boost::shared_ptr<MessageQueue> queue);
   ~ScreenRecieverX264();
   
   virtual void setImageManager(boost::shared_ptr<ImageManager> manager);
   virtual void setScreenDumper(boost::shared_ptr<ScreenDumper> dumper);
   virtual void processScreen( boost::shared_ptr<ImageType> );
   virtual void setSize(int width, int height);
   virtual void stopProcess();

   private:

      int width;
      int height;

      int forcedFrames;
      int forceMono;

      boost::shared_ptr<ImageManager> manager;
      boost::shared_ptr<ScreenDumper> dumper;
      
      boost::shared_ptr<ConvertedImage> lastImage;
};

#endif
