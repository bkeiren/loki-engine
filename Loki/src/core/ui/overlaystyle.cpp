#include "core/ui/overlaystyle.h"
#include "util/iniparser/iniparser.h"

namespace loki
{

namespace ui
{

LkOverlayStyle::LkOverlayStyle( const char* _Name, const char* _File, bool& _SuccessOutput )	:
	m_Name(_Name),
	m_File(_File)
{
	_SuccessOutput = false;
	
	util::general::INIParser::INIFile* file = util::general::INIParser::ParseFile(_File);
	if (!file)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// The INI file's structure is mirrored in the overlay style. This means
	// that the data is stored in a map that links section names to
	// maps of data. These second-tier maps of data link property names to
	// string data.
	//////////////////////////////////////////////////////////////////////////

	const util::general::INIParser::INIFile::SectionsMap& data = file->GetData();
	for (util::general::INIParser::INIFile::SectionsMap::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		const util::general::INIParser::INIFile::Section* section = &((*it).second);

		std::pair<Sections::iterator, bool> p = m_Data.insert(SectionsPair((*it).first, Properties()));
		
		if (!p.second)
		{
			// A section name duplicate was found. This isn't an issue as we can just group the properties under this name.
			// TODO: Maybe output a warning because it's probably not intended.
		}

		for (util::general::INIParser::INIFile::Section::const_iterator it2 = section->begin(); it2 != section->end(); ++it2)
		{
			(*p.first).second.insert(PropertiesPair((*it2).first, (*it2).second));
		}
	}

	_SuccessOutput = true;
}

LkOverlayStyle::LkOverlayStyle()
{
	ILLEGAL_CTOR_ERROR("OverlayStyle");
}

LkOverlayStyle::~LkOverlayStyle()
{

}

const std::string& LkOverlayStyle::GetName() const
{
	return m_Name;
}

const std::string& LkOverlayStyle::GetFile() const
{
	return m_File;
}

bool LkOverlayStyle::GetData( const std::string& _Section, const std::string& _Property, std::string& _Output ) const
{
	Sections::const_iterator it = m_Data.find(_Section);
	if (it == m_Data.end())
	{
		LOG(VL_ERROR, "OverlayStyle::GetData: A section with name '%s' does not exist", _Section.c_str());
		return false;
	}

	Properties::const_iterator it2 = (*it).second.find(_Property);
	if (it2 == (*it).second.end())
	{
		LOG(VL_ERROR, "OverlayStyle::GetData: A property with name '%s' in section '%s' does not exist", _Property.c_str(), _Section.c_str());
		return false;
	}

	_Output = (*it2).second;
	return true;
}

}

}
