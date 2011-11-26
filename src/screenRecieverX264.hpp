#ifndef SCREEN_RECIEVER_X264_HPP_INCLUDED
#define SCREEN_RECIEVER_X264_HPP_INCLUDED

#include "screenReciever.hpp"
#include <aio.h>

class x264_t;
class AVStream;
class AVFormatContext;
class ScreenRecieverX264 : public ScreenReciever
{
   public:
   ScreenRecieverX264();
   ~ScreenRecieverX264();
   virtual void setImageManager(boost::shared_ptr<ImageManager> manager);
   virtual void processScreen( boost::shared_ptr<ImageType> );
   virtual void setSize(int width, int height);
   virtual void stopProcess();

   private:

   int fileD;
      int width;
      int height;
      boost::shared_ptr<ImageManager> manager;

   x264_t* encoder;
   AVStream *stream;
   AVFormatContext *os;
};

#endif
