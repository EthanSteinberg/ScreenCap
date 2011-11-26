#ifndef SCREEN_DUMPER_X264_INCLUDED
#define SCREEN_DUMPER_X264_INCLUDED

#include <cstdint>
extern "C"
{
#include <x264.h>
}

#include "screenDumper.hpp"

class ScreenDumperX264 : public ScreenDumper
{
public:
   virtual void setMessageQueue(boost::shared_ptr<MessageQueue>);
   virtual void dumpImage(boost::shared_ptr<ConvertedImage> );
   virtual void setSize(int width, int height);
   virtual void finish();

private:
   boost::shared_ptr<MessageQueue> myQueue;


   int width;
   int height;
   std::string filename;
   int fileD;
   x264_t* encoder;
};

#endif
