#include "IOManager.h"
#include <fstream>

bool IOManager::readFileToBuffer(const std::string &filename, std::vector<unsigned char> &buffer)
{
	std::ifstream file(filename, std::ios::binary);
	if (file.fail())
	{
		perror(filename.c_str());
		return false;
	}

	// seek to the end
	file.seekg(0, std::ios::end);
	// so we can get the file size
	int fileSize = static_cast<int>(file.tellg());
	// restart to read
	file.seekg(0, std::ios::beg);

	// remove possible header size
	fileSize -= static_cast<int>(file.tellg());

	buffer.resize(fileSize);
	file.read((char*)&buffer[0], fileSize);
	file.close();

	return true;
}

bool IOManager::readFileToBuffer(const std::string &filename, std::string &buffer)
{
	std::ifstream file(filename, std::ios::binary);
	if (file.fail())
	{
		perror(filename.c_str());
		return false;
	}

	// seek to the end
	file.seekg(0, std::ios::end);
	// so we can get the file size
	int fileSize = static_cast<int>(file.tellg());
	// restart to read
	file.seekg(0, std::ios::beg);

	// remove possible header size
	fileSize -= static_cast<int>(file.tellg());

	buffer.resize(fileSize);
	file.read((char*)&buffer[0], fileSize);
	file.close();

	return true;
}
