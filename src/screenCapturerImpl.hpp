#ifndef SCREEN_CAPTURE_IMPL_HPP_INCLUDED
#define SCREEN_CAPTURE_IMPL_HPP_INCLUDED

#include "screenCapturer.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include "imageManager.hpp"
#include "clock.hpp"

#define cimg_use_png
#include <CImg.h>

using namespace cimg_library;
class ScreenCapturerImpl : public ScreenCapturer
{
public:
   ScreenCapturerImpl(std::shared_ptr<MessageQueue> queue, std::shared_ptr<ConfigurationManager>);
   ~ScreenCapturerImpl();
   virtual void setScreenReciever(std::shared_ptr<ScreenReciever> reciever);
   virtual void setImageManager();
   virtual void captureScreen();
   virtual void stopCapture();

private:
   CImg<unsigned char> cursorImage;
   
   Display* display;
   Visual* visual;
   Window root;
   XImage* image;
   int width;
   int height;
   XShmSegmentInfo shminfo;

   int fps;

   bool first;
   bool stopped;


   std::shared_ptr<ImageManager> manager;
   std::shared_ptr<ScreenReciever> reciever;

   std::shared_ptr<Clock> clock;
};


#endif
