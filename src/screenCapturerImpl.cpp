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
   stopped = false;
   clock = Clock::create();
}

ScreenCapturerImpl::~ScreenCapturerImpl()
{
}


void ScreenCapturerImpl::setMessageQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   queue = theQueue;
}
void ScreenCapturerImpl::setScreenRecieverQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   recieverQueue = theQueue;
}

void ScreenCapturerImpl::stopCapture()
{
   queue->pushIn(boost::function<void(void)>());
   stopped = true;
   printf("Screen capture stop\n");
   recieverQueue->pushIn(boost::bind(&ScreenReciever::stopProcess,reciever));
}

void ScreenCapturerImpl::setImageManager()
{
   manager = ImageManager::create(width,height);
   recieverQueue->pushIn(boost::bind(&ScreenReciever::setImageManager,reciever,manager));
   recieverQueue->pushIn(boost::bind(&ScreenReciever::setSize,reciever,width,height));
   
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

   stuff->time = time;
   //printf("I have captured a screen at %f\n",time);

   recieverQueue->pushIn(boost::bind(&ScreenReciever::processScreen,reciever,stuff));
   
   clock->sleepUntilNext(1.0/30.0);
   queue->pushIn(boost::bind(&ScreenCapturer::captureScreen,this));
}

void ScreenCapturerImpl::setScreenReciever(boost::shared_ptr<ScreenReciever> theReciever)
{
   reciever = theReciever;
}
