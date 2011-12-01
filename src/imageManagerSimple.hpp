#ifndef IMAGE_MANAGER_SIMPLE_HPP_INCLUDED
#define IMAGE_MANAGER_SIMPLE_HPP_INCLUDED

#include "imageManager.hpp"

class ImageManagerSimple : public ImageManager
{
   public:    

   ImageManagerSimple(int width, int height);
   virtual boost::shared_ptr<ImageType> getImage();
   virtual void disposeImage(boost::shared_ptr<ImageType> image);

   private:
   int width;
   int height;

};

#endif
