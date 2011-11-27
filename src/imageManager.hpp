#ifndef IMAGE_MANAGER_HPP_INCLUDED
#define IMAGE_MANAGER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

struct ImageType
{
   int shmid;
   unsigned char *shmaddr;
   double time;
   int id; /*For internal use*/
};

struct ConvertedImage
{
   void* image;
   int id; //For internal use
};


class ImageManager
{
public:
   static boost::shared_ptr<ImageManager> create(int width, int height);
   virtual boost::shared_ptr<ImageType> getImage() = 0;
   virtual void disposeImage(boost::shared_ptr<ImageType> image) = 0;

   virtual boost::shared_ptr<ConvertedImage> getConvertedImage() =0;
   virtual void disposeConvertedImage(boost::shared_ptr<ConvertedImage>) = 0;

protected:
   ~ImageManager()
   {}
};


#endif
