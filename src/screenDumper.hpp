#ifndef SCREEN_DUMPER_HPP_INCLUDED
#define SCREEN_DUMPER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <cstdint>

extern "C"
{
#include <x264.h>
}

#include "messageQueue.hpp"

typedef x264_picture_t ConvertedImage;

class ScreenDumper
{
public:
   static boost::shared_ptr<ScreenDumper> create();
   virtual void setMessageQueue(boost::shared_ptr<MessageQueue>) = 0;
   virtual void dumpImage(boost::shared_ptr<ConvertedImage> ) = 0;
   virtual void setSize(int width, int height) = 0;
   virtual void finish() = 0;

protected:
   ~ScreenDumper()
   {}
};

#endif
