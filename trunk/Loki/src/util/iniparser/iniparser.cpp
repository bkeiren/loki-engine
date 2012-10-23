#include "util/iniparser/iniparser.h"
#include <fstream>
#include <cassert>
#include <ctime>

namespace iniparser
{

// Disable some warnings because they will appear in this C file.
#pragma warning( push )
#pragma warning( disable : 4003 )
#pragma warning( disable : 4018 )
extern "C"
{
#include "../src/flex/iniparser_flex.c"
};
#pragma warning( pop )	// There is no 'enable' keyword (and 'restore' does not work) for warnings, so we need
						// to push, diable, include, then pop the warning state.

}

namespace loki
{

namespace util
{

LkINIParser::INIFile* LkINIParser::m_OutputFile = NULL;
std::string LkINIParser::m_CurrentSection;

LkINIParser::LkINIParser()
{

}

LkINIParser::~LkINIParser()
{

}

LkINIParser::INIFile* LkINIParser::ParseFile( const char* _File )
{
	iniparser::_ParseSection = &_ParseSection;
	iniparser::_ParseProperty = &_ParseProperty;
	
	FILE* file;
	if (fopen_s(&file, _File, "r"))
	{
		// Error, file could not be opened.
		LOG(VL_ERROR, "INIParser::ParseFile: Could not open file '%s'", _File);
		return 0;
	}

	m_OutputFile = new INIFile();

	iniparser::yyin = file;
	iniparser::yylex();

	fclose(file);

	m_CurrentSection.clear();
	INIFile* temp = m_OutputFile;
	m_OutputFile = NULL;
	return temp;
}

void LkINIParser::_ParseSection( const char* _Str )
{
	m_CurrentSection = (_Str + 1);
	m_CurrentSection.erase(m_CurrentSection.length() - 1);
	m_OutputFile->m_Data.insert(std::pair<std::string, INIFile::Section>(m_CurrentSection, INIFile::Section()));	// Insert a new map for a section called '_Str'.
																													// Due to the way insert works, if a section with the same name exists, nothing will change.
}

void LkINIParser::_ParseProperty( const char* _Str )
{
	std::string str = _Str;
	std::string name;
	std::string value;

	// Disect '_Str' to find the name and the value of the property.
	std::string::iterator it;
	for (it = str.begin(); it != str.end(); ++it)
	{
		// Parse until a space, tab or '=' character is encountered.
		if ((*it) == ' ' || (*it) == '\t' || (*it) == '=')
		{
			// Keep parsing until no space, tab or '=' is encountered anymore (which should be the start of the value section.
			while (it != str.end() && ((*it) == ' ' || (*it) == '\t' || (*it) == '='))
			{
				++it;
			}
			break;
		}
		else
		{
			name += (*it);
		}
	}

	for (; it != str.end(); ++it)
	{
		value += (*it);
	}

	(m_OutputFile->m_Data[m_CurrentSection])[name] = value;
}

LkINIParser::INIFile::INIFile()
{

}

LkINIParser::INIFile::~INIFile()
{

}

bool LkINIParser::INIFile::GetProperty( const char* _Section, const char* _Name, std::string& _Output )
{
	return GetProperty(std::string(_Section), std::string(_Name), _Output);
}

bool LkINIParser::INIFile::GetProperty( const std::string& _Section, const std::string& _Name, std::string& _Output )
{
	Section* section = _GetSection(_Section);
	if (!section)
	{
		// No matching section exists.
		LOG(VL_ERROR, "INIFile::GetProperty: No section '%s' exists", _Section);
		return false;
	}

	Section::iterator it = section->find(_Name);
	if (it == section->end())
	{
		// No matching property exists within the section.
		LOG(VL_ERROR, "INIFile::GetProperty: No property '%s' exists", _Name);
		return false;
	}

	_Output = (*it).second;
	return true;
}

bool LkINIParser::INIFile::GetProperty( const char* _Name, std::string& _Output )
{
	return GetProperty(std::string(_Name), _Output);
}

bool LkINIParser::INIFile::GetProperty( const std::string& _Name, std::string& _Output )
{
	for (SectionsMap::iterator it = m_Data.begin(); it != m_Data.end(); ++it)
	{
		Section* section = &(*it).second;
		for (Section::iterator it2 = section->begin(); it2 != section->end(); ++it2)
		{
			if ((*it2).first == _Name)
			{
				_Output = (*it2).second;
				return true;
			}
		}
	}
	LOG(VL_ERROR, "INIFile::GetProperty: No property '%s' exists in any section", _Name);
	return false;
}

int32 LkINIParser::INIFile::SetProperty( const char* _Section, const char* _Name, const char* _Value )
{
	return SetProperty(std::string(_Section), std::string(_Name), std::string(_Value));
}

int32 LkINIParser::INIFile::SetProperty( const std::string& _Section, const std::string& _Name, const std::string& _Value )
{
	int32 ret = 0;
	Section* section = _GetSection(_Section);
	if (!section)
	{
		// The section doesn't exist yet, create a new section.
		m_Data.insert(std::pair<std::string, INIFile::Section>(_Section, INIFile::Section()));
		section = _GetSection(_Section);

		ret = 2;
	}

	if (!section)
	{
		assert("INIParser::INIFile::SetProperty: Code should not reach this point!" && 0);
	}

	// Assign the value.
	Section::iterator it = section->find(_Name);
	if (it == section->end())
	{
		// Property does not exist yet, needs to be inserted.
		ret = 1;
	}
	section->operator [](_Name) = _Value;
	return ret;
}

void LkINIParser::INIFile::RemoveSection( const char* _Section )
{
	RemoveSection(std::string(_Section));
}

void LkINIParser::INIFile::RemoveSection( const std::string& _Section )
{
	m_Data.erase(_Section);
}

void LkINIParser::INIFile::RemoveProperty( const char* _Section, const char* _Name )
{
	RemoveProperty(std::string(_Section), std::string(_Name));
}

void LkINIParser::INIFile::RemoveProperty( const std::string& _Section, const std::string& _Name )
{
	Section* section = _GetSection(_Section);
	if (!section)
	{
		// No section matches the given section name.
		LOG(VL_ERROR, "INIFile::RemoveProperty: No section '%s' exists", _Section);
		return;
	}

	section->erase(_Name);
}

bool LkINIParser::INIFile::Export( const char* _File )
{
	std::ofstream file = std::ofstream(_File);
	if (!file.is_open())
	{
		// File could not be opened/created.
		LOG(VL_ERROR, "INIFile::Export: Could not open/create output file '%s'", _File);
		return false;
	}

	file.clear();


	//////////////////////////////////////////////////////////////////////////
	// Add a header to the file to indicate that the file was exported
	// automatically.
	//////////////////////////////////////////////////////////////////////////
	std::string timestamp;
	GetTimeStamp(timestamp);
	file << ";\n; This file was exported by code @ ";
	file << timestamp;
	file << "\n; by function '";
	file << __FUNCTION__;
	file << "' in file ";
	file << __FILE__;
	file << "\n;\n\n";

	for (SectionsMap::iterator it = m_Data.begin(); it != m_Data.end(); ++it)
	{
		// Write the section name to the file.
		file << '[';
		file << (*it).first;
		file << "]\n";

		Section* section = &(*it).second;
		for (Section::iterator it2 = section->begin(); it2 != section->end(); ++it2)
		{
			file << (*it2).first;
			file << "=";
			file << (*it2).second;
			file << '\n';
		}

		file << '\n';
	}

	file.close();
	return true;
}

const LkINIParser::INIFile::SectionsMap& LkINIParser::INIFile::GetData() const
{
	return m_Data;
}

LkINIParser::INIFile::Section* LkINIParser::INIFile::_GetSection( const std::string& _Section )
{
	SectionsMap::iterator it = m_Data.find(_Section);
	if (it == m_Data.end())
	{
		// No matching section found.
		return 0;
	}
	return &(*it).second;
}

}

}