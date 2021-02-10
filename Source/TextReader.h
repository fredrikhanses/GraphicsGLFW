#pragma once
#include <string>

class TextReader
{
public:
	TextReader();
	~TextReader();
	std::string ReadText(std::string Path);
private:
};