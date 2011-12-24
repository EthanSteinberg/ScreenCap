#ifndef IMAGE_MANAGER_HPP_INCLUDED
#define IMAGE_MANAGER_HPP_INCLUDED

#include <memory>


struct ImageType
{
   int shmid;
   unsigned char *shmaddr;
   double time;
};


struct ConvertedImage;


class ImageManager
{
public:
   static std::unique_ptr<ImageManager> create(int width, int height);
   
   virtual std::shared_ptr<ImageType> getImage() = 0;
   virtual std::shared_ptr<ConvertedImage> getConvertedImage() =0;

   virtual ~ImageManager()
   {}
};


#endif
