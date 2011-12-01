#include "screenCapturerImpl.hpp"
#include <boost/make_shared.hpp>

#include "screenReciever.hpp"

#include <boost/bind.hpp>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <CImg.h>
using namespace cimg_library;

boost::shared_ptr<ScreenCapturer> ScreenCapturer::create()
{
   return boost::make_shared<ScreenCapturerImpl>();
}

ScreenCapturerImpl::ScreenCapturerImpl()
{
   display = XOpenDisplay(NULL);
   root = DefaultRootWindow(display);

   Visual* visual = DefaultVisual(display, DefaultScreen(display));
   XWindowAttributes gwa;

   XGetWindowAttributes(display, root , &gwa);
   width = gwa.width;
   height = gwa.height;


   image = XShmCreateImage(display,visual, 24,2, NULL, &shminfo,width, height);

   shminfo.shmid = shmget(IPC_PRIVATE, image->bytes_per_line * image->height, IPC_CREAT | 0777);
   image->data = (char *)shmat(shminfo.shmid,0,0);
   shminfo.shmaddr = image->data;
   shminfo.readOnly = False;

   printf("And the shm was %d\n", XShmAttach(display,&shminfo));

}

void ScreenCapturerImpl::setMessageQueue(boost::shared_ptr<MessageQueue> theQueue)
{
   queue = theQueue;
}

void ScreenCapturerImpl::setImageManager()
{
   manager = ImageManager::create(width,height);
   queue->pushIn(boost::bind(&ScreenReciever::setImageManager,reciever,manager));
}

void ScreenCapturerImpl::captureScreen()
{
   XShmGetImage(display,root,image,0,0,AllPlanes);

   printf("%d %d %d\n",image->depth,image->format,1);

   auto picture = manager->getImage();
   

   /* 
   CImg<unsigned char> &pic = *picture;

   unsigned long red_mask = image->red_mask;
   unsigned long green_mask = image->green_mask;
   unsigned long blue_mask = image->blue_mask;

   for (int x = 0; x < width; x++)
      for (int y = 0; y < height ; y++)
      {
         unsigned long pixel = XGetPixel(image,x,y);

         unsigned char blue = pixel & blue_mask;
         unsigned char green = (pixel & green_mask) >> 8;
         unsigned char red = (pixel & red_mask) >> 16;

         pic(x,y,0,0) = red;
         pic(x,y,0,1) = green;
         pic(x,y,0,2) = blue;
      }

   */
   
   printf("I have captured a screen\n");
   queue->pushIn(boost::bind(&ScreenReciever::processScreen,reciever,picture));
}

void ScreenCapturerImpl::setScreenReciever(boost::shared_ptr<ScreenReciever> theReciever)
{
   reciever = theReciever;
}
