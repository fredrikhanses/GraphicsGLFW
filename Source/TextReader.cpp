#include "TextReader.h"
#include <fstream>

TextReader::TextReader()
{
}

TextReader::~TextReader()
{
}

std::string TextReader::ReadText(std::string Path)
{
	std::ifstream InData;
	std::string String;
	std::string OutData;
	InData.open(Path);
	if (InData.is_open())
	{
		while (!InData.eof())
		{
			getline(InData, String);
			OutData += String;
			OutData += "\n";
		}
	}
	InData.close();
	return OutData;
}
