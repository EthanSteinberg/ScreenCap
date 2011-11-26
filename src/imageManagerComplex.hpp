#ifndef IMAGE_MANAGER_COMPLEX_HPP_INCLUDED
#define IMAGE_MANAGER_COMPLEX_HPP_INCLUDED

#include "imageManager.hpp"


#include <boost/thread.hpp>


class ImageManagerComplex : public ImageManager
{
   public:    

   ImageManagerComplex(int width, int height);
   ~ImageManagerComplex();

   virtual boost::shared_ptr<ImageType> getImage();
   virtual void disposeImage(boost::shared_ptr<ImageType> image);

   private:

   int width;
   int height;
   int size;

   int fd;

   boost::mutex mutex;


};

#endif
