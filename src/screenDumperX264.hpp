#ifndef SCREEN_DUMPER_X264_INCLUDED
#define SCREEN_DUMPER_X264_INCLUDED

#include "screenDumper.hpp"

#include <cstdint>
extern "C"
{
#include <x264.h>
}


class ScreenDumperX264 : public ScreenDumper
{
public:
   ScreenDumperX264(std::shared_ptr<MessageQueue> queue, std::shared_ptr<ConfigurationManager>);
   virtual ~ScreenDumperX264() {}
   virtual void dumpImage(std::shared_ptr<ConvertedImage> );
   virtual void setImageManager(std::shared_ptr<ImageManager>);
   virtual void setSize(int width, int height);
   virtual void finish();

private:
   std::string tmpDir;
   std::string outFile;

   std::shared_ptr<ImageManager> manager;

   int fps;

   int width;
   int height;
   std::string filename;
   int fileD;
   x264_t* encoder;
};

#endif
