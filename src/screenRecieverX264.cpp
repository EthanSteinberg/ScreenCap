#include "screenRecieverX264.hpp"

#include <boost/make_shared.hpp>
#include <snappy.h>
#include <cstring>


#include <sys/eventfd.h>

extern "C"
{
#include <x264.h>
#include <libswscale/swscale.h>
}
#include <CImg.h>

using namespace cimg_library;

boost::shared_ptr<ScreenReciever> ScreenReciever::create()
{
   return boost::make_shared<ScreenRecieverX264>();
}

ScreenRecieverX264::ScreenRecieverX264()
{
}


void ScreenRecieverX264::setImageManager(boost::shared_ptr<ImageManager> theManager)
{
   manager = theManager;
}


void ScreenRecieverX264::processScreen(boost::shared_ptr<ImageType> image)
{
   x264_picture_t pic_in,picOut;
   x264_picture_alloc(&pic_in,X264_CSP_I420, width, height);


   SwsContext *convertCtx = sws_getContext(width, height, PIX_FMT_RGB32, width, height, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);

   //int strides[3] = {width * 4, width * 4, width * 4};
   //unsigned char *data[3] = {image->shmaddr, image->shmaddr, image->shmaddr};
   unsigned char* data = image->shmaddr;
   int stride = width * 4;
   sws_scale(convertCtx, &data,&stride, 0, height, pic_in.img.plane, pic_in.img.i_stride);
   manager->disposeImage(image);

   x264_nal_t *nals;
   int i_nals;

  /* int frame_size =*/ x264_encoder_encode(encoder,&nals,&i_nals,&pic_in,&picOut);

   //printf("The num of planes was %d %d\n",frame_size,i_nals);

   for (int i = 0 ; i < i_nals; i++)
   {
      write(fileD, nals[i].p_payload, nals[i].i_payload);
   }


}

void ScreenRecieverX264::stopProcess()
{
  // printf("I am printing delayed\n");

   while (x264_encoder_delayed_frames(encoder) != 0)
   {
      x264_picture_t picOut;
      x264_nal_t *nals;
      int i_nals;

      /*int frame_size =*/ x264_encoder_encode(encoder,&nals,&i_nals,NULL,&picOut);
    //  printf("The num of planes was %d %d\n",frame_size,i_nals);

      for (int i = 0 ; i < i_nals; i++)
      {
         write(fileD, nals[i].p_payload, nals[i].i_payload);
      }
   }


}

void ScreenRecieverX264::setSize(int Width, int Height)
{
   width = Width;
   height = Height;

   x264_param_t param;
   x264_param_default_preset(&param, "ultrafast", NULL);
   param.i_threads = 3;
   param.i_width = width;
   param.i_height = height;
   param.i_fps_num = 25;
   param.i_fps_den = 1;
   param.b_vfr_input = 0;

   x264_param_apply_fastfirstpass(&param);
   x264_param_apply_profile(&param, "baseline");

   encoder = x264_encoder_open(&param);


   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
   fileD = open("yay.h264", O_WRONLY | O_CREAT | O_TRUNC ,mode);

   x264_nal_t *nals;
   int numNals;

   x264_encoder_headers(encoder,&nals,&numNals);

   for (int i = 0 ; i < numNals; i++)
   {
      write(fileD, nals[i].p_payload, nals[i].i_payload);
   }

   printf("The num was %d\n",numNals);
}

ScreenRecieverX264::~ScreenRecieverX264()
{
   x264_encoder_close(encoder);
}

