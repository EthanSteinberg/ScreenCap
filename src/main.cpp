#include "messageQueue.hpp"
#include "threadRunner.hpp"
#include "screenCapturer.hpp"
#include "screenReciever.hpp"
#include "signalHandler.hpp"

#include <boost/program_options.hpp>

#include "messageQueueUser.hpp"

namespace po = boost::program_options;

class Foo : public MessageQueueUser<Foo>
{
public:
   Foo(boost::shared_ptr<MessageQueue> qu) : MessageQueueUser<Foo>(qu)
   {}

   void wow()
   {
      printf("I cannot even believe this\n");
   }

};


int main(int argc, char** argv)
{
   auto fooQueue = MessageQueue::create();
   Foo fun(fooQueue);
   fun.pushIn(&Foo::wow);
   fooQueue->getNextOrWait()();


   int fps;
   std::string tmpDir;
   std::string outFile;

   po::positional_options_description p;
   p.add("output-file",1);


   po::options_description desc("Usage: screencap [OPTIONS] outputfile\nDefault output file is out.mkv");

   po::options_description internal;
   internal.add_options()
   ("output-file",po::value<std::string>(&outFile)->default_value("out.mkv"),"set output location");

   po::options_description gen("General options");
   gen.add_options()
   ("help,h","Produce help message");

   po::options_description video("Video output options");
   video.add_options()
   ("fps", po::value<int>(&fps)->default_value(30), "Video fps, needs to be an integer");

   po::options_description other("Other options");
   other.add_options()
   ("temp-dir",po::value<std::string>(&tmpDir)->default_value("/tmp"),"Location to store temporary files");

   desc.add(gen);
   desc.add(video);
   desc.add(other);

   po::variables_map vm;
   po::store(po::command_line_parser(argc,argv).options(internal.add(desc)).positional(p).run(),vm);
   po::notify(vm);

   if (vm.count("help"))
   {
      std::cout<<desc<<"\n";
      return 1;
   }


   


   auto signalHandlerQueue = MessageQueue::create();
   auto screenCapturerQueue = MessageQueue::create();
   auto screenRecieverQueue = MessageQueue::create();
   auto screenDumperQueue = MessageQueue::create();

   auto signalHandler = SignalHandler::create(signalHandlerQueue);
   auto screenCapturer = ScreenCapturer::create(screenCapturerQueue,fps);
   auto screenReciever = ScreenReciever::create(screenRecieverQueue);
   auto screenDumper   = ScreenDumper::create(screenDumperQueue,fps,tmpDir,outFile);


   signalHandler->blockSignals();
   signalHandler->setScreenCapturer(screenCapturer);

   screenReciever->setScreenDumper(screenDumper);
   
   screenCapturer->setScreenRecieverQueue(screenRecieverQueue);
   screenCapturer->setScreenReciever(screenReciever);
   screenCapturer->setImageManager();

   boost::thread signalThread   = ThreadRunner::createThread(signalHandlerQueue);
   boost::thread capturerThread = ThreadRunner::createThread(screenCapturerQueue);
   boost::thread recieverThread = ThreadRunner::createThread(screenRecieverQueue);
   boost::thread dumperThread = ThreadRunner::createThread(screenDumperQueue);

   signalHandlerQueue->pushIn(boost::bind(&SignalHandler::handleSignal,signalHandler));

   screenCapturerQueue->pushIn(boost::bind(&ScreenCapturer::captureScreen,screenCapturer));
   

   capturerThread.join();
   recieverThread.join();
   dumperThread.join();
   return 0;
}
