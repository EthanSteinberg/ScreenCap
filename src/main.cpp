#include "messageQueue.hpp"
#include "threadRunner.hpp"
#include "screenCapturer.hpp"
#include "screenReciever.hpp"
#include "signalHandler.hpp"
#include "configurationManager.hpp"

#include <boost/program_options.hpp>

#include "messageQueueUser.hpp"

namespace po = boost::program_options;

boost::shared_ptr<ConfigurationManager> handleArgs(int argc, char** argv);

int main(int argc, char** argv)
{
   auto configuration = handleArgs(argc,argv);


   auto signalHandlerQueue = MessageQueue::create();
   auto screenCapturerQueue = MessageQueue::create();
   auto screenRecieverQueue = MessageQueue::create();
   auto screenDumperQueue = MessageQueue::create();

   auto signalHandler = SignalHandler::create(signalHandlerQueue);
   auto screenCapturer = ScreenCapturer::create(screenCapturerQueue,configuration);
   auto screenReciever = ScreenReciever::create(screenRecieverQueue,configuration);
   auto screenDumper   = ScreenDumper::create(screenDumperQueue,configuration);


   signalHandler->blockSignals();
   signalHandler->setScreenCapturer(screenCapturer);

   screenReciever->setScreenDumper(screenDumper);
   
   screenCapturer->setScreenReciever(screenReciever);
   screenCapturer->setImageManager();

   boost::thread signalThread   = ThreadRunner::createThread(signalHandlerQueue);
   boost::thread capturerThread = ThreadRunner::createThread(screenCapturerQueue);
   boost::thread recieverThread = ThreadRunner::createThread(screenRecieverQueue);
   boost::thread dumperThread = ThreadRunner::createThread(screenDumperQueue);

   signalHandler->pushIn(&SignalHandler::handleSignal);

   screenCapturer->pushIn(&ScreenCapturer::captureScreen);
   

   capturerThread.join();
   recieverThread.join();
   dumperThread.join();
   return 0;
}

boost::shared_ptr<ConfigurationManager> handleArgs(int argc, char** argv)
{
   po::positional_options_description p;
   p.add("output-file",1);


   po::options_description desc("Usage: screencap [OPTIONS] outputfile\nDefault output file is out.mkv");

   po::options_description internal;
   internal.add_options()
   ("output-file",po::value<std::string>()->default_value("out.mkv"),"set output location");

   po::options_description gen("General options");
   gen.add_options()
   ("help,h","Produce help message");

   po::options_description video("Video output options");
   video.add_options()
   ("fps", po::value<int>()->default_value(30), "Video fps, needs to be an integer");

   po::options_description other("Other options");
   other.add_options()
   ("temp-dir",po::value<std::string>()->default_value("/tmp"),"Location to store temporary files");

   desc.add(gen);
   desc.add(video);
   desc.add(other);

   po::variables_map vm;
   po::store(po::command_line_parser(argc,argv).options(internal.add(desc)).positional(p).run(),vm);
   po::notify(vm);

   if (vm.count("help"))
   {
      std::cout<<desc<<"\n";
      exit(1);
   }

   auto configuration = ConfigurationManager::create();

   configuration->setOption<int>("fps",vm["fps"].as<int>());
   configuration->setOption<std::string>("tmpDir",vm["temp-dir"].as<std::string>());
   configuration->setOption<std::string>("outFile",vm["output-file"].as<std::string>());

   return configuration;
}
