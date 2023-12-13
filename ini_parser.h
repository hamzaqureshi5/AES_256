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
// RDU_1_0Dlg.cpp : implementation file
//
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "main.h"
#include "pch.h"

class INIParser
{
public:
	bool load(const std::string& filename);

	std::string getValue(const std::string& section, const std::string& key) const;

private:
	void parseLine(const std::string& line, std::string& currentSection);

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> iniData;
};


