#include "imageManagerSimple.hpp"

#include <boost/make_shared.hpp>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/eventfd.h>


boost::shared_ptr<ImageManager> ImageManager::create(int width, int height)
{
   return boost::make_shared<ImageManagerSimple>(width,height);
}

ImageManagerSimple::ImageManagerSimple(int Width, int Height) : width(Width), height(Height), size(width *height * 4)
{


   //id = shmget(IPC_PRIVATE , width * height * 4, IPC_CREAT | 0777);
   //pointerToBig = (char*) shmat(id,0,0);
   fd = eventfd(5, EFD_SEMAPHORE);
}

ImageManagerSimple::~ImageManagerSimple()
{
   //shmctl(id, IPC_RMID,0);
   //shmdt(pointerToBig);
}

boost::shared_ptr<ImageType> ImageManagerSimple::getImage()
{
   uint64_t val;
   read(fd,&val,sizeof(val));


   boost::lock_guard<boost::mutex> lock(mutex);
   auto image = boost::make_shared<ImageType>();
   image->shmid = shmget(IPC_PRIVATE , width * height * 4, IPC_CREAT | 0777);
   image->shmaddr = (unsigned char *) shmat(image->shmid,0,0);


   return image;
}

void ImageManagerSimple::disposeImage(boost::shared_ptr<ImageType> image)
{
   {
      boost::lock_guard<boost::mutex> lock(mutex);
      shmdt(image->shmaddr);
      shmctl(image->shmid, IPC_RMID,0);
   }
   uint64_t val = 1;
   write(fd,&val,sizeof(val));
}
