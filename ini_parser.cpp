#include "pch.h"
#include "ini_parser.h"

/*
--   ----------------------------------------------------------------------
--   Project:          DLL creation
--
--   Author:           Hamza Qureshi
--
--   File name:        INI_Parser.h
--
--   Rev.:             1.0
--   Creation date:    DEC 2023
--
--   ---------------------------------------------------------------------
Purpose of this module: MAIN MODULE

----------Main body--------------

Comments:

WARININGS

---------------------------------------------------------------------------
*/
// ini_parser.cpp : implementation file
//


bool INIParser::load(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open the INI file: " << filename << std::endl;
		return false;
	}

	std::string line;
	std::string currentSection;

	while (std::getline(file, line))
	{
		parseLine(line, currentSection);
	}

	file.close();
	return true;
}

std::string INIParser::getValue(const std::string& section, const std::string& key) const
{
	auto sectionIter = iniData.find(section);
	if (sectionIter != iniData.end()) {
		auto keyIter = sectionIter->second.find(key);
		if (keyIter != sectionIter->second.end())
		{
			return keyIter->second;
		}
	}
	return ""; // Return empty string if the section or key is not found
}

void INIParser::parseLine(const std::string& line, std::string& currentSection)
{
	std::istringstream iss(line);
	std::string token;
	iss >> std::ws >> token;

	if (token.empty() || token[0] == ';') 
	{
		// Ignore empty lines and comments
		return;
	}

	if (token[0] == '[' && token.back() == ']') {
		// Section header
		currentSection = token.substr(1, token.size() - 2);
	}
	else {
		// Key-value pair
		size_t equalsPos = token.find('=');
		if (equalsPos != std::string::npos) 
		{
			std::string key = token.substr(0, equalsPos);
			std::string value = token.substr(equalsPos + 1);
			iniData[currentSection][key] = value;
		}
	}
}



