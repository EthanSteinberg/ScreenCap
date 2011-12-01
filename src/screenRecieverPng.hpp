#ifndef SCREEN_RECIEVER_PNG_HPP_INCLUDED
#define SCREEN_RECIEVER_PNG_HPP_INCLUDED

#include "screenReciever.hpp"

class ScreenRecieverPng : public ScreenReciever
{
   public:
   virtual void setImageManager(boost::shared_ptr<ImageManager> manager);
   virtual void processScreen( boost::shared_ptr<ImageType > );

   private:
      boost::shared_ptr<ImageManager> manager;

};

#endif
