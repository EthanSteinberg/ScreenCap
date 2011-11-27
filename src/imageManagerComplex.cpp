#include "imageManagerComplex.hpp"

#include <boost/make_shared.hpp>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/eventfd.h>

#include <cstdint>
extern "C"
{
#include <x264.h>
}


boost::shared_ptr<ImageManager> ImageManager::create(int width, int height)
{
   return boost::make_shared<ImageManagerComplex>(width,height);
}

void killPicture(ConvertedImage *pic)
{
   x264_picture_clean((x264_picture_t*) pic->image);
   delete(pic);
}


ImageManagerComplex::ImageManagerComplex(int Width, int Height) : width(Width), height(Height), size(width *height * 4)
{


   //id = shmget(IPC_PRIVATE , width * height * 4, IPC_CREAT | 0777);
   //pointerToBig = (char*) shmat(id,0,0);
   imageFd = eventfd(numOfImages, EFD_SEMAPHORE);
   imageUsed = numOfImages;

   for (int i = 0; i < numOfImages;i++)
   {
      StoredImage storage;

      storage.image = boost::make_shared<ImageType>();
      storage.image->shmid = shmget(IPC_PRIVATE , width * height * 4, IPC_CREAT | 0777);
      storage.image->shmaddr = (unsigned char *) shmat(storage.image->shmid,0,0);
      storage.image->id = i;
      storage.isUsed = false;

      storedImages.push_back(storage);
   }

   convertedImageFd = eventfd(numOfConvertedImages, EFD_SEMAPHORE);
   convertedImageUsed = numOfConvertedImages;

   for (int i = 0; i < numOfConvertedImages;i++)
   {
      StoredConvertedImage storage;

      storage.image = boost::shared_ptr<ConvertedImage>(new ConvertedImage, killPicture); 
      x264_picture_t* pic = new x264_picture_t();
      x264_picture_alloc(pic,X264_CSP_I420, width, height);

      storage.image->image = pic;
      storage.image->id = i;
      storage.isUsed = false;

      storedConvertedImages.push_back(storage);
   }
       
}

ImageManagerComplex::~ImageManagerComplex()
{
   for (int i = 0; i < numOfImages;i++)
   {
      shmdt(storedImages[i].image->shmaddr);
      shmctl(storedImages[i].image->shmid, IPC_RMID,0);
   }
}

boost::shared_ptr<ImageType> ImageManagerComplex::getImage()
{
   if (imageUsed == 0)
      printf("I am stalling on get image\n");

   uint64_t val;
   read(imageFd,&val,sizeof(val));

   {
      imageUsed--;
      boost::lock_guard<boost::mutex> lock(imageMutex);

      for (int i = 0; i < numOfImages;i ++)
      {
         if (storedImages[i].isUsed == false)
         {
            storedImages[i].isUsed = true;
            return storedImages[i].image;
         }
      }

   }

   exit(1); // I should never get here
}

void ImageManagerComplex::disposeImage(boost::shared_ptr<ImageType> image)
{
   {
      boost::lock_guard<boost::mutex> lock(imageMutex);
      imageUsed++;
      storedImages[image->id].isUsed = false;
   }
   uint64_t val = 1;
   write(imageFd,&val,sizeof(val));
}

boost::shared_ptr<ConvertedImage> ImageManagerComplex::getConvertedImage()
{
   if (convertedImageUsed == 0)
      printf("I am stalling on get converted image\n");

   uint64_t val;
   read(convertedImageFd,&val,sizeof(val));

   {
      convertedImageUsed--;
      boost::lock_guard<boost::mutex> lock(convertedImageMutex);

      for (int i = 0; i < numOfImages;i ++)
      {
         if (storedConvertedImages[i].isUsed == false)
         {
            storedConvertedImages[i].isUsed = true;
            return storedConvertedImages[i].image;
         }
      }

   }

   exit(1); // I should never get here
}

void ImageManagerComplex::disposeConvertedImage(boost::shared_ptr<ConvertedImage> image)
{
   {
      boost::lock_guard<boost::mutex> lock(convertedImageMutex);
      convertedImageUsed++;
      storedConvertedImages[image->id].isUsed = false;
   }
   uint64_t val = 1;
   write(convertedImageFd,&val,sizeof(val));
}
