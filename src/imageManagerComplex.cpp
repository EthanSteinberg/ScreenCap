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
struct ConvertedImage : public x264_picture_t
   {};


namespace
{
void killPicture(ConvertedImage *pic)
{
   x264_picture_clean(pic);
   delete(pic);
}

void killSharedMemory(ImageType *image)
{
   shmdt(image->shmaddr);
   shmctl(image->shmid, IPC_RMID,0);
}
}



boost::shared_ptr<ImageManager> ImageManager::create(int width, int height)
{
   return boost::make_shared<ImageManagerComplex>(width,height);
}




ImageManagerComplex::ImageManagerComplex(int Width, int Height) : width(Width), height(Height), size(width *height * 4)
{


   //id = shmget(IPC_PRIVATE , width * height * 4, IPC_CREAT | 0777);
   //pointerToBig = (char*) shmat(id,0,0);
   imageFd = eventfd(numOfImages, EFD_SEMAPHORE);
   imageUsed = numOfImages;

   for (int i = 0; i < numOfImages; i++)
   {
      StoredImage storage;

      storage.image = boost::shared_ptr<ImageType>(new ImageType(),killSharedMemory);
      storage.image->shmid = shmget(IPC_PRIVATE , width * height * 4, IPC_CREAT | 0777);
      storage.image->shmaddr = (unsigned char *) shmat(storage.image->shmid,0,0);
      storage.isUsed = false;

      storedImages.push_back(storage);
   }

   convertedImageFd = eventfd(numOfConvertedImages, EFD_SEMAPHORE);
   convertedImageUsed = numOfConvertedImages;

   for (int i = 0; i < numOfConvertedImages; i++)
   {
      StoredConvertedImage storage;


      boost::shared_ptr<ConvertedImage> pic = boost::shared_ptr<ConvertedImage>(new ConvertedImage(), killPicture);
      x264_picture_alloc(pic.get(),X264_CSP_I420, width, height);

      storage.image= pic;
      storage.isUsed = false;

      storedConvertedImages.push_back(storage);
   }

}

boost::shared_ptr<ImageType> ImageManagerComplex::getImage()
{
   if (imageUsed == 0)
   {
      printf("I am stalling on get image\n");
   }

   uint64_t val;
   read(imageFd,&val,sizeof(val));

   {
      imageUsed--;
      boost::lock_guard<boost::mutex> lock(imageMutex);

      for (int i = 0; i < numOfImages; i ++)
      {
         if (storedImages[i].isUsed == false)
         {
            storedImages[i].isUsed = true;
            return boost::shared_ptr<ImageType>(storedImages[i].image.get(),
                                                boost::bind(&ImageManagerComplex::disposeImage,this,i));
         }
      }

   }

   exit(1); // I should never get here
}


void ImageManagerComplex::disposeImage(int id)
{
   {
      boost::lock_guard<boost::mutex> lock(imageMutex);
      imageUsed++;
      storedImages[id].isUsed = false;
   }
   uint64_t val = 1;
   write(imageFd,&val,sizeof(val));
}

boost::shared_ptr<ConvertedImage> ImageManagerComplex::getConvertedImage()
{
   if (convertedImageUsed == 0)
   {
      printf("I am stalling on get converted image\n");
   }

   uint64_t val;
   read(convertedImageFd,&val,sizeof(val));

   {
      convertedImageUsed--;
      boost::lock_guard<boost::mutex> lock(convertedImageMutex);

      for (int i = 0; i < numOfImages; i ++)
      {
         if (storedConvertedImages[i].isUsed == false)
         {
            storedConvertedImages[i].isUsed = true;

            return boost::shared_ptr<ConvertedImage>(storedConvertedImages[i].image.get(),
                   boost::bind(&ImageManagerComplex::disposeConvertedImage,this,i));
         }
      }

   }

   exit(1); // I should never get here
}


void ImageManagerComplex::disposeConvertedImage(int id)
{
   {
      boost::lock_guard<boost::mutex> lock(convertedImageMutex);
      convertedImageUsed++;
      storedConvertedImages[id].isUsed = false;
   }
   uint64_t val = 1;
   write(convertedImageFd,&val,sizeof(val));
}

ImageManagerComplex::~ImageManagerComplex()
{}
