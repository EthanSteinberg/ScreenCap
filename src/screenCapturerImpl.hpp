#ifndef SCREEN_CAPTURE_IMPL_HPP_INCLUDED
#define SCREEN_CAPTURE_IMPL_HPP_INCLUDED

#include "screenCapturer.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include "imageManager.hpp"

class ScreenCapturerImpl : public ScreenCapturer
{
public:
   ScreenCapturerImpl();
   virtual void setMessageQueue(boost::shared_ptr<MessageQueue> queue);
   virtual void setScreenReciever(boost::shared_ptr<ScreenReciever> reciever);
   virtual void setImageManager();
   virtual void captureScreen();

private:
   
   Display* display;
   Window root;
   XImage* image;
   int width;
   int height;
   XShmSegmentInfo shminfo;


   boost::shared_ptr<ImageManager> manager;
   boost::shared_ptr<MessageQueue> queue;
   boost::shared_ptr<ScreenReciever> reciever;
};


#endif
