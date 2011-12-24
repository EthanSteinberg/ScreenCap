#ifndef CONFIGURATIONMANAGERIMPL_HPP
#define CONFIGURATIONMANAGERIMPL_HPP 

#include "configurationManager.hpp"
#include <map>

class ConfigurationManagerImpl : public ConfigurationManager
{
public:
   virtual ~ConfigurationManagerImpl()
   {}
private:
   virtual boost::any getAny(const std::string& name);
   virtual void setAny(const std::string& name, boost::any thing);


   std::map<std::string, boost::any> myMap;
};

#endif /* CONFIGURATIONMANAGERIMPL_HPP */
