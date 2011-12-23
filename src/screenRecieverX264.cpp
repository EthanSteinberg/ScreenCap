#include "screenRecieverX264.hpp"

#include <boost/make_shared.hpp>
#include <snappy.h>
#include <cstring>

#include <boost/bind.hpp>

#include <sys/eventfd.h>

extern "C"
{
#include <x264.h>
#include <libswscale/swscale.h>
}
#include <cstdlib>

struct ConvertedImage : public x264_picture_t
{};


boost::shared_ptr<ScreenReciever> ScreenReciever::create(boost::shared_ptr<MessageQueue> queue,boost::shared_ptr<ConfigurationManager>)
{
   return boost::make_shared<ScreenRecieverX264>(queue);
}

ScreenRecieverX264::ScreenRecieverX264(boost::shared_ptr<MessageQueue> queue) : ScreenReciever(queue)
{
   forcedFrames = 0;
   forceMono = 0;
}


void ScreenRecieverX264::setImageManager(boost::shared_ptr<ImageManager> theManager)
{
   manager = theManager;
   dumper->pushIn(boost::bind(&ScreenDumper::setImageManager,_1,manager));
}

void ScreenRecieverX264::setScreenDumper(boost::shared_ptr<ScreenDumper> theDumper)
{
   dumper = theDumper;
}



void ScreenRecieverX264::processScreen(boost::shared_ptr<ImageType> image)
{
   auto picture = manager->getConvertedImage();

   x264_picture_t *pic_in = picture.get();

   int curtime = (image->time * 30);
   pic_in->i_pts = forceMono++;
   if (curtime != forceMono-1)
   {
      //printf("bad time was %d %d\n",curtime,forceMono-1);
   }

   SwsContext *convertCtx = sws_getContext(width, height, PIX_FMT_RGB32, width, height, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);

   unsigned char* data = image->shmaddr;
   int stride = width * 4;
   sws_scale(convertCtx, &data,&stride, 0, height, pic_in->img.plane, pic_in->img.i_stride);
   //printf("I have processed a screen\n");

   while (curtime > forceMono -1)
   {
      forcedFrames++;
      printf("Forced to force a frame\n");
      forceMono++;
      pic_in->i_pts++;
      dumper->pushIn(boost::bind(&ScreenDumper::dumpImage,_1, picture));
   }

   dumper->pushIn(boost::bind(&ScreenDumper::dumpImage,_1, picture));


}

void ScreenRecieverX264::stopProcess()
{
   printf("Reciever is told to quit\n");
   printf(" %d out of %d , or %f%% frames were forced\n",forcedFrames,forceMono,(float) forcedFrames/forceMono * 100);
   dumper->pushIn(&ScreenDumper::finish);
   kill();
}

void ScreenRecieverX264::setSize(int Width, int Height)
{
   width = Width;
   height = Height;

   dumper->pushIn(boost::bind(&ScreenDumper::setSize,_1,width,height));

}

ScreenRecieverX264::~ScreenRecieverX264()
{
}

