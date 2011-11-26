#include "screenRecieverX264.hpp"

#include <boost/make_shared.hpp>
#include <snappy.h>
#include <cstring>


#include <sys/eventfd.h>

extern "C"
{
#include <x264.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
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

   static int forcePts = 0;
   pic_in.i_pts = forcePts++;

   SwsContext *convertCtx = sws_getContext(width, height, PIX_FMT_RGB32, width, height, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);

   //int strides[3] = {width * 4, width * 4, width * 4};
   //unsigned char *data[3] = {image->shmaddr, image->shmaddr, image->shmaddr};
   unsigned char* data = image->shmaddr;
   int stride = width * 4;
   sws_scale(convertCtx, &data,&stride, 0, height, pic_in.img.plane, pic_in.img.i_stride);
   manager->disposeImage(image);

   x264_nal_t *nals;
   int i_nals;

   int frame_size = x264_encoder_encode(encoder,&nals,&i_nals,&pic_in,&picOut);


  x264_picture_clean(&pic_in);

   printf("The num of planes was %d %d\n",frame_size,i_nals);

   for (int i = 0 ; i < i_nals; i++)
   {
         AVPacket pkt;
         av_init_packet(&pkt);

         pkt.pts = picOut.i_pts;
         pkt.dts = picOut.i_dts;
         if (picOut.b_keyframe)
            pkt.flags |= AV_PKT_FLAG_KEY;

         pkt.stream_index = stream->index;
         pkt.data = nals[i].p_payload;
         pkt.size = nals[i].i_payload;

         int ret = av_interleaved_write_frame(os,&pkt);
         if (ret != 0)
         {
            fprintf(stderr, "Error when writing video\n");
         }
      //write(fileD, nals[i].p_payload, nals[i].i_payload);
   }


}

void ScreenRecieverX264::stopProcess()
{
   printf("I am printing delayed\n");

   while (x264_encoder_delayed_frames(encoder) != 0)
   {
      x264_picture_t picOut;
      x264_nal_t *nals;
      int i_nals;

      int frame_size = x264_encoder_encode(encoder,&nals,&i_nals,NULL,&picOut);
      printf("The num of planes was %d %d\n",frame_size,i_nals);

      for (int i = 0 ; i < i_nals; i++)
      {
         AVPacket pkt;
         av_init_packet(&pkt);

         pkt.pts = picOut.i_pts;
         if (picOut.b_keyframe)
            pkt.flags |= AV_PKT_FLAG_KEY;

         pkt.stream_index = stream->index;
         pkt.data = nals[i].p_payload;
         pkt.size = nals[i].i_payload;

         int ret = av_interleaved_write_frame(os,&pkt);
         if (ret != 0)
         {
            fprintf(stderr, "Error when writing video\n");
         }
         //write(fileD, nals[i].p_payload, nals[i].i_payload);
      }
   }

   av_write_trailer(os); 
      
  if (!(os->oformat->flags & AVFMT_NOFILE))
   {
      /* close the output file */
      avio_close(os->pb);
   };



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
   
   param.i_fps_num = 20;
   param.i_fps_den = 1;

   param.i_timebase_num = 1;
   param.i_timebase_den = 20;

   
   param.b_vfr_input = 0;

   x264_param_apply_fastfirstpass(&param);
   x264_param_apply_profile(&param, "baseline");

   encoder = x264_encoder_open(&param);


   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
   fileD = open("yay.h264", O_WRONLY | O_CREAT | O_TRUNC ,mode);

   const char *filename = "boo.mkv";

   AVOutputFormat *fmt = av_guess_format(NULL,filename,NULL);
   os = avformat_alloc_context();
   os->oformat = fmt;
   strcpy(os->filename, filename);

   stream = av_new_stream(os,0);

   AVCodecContext *codec = stream->codec;
   avcodec_get_context_defaults2(codec, AVMEDIA_TYPE_VIDEO);
   codec->codec_type = AVMEDIA_TYPE_VIDEO;

   codec->codec_id = CODEC_ID_H264;
   codec->bit_rate = 1000000;
   codec->width = width;
   codec->height = height;
   codec->time_base.num = 1;
   codec->time_base.den = 20;

if ( !( os->oformat->flags & AVFMT_NOFILE ) )
        if (avio_open( &os->pb, os->filename, URL_WRONLY ) < 0)
{
   fprintf(stderr, "could not open %s\n",os->filename);
   exit(1);
}

avformat_write_header(os, NULL);



   x264_nal_t *nals;
   int numNals;

   x264_encoder_headers(encoder,&nals,&numNals);

   for (int i = 0 ; i < numNals; i++)
   {
      write(fileD, nals[i].p_payload, nals[i].i_payload);
         AVPacket pkt;
         av_init_packet(&pkt);

         pkt.pts = i-numNals;
         pkt.dts = i-numNals;

         pkt.stream_index = stream->index;
         pkt.data = nals[i].p_payload;
         pkt.size = nals[i].i_payload;

         int ret = av_write_frame(os,&pkt);
         if (ret != 0)
         {
            fprintf(stderr, "Error when writing video\n");
         }
   }
   

   printf("The num was %d\n",numNals);
   
}

ScreenRecieverX264::~ScreenRecieverX264()
{
   x264_encoder_close(encoder);
}

