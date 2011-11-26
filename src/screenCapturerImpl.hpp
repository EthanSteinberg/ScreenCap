#ifndef SCREEN_CAPTURE_IMPL_HPP_INCLUDED
#define SCREEN_CAPTURE_IMPL_HPP_INCLUDED

#include "screenCapturer.hpp"
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#include "imageManager.hpp"
#include "clock.hpp"

class ScreenCapturerImpl : public ScreenCapturer
{
public:
   ScreenCapturerImpl();
   ~ScreenCapturerImpl();
   virtual void setMessageQueue(boost::shared_ptr<MessageQueue> queue);
   virtual void setScreenRecieverQueue(boost::shared_ptr<MessageQueue> queue);
   virtual void setScreenReciever(boost::shared_ptr<ScreenReciever> reciever);
   virtual void setImageManager();
   virtual void captureScreen();
   virtual void stopCapture();

private:
   
   Display* display;
   Visual* visual;
   Window root;
   XImage* image;
   int width;
   int height;
   XShmSegmentInfo shminfo;

   bool first;
   bool stopped;


   boost::shared_ptr<ImageManager> manager;
   boost::shared_ptr<MessageQueue> queue;
   boost::shared_ptr<MessageQueue> recieverQueue;
   boost::shared_ptr<ScreenReciever> reciever;

   boost::shared_ptr<Clock> clock;
};


#endif
