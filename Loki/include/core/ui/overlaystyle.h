#pragma once

#ifndef OVERLAYSTYLE_H
#define OVERLAYSTYLE_H

#include <map>

namespace loki
{

namespace ui
{

class LkOverlayStyle
{
	friend class LkOverlayManager;

	typedef std::map<std::string, std::string>	Properties;
	typedef std::pair<std::string, std::string>	PropertiesPair;
	typedef std::map<std::string, Properties>	Sections;
	typedef std::pair<std::string, Properties>	SectionsPair;
public:
	const std::string& GetName() const;
	const std::string& GetFile() const;

	bool GetData( const std::string& _Section, const std::string& _Property, std::string& _Output ) const;
private:
	LkOverlayStyle( const char* _Name, const char* _File, bool& _SuccessOutput );
	LkOverlayStyle();
	virtual ~LkOverlayStyle();

	std::string m_Name;
	std::string m_File;

	Sections m_Data;
};

}

}

#endif