#include "configurationManagerImpl.hpp"
#include <boost/make_shared.hpp>

boost::shared_ptr<ConfigurationManager> ConfigurationManager::create()
{
   return boost::make_shared<ConfigurationManagerImpl>();
}


boost::any ConfigurationManagerImpl::getAny(const std::string& name)
{
   return myMap[name];
}

void ConfigurationManagerImpl::setAny(const std::string& name,boost::any thing)
{
   myMap[name] = thing;
}
