#ifndef SCREEN_DUMPER_HPP_INCLUDED
#define SCREEN_DUMPER_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <cstdint>


#include "messageQueue.hpp"
#include "imageManager.hpp"


class ScreenDumper
{
public:
   static boost::shared_ptr<ScreenDumper> create(int fps,std::string tmpDir, std::string outFile);
   virtual void setMessageQueue(boost::shared_ptr<MessageQueue>) = 0;
   virtual void setImageManager(boost::shared_ptr<ImageManager>) = 0;
   virtual void dumpImage(boost::shared_ptr<ConvertedImage> ) = 0;
   virtual void setSize(int width, int height) = 0;
   virtual void finish() = 0;

protected:
   ~ScreenDumper()
   {}
};

#endif
