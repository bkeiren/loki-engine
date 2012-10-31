#pragma once

#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <map>
#include <vector>
#include <string>

namespace loki
{

namespace game
{

//////////////////////////////////////////////////////////////////////////
// If changes are made to this enumeration, LocaleStrings in 
// localization.cpp must also be altered accordingly.
//////////////////////////////////////////////////////////////////////////
enum ELocale
{
	LOCALE_EN = 0,
	LOCALE_NL,
	LOCALE_FR,
	LOCALE_GE,
	LOCALE_RU,
	LOCALE_SP,
	LOCALE_IT,
	LOCALE_NO,
	LOCALE_SW,
	LOCALE_DN,
	LOCALE_PL,

	LOCALE_COUNT	// Keep as last!
};

class LkLocalization
{
	friend class LokiEngine;

	typedef std::vector<std::string>					LocalizedStrings;
	typedef std::map<uint32, LocalizedStrings>	StringMap;
	typedef std::pair<uint32, LocalizedStrings>	StringMapPair;
public:
	//////////////////////////////////////////////////////////////////////////
	// Loads a localization table.
	//////////////////////////////////////////////////////////////////////////
	bool LoadLocalizationTable( const std::string& _File );

	//////////////////////////////////////////////////////////////////////////
	// Sets the current language to use when obtaining a localized string.
	//////////////////////////////////////////////////////////////////////////
	void SetLocale( ELocale _Locale );

	const std::string& GetLocalizedString( const std::string& _String );
	const std::string& GetLocalizedStringForLocale( const std::string& _String, ELocale _Locale );
private:
	LkLocalization();
	~LkLocalization();

	ELocale m_Locale;
	StringMap m_Strings;
	std::string m_StringMissing;
};

extern LkLocalization* g_Localization;

}

}

#endif