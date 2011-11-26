#include "screenRecieverPng.hpp"

#include <boost/make_shared.hpp>
#include <snappy.h>
#include <cstring>

#include <sys/eventfd.h>

#include <CImg.h>

using namespace cimg_library;

boost::shared_ptr<ScreenReciever> ScreenReciever::create()
{
   return boost::make_shared<ScreenRecieverPng>();
}


void ScreenRecieverPng::setImageManager(boost::shared_ptr<ImageManager> theManager)
{
   manager = theManager; 
   evFd = eventfd(5,EFD_SEMAPHORE);
}


void ScreenRecieverPng::processScreen(boost::shared_ptr<ImageType> image)
{

   unsigned char* picture = (unsigned char*) image->shmaddr; 
   CImg<unsigned char> pic(width,height,1,3);
      
   for (int x = 0; x < width; x++)
      for (int y = 0; y < height ; y++)
      {
         const unsigned char* addr = &picture
               [y* width * 4 + x * 4];
         
         pic(x,y,0,0) = addr[2];
         pic(x,y,0,1) = addr[1];
         pic(x,y,0,2) = addr[0];
      }


   manager->disposeImage(image);
   pic.save_png("itworkesYetAgain.png");
   

   


}

void ScreenRecieverPng::stopProcess()
{
   printf("I am looking over the frames\n");
   for (auto iter = frames.begin(); iter != frames.end(); iter++)
   {
      int out = aio_error(iter->file);
      if (out == EINPROGRESS)
         printf("It is still going\n");
      if (out > 0)
         perror("Error ");
   }
}

void ScreenRecieverPng::setSize(int Width, int Height)
{
   width = Width;
   height = Height;
}
