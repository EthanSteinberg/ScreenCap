#include "screenCapturerImpl.hpp"
#include <boost/make_shared.hpp>

#include "screenReciever.hpp"
#include <X11/extensions/Xfixes.h>

#include <boost/bind.hpp>

#include <sys/ipc.h>
#include <sys/shm.h>



boost::shared_ptr<ScreenCapturer> ScreenCapturer::create(boost::shared_ptr<MessageQueue> queue,int fps)
{
   return boost::make_shared<ScreenCapturerImpl>(queue,fps);
}

ScreenCapturerImpl::ScreenCapturerImpl(boost::shared_ptr<MessageQueue> queue,int aFps) : ScreenCapturer(queue)
{
   display = XOpenDisplay(NULL);
   root = DefaultRootWindow(display);

   visual = DefaultVisual(display, DefaultScreen(display));
   XWindowAttributes gwa;

   XGetWindowAttributes(display, root , &gwa);
   width = gwa.width;
   height = gwa.height;


   image = XShmCreateImage(display,visual, 24,2, NULL, &shminfo,width, height);
   
   shminfo.readOnly = False;
   first = true;
   stopped = false;
   clock = Clock::create();

   fps = aFps;

   cursorImage.load_png("../res/arrow.png");
}

ScreenCapturerImpl::~ScreenCapturerImpl()
{
}


void ScreenCapturerImpl::stopCapture()
{
   kill();
   stopped = true;
   printf("Screen capture stop at %f\n",clock->getSeconds());
   reciever->pushIn(&ScreenReciever::stopProcess);
}

void ScreenCapturerImpl::setImageManager()
{
   manager = ImageManager::create(width,height);
   reciever->pushIn(boost::bind(&ScreenReciever::setImageManager,_1,manager));
   reciever->pushIn(boost::bind(&ScreenReciever::setSize,_1,width,height));
   
}

void ScreenCapturerImpl::captureScreen()
{
   if (stopped)
      return;

   if (first)
   {
      clock->init();
      first = false;
   }

   double time = clock->getSeconds();
   auto stuff = manager->getImage();

   shminfo.shmid = stuff->shmid;
   image->data = (char*) stuff->shmaddr;
   shminfo.shmaddr = image->data;

   XShmAttach(display,&shminfo);


   XShmGetImage(display,root,image,0,0,AllPlanes);

   XShmDetach(display,&shminfo);

   XFixesCursorImage *cursor = XFixesGetCursorImage(display);

   unsigned int* pixels = (unsigned int*) image->data;
   int xOffset = cursor->x - cursor->xhot;
   int yOffset = cursor->y - cursor->yhot;
   XFree(cursor);

   for (int y = 0; y < cursorImage.height()/2; y++)
   for (int x = 0; x < cursorImage.width()/2; x ++)
   {
      int Y = y + yOffset;
      int X = x + xOffset;

      if (X >= width || Y >= height || X <0 || Y < 0)
      {
         continue;
      }

      unsigned char*out = (unsigned char*) &(pixels[Y * width + X]);
      int a = cursorImage(x*2,y*2,3);
      int b = 255 - a;

      out[0] = (cursorImage(x*2,y*2,0)*a + out[0]*b)/255;
      out[1] = (cursorImage(x*2,y*2,1)*a + out[1] *b)/255;
      out[2] = (cursorImage(x*2,y*2,2)*a + out[2]*b)/255;

   }



   stuff->time = time;
   //printf("I have captured a screen at %f\n",time);

   reciever->pushIn(boost::bind(&ScreenReciever::processScreen,_1,stuff));
   
   clock->sleepUntilNext(1.0/fps);
   pushIn(&ScreenCapturer::captureScreen);
}

void ScreenCapturerImpl::setScreenReciever(boost::shared_ptr<ScreenReciever> theReciever)
{
   reciever = theReciever;
}
