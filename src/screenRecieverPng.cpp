#include "screenRecieverPng.hpp"

#include <boost/make_shared.hpp>
using namespace cimg_library;

boost::shared_ptr<ScreenReciever> ScreenReciever::create()
{
   return boost::make_shared<ScreenRecieverPng>();
}

void ScreenRecieverPng::setImageManager(boost::shared_ptr<ImageManager> theManager)
{
   manager = theManager; 
}

void ScreenRecieverPng::processScreen( boost::shared_ptr<ImageType> picture)
{
   //picture->save_png("itworkesYetAgain.png");
   manager->disposeImage(picture);
   printf("I have recieved a screen\n");
}

