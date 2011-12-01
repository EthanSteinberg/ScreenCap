#include "imageManagerSimple.hpp"

#include <boost/make_shared.hpp>


boost::shared_ptr<ImageManager> ImageManager::create(int width, int height)
{
   return boost::make_shared<ImageManagerSimple>(width,height);
}

ImageManagerSimple::ImageManagerSimple(int Width, int Height) : width(Width), height(Height)
{
}

boost::shared_ptr<ImageType> ImageManagerSimple::getImage()
{
   return boost::make_shared<ImageType>(width,height,1,3);
}

void ImageManagerSimple::disposeImage(boost::shared_ptr<ImageType> )
{
}
