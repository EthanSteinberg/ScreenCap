#ifndef IMAGE_MANAGER_COMPLEX_HPP_INCLUDED
#define IMAGE_MANAGER_COMPLEX_HPP_INCLUDED

#include "imageManager.hpp"

#include <thread>

#include <vector>


const int numOfImages = 5;
const int numOfConvertedImages = 5;


class ImageManagerComplex : public ImageManager
{
public:

   ImageManagerComplex(int width, int height);

   virtual ~ImageManagerComplex();
   virtual std::shared_ptr<ImageType> getImage();
   virtual std::shared_ptr<ConvertedImage> getConvertedImage() ;

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
      std::shared_ptr<ImageType> image;
      bool isUsed;
   };

   struct StoredConvertedImage
   {
      std::shared_ptr<ConvertedImage> image;
      bool isUsed;
   };

   std::vector<StoredImage> storedImages;
   std::vector<StoredConvertedImage> storedConvertedImages;

   std::mutex imageMutex;
   std::mutex convertedImageMutex;


};

#endif
