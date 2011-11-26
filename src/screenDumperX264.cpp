#include "screenDumperX264.hpp"

#include <boost/make_shared.hpp>

boost::shared_ptr<ScreenDumper> ScreenDumper::create()
{
   return boost::make_shared<ScreenDumperX264>();
}

void ScreenDumperX264::setMessageQueue(boost::shared_ptr<MessageQueue> queue)
{
   myQueue = queue;
}

void ScreenDumperX264::dumpImage(boost::shared_ptr<ConvertedImage> image)
{
   x264_picture_t picOut;

   x264_nal_t *nals;
   int i_nals;



  x264_encoder_encode(encoder,&nals,&i_nals,image.get(),&picOut);


   //printf("I have dumped a screen\n");

   for (int i = 0 ; i < i_nals; i++)
   {
      write(fileD, nals[i].p_payload, nals[i].i_payload);
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
   
   param.i_fps_num = 30;
   param.i_fps_den = 1;

   param.i_timebase_num = 1;
   param.i_timebase_den = 30;

   
   param.b_vfr_input = 0;

   x264_param_apply_fastfirstpass(&param);
   x264_param_apply_profile(&param, "baseline");

   encoder = x264_encoder_open(&param);

   char file[] = "/tmp/screenXXXXXX";

   fileD = mkstemp(file);
   filename = file;


   x264_nal_t *nals;
   int numNals;

   x264_encoder_headers(encoder,&nals,&numNals);

   for (int i = 0 ; i < numNals; i++)
   {
      write(fileD, nals[i].p_payload, nals[i].i_payload);
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
         write(fileD, nals[i].p_payload, nals[i].i_payload);
      }
   }
   x264_encoder_close(encoder);

   int sin = dup(0);
   int sot = dup(1);
   int ser = dup(2);



   std::string command = "ffmpeg -y -i ";
   command+= filename;
   command+= " -vcodec copy super.mkv";
   printf("%s\n",command.c_str());

   close(0);
   close(1);
   close(2);

   system(command.c_str());

   dup2(sin,0);
   dup2(sot,1);
   dup2(ser,2);

   myQueue->pushIn(boost::function<void(void)>());
}
