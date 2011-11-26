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
#include <CImg.h>
#include <cstdlib>

using namespace cimg_library;

boost::shared_ptr<ScreenReciever> ScreenReciever::create()
{
   return boost::make_shared<ScreenRecieverX264>();
}

ScreenRecieverX264::ScreenRecieverX264()
{
}


void ScreenRecieverX264::setImageManager(boost::shared_ptr<ImageManager> theManager)
{
   manager = theManager;
}

void ScreenRecieverX264::setScreenDumper(boost::shared_ptr<ScreenDumper> theDumper)
{
   dumper = theDumper;
}

void ScreenRecieverX264::setScreenDumperQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   dumperQueue = theQueue;
}

void ScreenRecieverX264::setMessageQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   myQueue = theQueue;
}

void killPicture(ConvertedImage *pic)
{
   x264_picture_clean(pic);
   delete(pic);
}

void ScreenRecieverX264::processScreen(boost::shared_ptr<ImageType> image)
{
   boost::shared_ptr<ConvertedImage> pic_in(new ConvertedImage, killPicture);
   x264_picture_alloc(pic_in.get(),X264_CSP_I420, width, height);

   static int forceMono = 0;

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
   manager->disposeImage(image);
   //printf("I have processed a screen\n");

   while (curtime > forceMono -1)
   {
      printf("Forced to force a frame\n");
      forceMono++;
      pic_in->i_pts++;
      dumperQueue->pushIn(boost::bind(&ScreenDumper::dumpImage,dumper,pic_in));
   }

   dumperQueue->pushIn(boost::bind(&ScreenDumper::dumpImage,dumper,pic_in));


}

void ScreenRecieverX264::stopProcess()
{
   printf("Reciever is told to quit\n");
   dumperQueue->pushIn(boost::bind(&ScreenDumper::finish,dumper));
   myQueue->pushIn(boost::function<void(void)>());
}

void ScreenRecieverX264::setSize(int Width, int Height)
{
   width = Width;
   height = Height;

   dumperQueue->pushIn(boost::bind(&ScreenDumper::setSize,dumper,width,height));

}

ScreenRecieverX264::~ScreenRecieverX264()
{
}

