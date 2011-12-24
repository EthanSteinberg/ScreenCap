#include "configurationManagerImpl.hpp"

std::unique_ptr<ConfigurationManager> ConfigurationManager::create()
{
   return std::unique_ptr<ConfigurationManagerImpl>(new ConfigurationManagerImpl());
}


boost::any ConfigurationManagerImpl::getAny(const std::string& name)
{
   return myMap[name];
}

void ConfigurationManagerImpl::setAny(const std::string& name,boost::any thing)
{
   myMap[name] = thing;
}
