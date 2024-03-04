#include "Configuration.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>


using namespace std;


Configuration::Configuration(string filenameConfig)
{
	this->filenameConfig = filenameConfig;
	this->prefixCollision = 'C';
	this->prefixFloating = 'F';
	this->readConfig();
}

#ifdef _WIN32
void Configuration::readConfig()
{
	FILE* fp;

	fopen_s(&fp, this->filenameConfig.c_str(), "r");

	ifstream configFile(fp);
	stringstream * buffer = new stringstream();
	if (configFile.is_open())
	{
		(*buffer) << configFile.rdbuf();
		this->parseJSON(buffer->str());
		configFile.close();
		(*buffer).flush();
		delete buffer;
	}
	/*cout << "Config read: " << endl;
	for (const auto& el : this->configValues)
	{
		cout << el.first << " " << el.second << endl;
	}*/

}
#else
void Configuration::readConfig()
{
	ifstream configFile;
	configFile.open(this->filenameConfig.c_str());

	//fp = fopen(this->filenameConfig.c_str(), "r");
	//ifstream configFile(fp);
	stringstream* buffer = new stringstream();
	if (configFile)
	{
		(*buffer) << configFile.rdbuf();
		this->parseJSON(buffer->str());
		configFile.close();
		(*buffer).flush();
		delete buffer;
	}
	/*cout << "Config read: " << endl;
	for (const auto& el : this->configValues)
	{
		cout << el.first << " " << el.second << endl;
	}*/
}
#endif

void Configuration::parseJSON(string jsonString)
{
	const unsigned int bufferSize = 256;
	char buffer[bufferSize];
	stringstream ss;
	ss << jsonString;
	int cnt = 0;

	while (ss.peek() != '{')
	{
		//cout << "while1: " << tmp << endl;
		ss.ignore();
		if (++cnt > 100)
		{
			cout << "Error while finding '{': Config is too large or not readable. JSON-Format: { \"FolderSoundfiles\":\"folderpath\",\"CO\":\"filename\", \"F0\":\"filename\" }" << endl;
			cout << "C=Collision soundfile, F=Floating soundfile" << endl;
			break;
		}
	}

	cnt = 0;
	bool isFolder = false;
	while (ss.peek() != '}')
	{
		string key;
		string filename;

		//read until opening "
		ss.get(buffer, bufferSize, '\"');
		ss.ignore(); //ignore "
		//read until closing "
		ss.get(buffer, bufferSize, '\"');
		ss.ignore();
		key = string(buffer);

		//read until second opening "
		ss.get(buffer, bufferSize, '\"');
		ss.ignore(); //ignore "
		//read until second closing "
		ss.get(buffer, bufferSize, '\"');
		ss.ignore();
		filename = string(buffer);
		
		//sanitize filenames and path
		if (key.compare("FolderSoundfiles") == 0)//strcmp(key.c_str(), "FolderSoundfiles") == 0)
			isFolder = true;
		else
			isFolder = false;

		
		filename = this->sanitize(filename, isFolder);

		//add to map
		if (isFolder || ((key[0] == this->prefixCollision || key[0] == this->prefixFloating) && key.length() == 2))
		{
#ifdef _WIN32
			this->configValues.insert_or_assign(key, filename);
#else
			this->configValues.emplace(key, filename);
#endif
		}
			
		//ss.get();
		ss.ignore();

		if (++cnt > 100)
		{
			cout << "Error while finding '}': Config is too large or not readable. JSON-Format: { \"FolderSoundfiles\":\"folderpath\",\"CO\":\"filename\", \"F1\":\"filename\"}" << endl;
			cout << "C=Collision soundfile, F=Floating soundfile, Numbers 0-9 (max 10 soundfiles per event)" << endl;
			break;
		}
	}
}
string Configuration::sanitize(string str, bool isFolder)
{
	static const char badCharsFileExplicit[] = {'/', '\\'};
	static const char badChars[] = {'$','&', '?', '%', '*', ':', '|', '"', '<', '>' };

	for (const auto & el : badChars)
	{
		str.erase(remove(str.begin(), str.end(), el), str.end());
	}
	if(!isFolder)
	{
		for (const auto& el : badCharsFileExplicit)
		{
			str.erase(remove(str.begin(), str.end(), el), str.end());
		}
	}
	return str;
}
std::map<std::string, std::string> Configuration::getConfigMap()
{
	return this->configValues;
}

int main_test()
{
	cout << "alive" << endl;
	Configuration* myConfig = new Configuration("C:\\SysGCC\\raspberry\\files\\DobbySpeakConfig_WIN32.json");
	return 0;
}