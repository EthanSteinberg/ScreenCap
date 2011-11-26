#ifndef IMAGE_MANAGER_HPP_INCLUDED
#define IMAGE_MANAGER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

struct ImageType
{
   int shmid;
   unsigned char *shmaddr;
};

class ImageManager
{
public:
   static boost::shared_ptr<ImageManager> create(int width, int height);
   virtual boost::shared_ptr<ImageType> getImage() = 0;
   virtual void disposeImage(boost::shared_ptr<ImageType> image) = 0;

protected:
   ~ImageManager()
   {}
};


#endif
