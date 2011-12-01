#include <X11/Xlib.h>


#include <cstdio>
#include <CImg.h>
#include <X11/extensions/XShm.h>
#include <sys/shm.h>
#include <sys/ipc.h>


#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}


#include "messageQueue.hpp"
#include "threadRunner.hpp"
#include "screenCapturer.hpp"
#include "screenReciever.hpp"

#include <boost/thread.hpp>

using namespace cimg_library;

void fooFunc(int i)
{
   printf("The num was %d\n",i);
}


int main()
{

   auto screenCapturerQueue = MessageQueue::create();
   auto screenRecieverQueue = MessageQueue::create();

   auto screenCapturer = ScreenCapturer::create();
   auto screenReciever = ScreenReciever::create();


   screenCapturer->setMessageQueue(screenRecieverQueue);
   screenCapturer->setScreenReciever(screenReciever);
   screenCapturer->setImageManager();
   
   boost::thread capturerThread = ThreadRunner::createThread(screenCapturerQueue);
   boost::thread recieverThread = ThreadRunner::createThread(screenRecieverQueue);

   for (int i = 0; i < 100; i++)
   screenCapturerQueue->pushIn(boost::bind(&ScreenCapturer::captureScreen,screenCapturer));

   screenCapturerQueue->pushIn(boost::function<void(void) >());


   capturerThread.join();
   screenRecieverQueue->pushIn(boost::function<void(void)>());
   recieverThread.join();
   return 0;

   Display *display = XOpenDisplay(NULL);
   Window root = DefaultRootWindow(display);
   Visual *visual = DefaultVisual(display,DefaultScreen(display));

   XWindowAttributes gwa;

   XGetWindowAttributes(display, root, &gwa);
   int width = gwa.width;
   int height = gwa.height;



   XShmSegmentInfo shminfo;
   XImage *image = XShmCreateImage(display,visual, 24,2, NULL, &shminfo,width, height);

   shminfo.shmid = shmget(IPC_PRIVATE, image->bytes_per_line * image->height, IPC_CREAT | 0777);
   image->data = (char *)shmat(shminfo.shmid,0,0);
   shminfo.shmaddr = image->data;
   shminfo.readOnly = False;

   printf("%d\n", XShmAttach(display,&shminfo));

   //XImage *image = XGetImage(display,root, 0,0 , width,height,AllPlanes, ZPixmap);

   XShmGetImage(display,root,image,0,0,AllPlanes);

   printf("%d %d %d\n",image->depth,image->format,1);

   CImg<unsigned char> pic(width,height,1,3);


   unsigned long red_mask = image->red_mask;
   unsigned long green_mask = image->green_mask;
   unsigned long blue_mask = image->blue_mask;

   for (int x = 0; x < width; x++)
      for (int y = 0; y < height ; y++)
      {
         unsigned long pixel = XGetPixel(image,x,y);

         unsigned char blue = pixel & blue_mask;
         unsigned char green = (pixel & green_mask) >> 8;
         unsigned char red = (pixel & red_mask) >> 16;

         pic(x,y,0,0) = red;
         pic(x,y,0,1) = green;
         pic(x,y,0,2) = blue;
      }


   printf("%ld %ld %ld\n",red_mask>> 16, green_mask>>8, blue_mask);




   avcodec_init();
   avcodec_register_all();
   av_register_all();
   int i, out_size, x, y, outbuf_size;
   uint8_t *outbuf;

   const char *filename = "test.webm";

   AVFormatContext *os = avformat_alloc_context();

   if (!os)
   {
      exit(1);
   }

   AVOutputFormat *fmt = av_guess_format(NULL,filename,NULL);
   os->oformat = fmt;




   AVStream *stream = av_new_stream(os,0);

   AVCodecContext *c = stream->codec;
   c->codec_id =fmt->video_codec;
   c->codec_type = AVMEDIA_TYPE_VIDEO;
   c->bit_rate = 400000;
   c->width = width;
   c->height = height;
   c->time_base.num = 1;
   c->time_base.den = 25;

   c->gop_size = 12;
   c->pix_fmt = PIX_FMT_YUV420P;

   if (os->oformat->flags & AVFMT_GLOBALHEADER)
   {
      c->flags |= CODEC_FLAG_GLOBAL_HEADER;
   }

   av_dump_format(os, 0, filename, 1);

   AVCodec *codec = avcodec_find_encoder(fmt->video_codec);

   /* open it */
   if (avcodec_open(c, codec) < 0)
   {
      fprintf(stderr, "could not open codec\n");
      exit(1);
   }

   if (!(fmt->flags & AVFMT_NOFILE))
   {
      if (avio_open(&os->pb, filename, URL_WRONLY) < 0)
      {
         fprintf(stderr, "Could not open '%s'\n", filename);
         exit(1);
      }
   }

   avformat_write_header(os,NULL);

   /* alloc image and output buffer */
   outbuf_size = 100000000;
   outbuf =(unsigned char *) malloc(outbuf_size);

   /* the image can be allocated by any means and av_image_alloc() is
    * just the most convenient way if av_malloc() is to be used */
   AVFrame *picture = avcodec_alloc_frame();
   av_image_alloc(picture->data, picture->linesize,
                  c->width, c->height, PIX_FMT_RGB24, 1);

   picture->linesize[1] = picture->linesize[0];
   picture->linesize[2] = picture->linesize[0];

   picture->data[1] = picture->data[0]+1;
   picture->data[2] = picture->data[0]+2;

   AVFrame *target = avcodec_alloc_frame();
   av_image_alloc(target->data, target->linesize, c->width, c->height, c->pix_fmt,1);

   /* encode 1 second of video */
   for (i=0; i<184; i++)
   {
      fflush(stdout);


      for (y=0; y<c->height; y++)
      {
         for (x=0; x<c->width; x++)
         {
            picture->data[0][y * picture->linesize[0] + x*3] = pic(x,y,0,0);
            picture->data[1][y * picture->linesize[1] + x*3] = pic(x,y,0,1);
            picture->data[2][y * picture->linesize[2] + x*3] = pic(x,y,0,2);
         }
      }


      SwsContext *sws = sws_getContext(c->width, c->height,PIX_FMT_RGB24,
                                       c->width, c->height, c->pix_fmt, SWS_BILINEAR,NULL,NULL,NULL);
      sws_scale(sws,picture->data,picture->linesize,0,c->height, target->data,target->linesize);


      out_size = avcodec_encode_video(c, outbuf, outbuf_size, target);

      if (out_size>0)
      {
         AVPacket pkt;
         av_init_packet(&pkt);

         if (c->coded_frame->pts != (unsigned) AV_NOPTS_VALUE)
         {
            pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, stream->time_base);
         }

         if (c->coded_frame->key_frame)
         {
            pkt.flags |= AV_PKT_FLAG_KEY;
         }

         pkt.stream_index = stream->index;
         pkt.data = outbuf;
         pkt.size = out_size;

         /* write the compressed frame in the media file */
         int ret = av_interleaved_write_frame(os, &pkt);

         if (ret != 0)
         {
            fprintf(stderr,"Error when writing video frame\n");
         }
      }

   }

   

   for (; out_size; i++)
   {

      out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);
      printf("write frame %3d (size=%5d)\n", i, out_size);
      if (out_size>0)
      {
         AVPacket pkt;
         av_init_packet(&pkt);

         if (c->coded_frame->pts != (unsigned) AV_NOPTS_VALUE)
         {
            pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, stream->time_base);
         }

         if (c->coded_frame->key_frame)
         {
            pkt.flags |= AV_PKT_FLAG_KEY;
         }

         pkt.stream_index = stream->index;
         pkt.data = outbuf;
         pkt.size = out_size;

         /* write the compressed frame in the media file */
         int ret = av_interleaved_write_frame(os, &pkt);

         if (ret != 0)
         {
            fprintf(stderr,"Error when writing video frame\n");
         }
      }
   }


   av_write_trailer(os);



   free(outbuf);

   avcodec_close(c);

   av_free(c);
   av_free(picture->data[0]);
   av_free(picture);
   av_free(target->data[0]);
   av_free(target);
   av_free(stream);


   if (!(fmt->flags & AVFMT_NOFILE))
   {
      /* close the output file */
      avio_close(os->pb);
   }

   av_free(os);

   printf("\n");



   return 0;
}
