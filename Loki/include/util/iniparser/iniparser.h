#pragma once

#ifndef INIPARSER_H
#define INIPARSER_H

#include <map>
#include <string>

namespace loki
{

namespace util
{

namespace general
{

class INIParser
{
public:
	class INIFile;

	INIParser();
	~INIParser();

	static INIFile* ParseFile( const char* _File );
private:
	//////////////////////////////////////////////////////////////////////////
	// Internal functions that are called by the flex-generated parsing code.
	//////////////////////////////////////////////////////////////////////////
	static void _ParseSection( const char* _Str );
	static void _ParseProperty( const char* _Str );

	static INIFile* m_OutputFile;
	static std::string m_CurrentSection;
};

class INIParser::INIFile
{
	friend class INIParser;
public:
	typedef std::map<std::string, std::string>		Section;	// A section maps strings to strings (property names to values).
	typedef std::map<std::string, Section>			SectionsMap;

	INIFile();
	~INIFile();

	bool GetProperty( const char* _Section, const char* _Name, std::string& _Output );
	bool GetProperty( const std::string& _Section, const std::string& _Name, std::string& _Output );
	bool GetProperty( const char* _Name, std::string& _Output );
	bool GetProperty( const std::string& _Name, std::string& _Output );

	//////////////////////////////////////////////////////////////////////////
	// Assigns a value to a property in the given section.
	// If a section or property does not exist yet, it is created.
	// Return values:
	// 0 - Section and property existed and were modified.
	// 1 - Property did not exist, had to be created.
	// 2 - Section (and thus property) did not exist, had to be created.
	//////////////////////////////////////////////////////////////////////////
	int32 SetProperty( const char* _Section, const char* _Name, const char* _Value );
	int32 SetProperty( const std::string& _Section, const std::string& _Name, const std::string& _Value );

	void RemoveSection( const char* _Section );
	void RemoveSection( const std::string& _Section );
	void RemoveProperty( const char* _Section, const char* _Name );
	void RemoveProperty( const std::string& _Section, const std::string& _Name );

	bool Export( const char* _File );

	//////////////////////////////////////////////////////////////////////////
	// Returns a const reference to the sections map. Useful to iterate over
	// the entire file.
	//////////////////////////////////////////////////////////////////////////
	const SectionsMap& GetData() const;
private:
	Section* _GetSection( const std::string& _Section );

	SectionsMap	m_Data;	// The data is organized as a map that maps strings (section names) to Section maps.
};

}

}

}

#endif