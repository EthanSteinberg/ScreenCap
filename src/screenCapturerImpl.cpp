#include "screenCapturerImpl.hpp"
#include <boost/make_shared.hpp>

#include "screenReciever.hpp"

#include <boost/bind.hpp>

#include <sys/ipc.h>
#include <sys/shm.h>

boost::shared_ptr<ScreenCapturer> ScreenCapturer::create()
{
   return boost::make_shared<ScreenCapturerImpl>();
}

ScreenCapturerImpl::ScreenCapturerImpl()
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
   clock = Clock::create();
}

ScreenCapturerImpl::~ScreenCapturerImpl()
{
}


void ScreenCapturerImpl::setMessageQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   queue = theQueue;
}

void ScreenCapturerImpl::setImageManager()
{
   manager = ImageManager::create(width,height);
   queue->pushIn(boost::bind(&ScreenReciever::setImageManager,reciever,manager));
   queue->pushIn(boost::bind(&ScreenReciever::setSize,reciever,width,height));
   
}

void ScreenCapturerImpl::captureScreen()
{
   if (first)
   {
      clock->init();
      first = false;
   }

   else
      clock->sleepUntilNext(1.0/20.0);
   double time = clock->getSeconds();

   auto stuff = manager->getImage();

   shminfo.shmid = stuff->shmid;
   image->data = (char*) stuff->shmaddr;
   shminfo.shmaddr = image->data;

   XShmAttach(display,&shminfo);
   

   XShmGetImage(display,root,image,0,0,AllPlanes);

   XShmDetach(display,&shminfo);


   //unsigned char* bigArray = new unsigned char[width * height * 4];
   //unsigned char* bigArray = (unsigned char*)image->data;
   //memcpy(bigArray, image->data, width * height * 4);
  

   //printf("%d %d %d\n",image->depth,image->bytes_per_line,image->bits_per_pixel);

   
   stuff->time = time;
   //printf("I have captured a screen at %f\n",time);

   queue->pushIn(boost::bind(&ScreenReciever::processScreen,reciever,stuff));
}

void ScreenCapturerImpl::setScreenReciever(boost::shared_ptr<ScreenReciever> theReciever)
{
   reciever = theReciever;
}
