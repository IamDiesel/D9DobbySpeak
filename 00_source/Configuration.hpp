#pragma once
#include <string>
#include <map>
#include <sstream>

class Configuration
{
private:
	std::map<std::string, std::string> configValues;
	char prefixCollision;
	char prefixFloating;
	std::string filenameConfig;
	void readConfig();
	void parseJSON(std::string jsonString);
	std::string sanitize(std::string str, bool isFolder);
public:
	Configuration(std::string filenameConfig);
	std::map<std::string, std::string> getConfigMap();	
};
