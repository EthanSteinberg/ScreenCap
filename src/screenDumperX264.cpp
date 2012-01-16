#include "screenDumperX264.hpp"


#include <boost/scoped_array.hpp>

struct ConvertedImage : public x264_picture_t
{};

std::unique_ptr<ScreenDumper> ScreenDumper::create(std::shared_ptr<MessageQueue> queue, std::shared_ptr<ConfigurationManager> config)
{
   return std::unique_ptr<ScreenDumperX264>(new ScreenDumperX264(queue,config));
}

ScreenDumperX264::ScreenDumperX264(std::shared_ptr<MessageQueue> queue, std::shared_ptr<ConfigurationManager> config) : ScreenDumper(queue)
{
   fps = config->getOption<int>("fps");
   tmpDir = config->getOption<std::string>("tmpDir");
   outFile = config->getOption<std::string>("outFile");
}

void ScreenDumperX264::setImageManager(std::shared_ptr<ImageManager> theManager)
{
   manager = theManager;
}

void ScreenDumperX264::dumpImage(std::shared_ptr<ConvertedImage> image)
{
   x264_picture_t picOut;

   x264_nal_t *nals;
   int i_nals;
   x264_picture_t* in = image.get();



  x264_encoder_encode(encoder,&nals,&i_nals,in,&picOut);


   //printf("I have dumped a screen\n");

   for (int i = 0 ; i < i_nals; i++)
   {
      if (write(fileD, nals[i].p_payload, nals[i].i_payload) == -1)
      {
         perror("write image failed: ");
         exit(1);
      }
   }

}

void ScreenDumperX264::setSize(int Width, int Height)
{
   width = Width;
   height = Height;
   
   x264_param_t param;
   x264_param_default_preset(&param, "ultrafast", NULL);
   param.i_threads = 3;
   param.i_width = width;
   param.i_height = height;
   
   param.i_fps_num = fps;
   param.i_fps_den = 1;

   param.i_timebase_num = 1;
   param.i_timebase_den = fps;

   
   param.b_vfr_input = 0;

   x264_param_apply_fastfirstpass(&param);
   x264_param_apply_profile(&param, "baseline");

   encoder = x264_encoder_open(&param);

   tmpDir += "/screenXXXXXX";
   boost::scoped_array<char> tmpFileName(new char[tmpDir.length()+1]);
   strcpy(tmpFileName.get(),tmpDir.c_str());

   fileD = mkstemp(tmpFileName.get());
   filename = tmpFileName.get();


   x264_nal_t *nals;
   int numNals;

   x264_encoder_headers(encoder,&nals,&numNals);

   for (int i = 0 ; i < numNals; i++)
   {
      if (write(fileD, nals[i].p_payload, nals[i].i_payload) == -1){
         perror("write headers failed");
         exit(1);}
   }

   printf("The num was %d\n",numNals);
}

void ScreenDumperX264::finish()
{
   printf("Dumper told to quit\n");

   while (x264_encoder_delayed_frames(encoder) != 0)
   {
      x264_picture_t picOut;
      x264_nal_t *nals;
      int i_nals;

      x264_encoder_encode(encoder,&nals,&i_nals,NULL,&picOut);

      for (int i = 0 ; i < i_nals; i++)
      {
         if (write(fileD, nals[i].p_payload, nals[i].i_payload) == -1){
            perror("write delayed failed");
            exit(1);}
      }
   }
   x264_encoder_close(encoder);

   int sin = dup(0);
   int sot = dup(1);
   int ser = dup(2);



   std::string command = "ffmpeg -y -i ";
   command+= filename;
   command+= " -vcodec copy ";
   command+= outFile;
   printf("%s\n",command.c_str());

   close(0);
   close(1);
   close(2);

   system(command.c_str());

   dup2(sin,0);
   dup2(sot,1);
   dup2(ser,2);

   kill();
}
