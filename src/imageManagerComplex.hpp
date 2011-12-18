#ifndef IMAGE_MANAGER_COMPLEX_HPP_INCLUDED
#define IMAGE_MANAGER_COMPLEX_HPP_INCLUDED

#include "imageManager.hpp"


#include <boost/thread.hpp>

const int numOfImages = 5;
const int numOfConvertedImages = 5;


class ImageManagerComplex : public ImageManager
{
public:

   ImageManagerComplex(int width, int height);
   ~ImageManagerComplex();

   virtual boost::shared_ptr<ImageType> getImage();
   virtual boost::shared_ptr<ConvertedImage> getConvertedImage() ;

private:
   void disposeImage(int id);
   void disposeConvertedImage(int id);

   int width;
   int height;
   int size;

   int imageFd;
   int imageUsed;

   int convertedImageFd;
   int convertedImageUsed;

   struct StoredImage
   {
      boost::shared_ptr<ImageType> image;
      bool isUsed;
   };

   struct StoredConvertedImage
   {
      boost::shared_ptr<ConvertedImage> image;
      bool isUsed;
   };

   std::vector<StoredImage> storedImages;
   std::vector<StoredConvertedImage> storedConvertedImages;

   boost::mutex imageMutex;
   boost::mutex convertedImageMutex;


};

#endif
