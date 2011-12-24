#ifndef SCREEN_RECIEVER_X264_HPP_INCLUDED
#define SCREEN_RECIEVER_X264_HPP_INCLUDED

#include "screenReciever.hpp"

class x264_t;
class ScreenRecieverX264 : public ScreenReciever
{
   public:
   ScreenRecieverX264(std::shared_ptr<MessageQueue> queue);
   virtual ~ScreenRecieverX264();
   
   virtual void setImageManager(std::shared_ptr<ImageManager> manager);
   virtual void setScreenDumper(std::shared_ptr<ScreenDumper> dumper);
   virtual void processScreen( std::shared_ptr<ImageType> );
   virtual void setSize(int width, int height);
   virtual void stopProcess();

   private:

      int width;
      int height;

      int forcedFrames;
      int forceMono;

      std::shared_ptr<ImageManager> manager;
      std::shared_ptr<ScreenDumper> dumper;
      
      std::shared_ptr<ConvertedImage> lastImage;
};

#endif
