#include "core/game/localization/localization.h"
#include "util/csvparser/csvparser.h"
#include "util/hash/hash.h"

// If defined, an explicit error string is returned when a requested string does not appear in the localization table.
// If not defined, the string name is simply returned (This is the name that is used to query the localized string).
#define LOCALIZATION_RETURN_EXPLICIT_ERRORSTRING_ON_MISSING

namespace loki
{

namespace game
{

LkLocalization* g_Localization;

namespace
{

// An array of strings representing each locale by name.
const char* LocaleStrings[LOCALE_COUNT] = { "English", 
											"Dutch", 
											"French", 
											"German", 
											"Russian", 
											"Spanish", 
											"Italian", 
											"Norwegian", 
											"Swedish", 
											"Danish", 
											"Polish" };

}

LkLocalization::LkLocalization()	:
	m_Locale(LOCALE_EN),
	m_StringMissing(std::string("STRING_MISSING"))
{
	
}

LkLocalization::~LkLocalization()
{
	
}

bool LkLocalization::LoadLocalizationTable( const std::string& _File )
{
	util::LkCSVParser::LkCSVDataTable* file = util::g_CSVParser->ParseFile(_File.c_str());
	if (!file)
	{
		LOG(VL_ERROR, "Localization::LoadLocalizationTable: Could not load file");
		return false;
	}

	unsigned int numrows = file->GetNumRows();
	for (unsigned int i = 0; i < numrows; ++i)
	{
		unsigned int numcolumns = file->GetNumColumnsAtRow(i);

		// Obtain the first entry on this row, this is the string name. The other entries are localized strings representing it.
		std::string stringname;
		if (!file->GetDataAt(0, i, stringname))
		{
			LOG(VL_ERROR, "Localization::LoadLocalizationTable: Unable to retrieve string name at row %i. Skipping to next row", i);
			continue;
		}
		unsigned int hash = HASH(stringname.c_str());
		m_Strings.insert(StringMapPair(hash, LocalizedStrings(numcolumns)));

		// Obtain the localized strings.
		for (unsigned int j = 1; j < numcolumns && j < LOCALE_COUNT; ++j)
		{
			std::string output;
			if (!file->GetDataAt(j, i, output))
			{
				LOG(VL_ERROR, "Localization::LoadLocalizationTable: Unable to retrieve data from table at [%i, %i] for string name '%s'", j, i, stringname.c_str());
				output = "STRING_RETRIEVE_ERROR";
			}
			m_Strings[hash][j - 1] = output;
		}
	}

	return true;
}

void LkLocalization::SetLocale( ELocale _Locale )
{
	assert(_Locale < LOCALE_COUNT);

	LOG(VL_NORMAL, "Localization::SetLocale: Changed locale to %s", LocaleStrings[_Locale]);
	m_Locale = _Locale;
}

const std::string& LkLocalization::GetLocalizedString( const std::string& _String )
{
	return GetLocalizedStringForLocale(_String, m_Locale);
}

const std::string& LkLocalization::GetLocalizedStringForLocale( const std::string& _String, ELocale _Locale )
{
	unsigned int hash = HASH(_String.c_str());
	StringMap::iterator it = m_Strings.find(hash);
	if (it == m_Strings.end())
	{
#ifdef LOCALIZATION_RETURN_EXPLICIT_ERRORSTRING_ON_MISSING
		return m_StringMissing;
#else
		return _String;
#endif
	}
	return it->second[_Locale];
}

}

}

#ifdef LOCALIZATION_RETURN_EXPLICIT_ERRORSTRING_ON_MISSING
#undef LOCALIZATION_RETURN_EXPLICIT_ERRORSTRING_ON_MISSING
#endif
