#pragma once

#ifndef JSONPARSER_H
#define JSONPARSER_H

//////////////////////////////////////////////////////////////////////////
// NOTE: Include json.h, not this file.
//////////////////////////////////////////////////////////////////////////

namespace loki
{

namespace util
{

#define JSON_CLOSE( doc )	( doc->Close(); doc = 0; )

class JSONDocument
{
public:
	enum EJSONOutputType
	{
		EJO_HUMANREADABLE = 0,	// Multi-line, intended for humans to read.
		EJO_LIGHTWEIGHT			// Single-line, preserves space. Can be useful for JSON data intended to be sent over a network,
								// when bandwidth might be limited.
	};

	//////////////////////////////////////////////////////////////////////////
	// Opens a JSON file. Once opened, the caller has to manage the lifetime
	// of the JSONDocument object by calling Close() on it when it's no 
	// longer needed.
	// Returns 0 when unsuccessful.
	//////////////////////////////////////////////////////////////////////////
	static JSONDocument* Open( const std::string& _File );

	//////////////////////////////////////////////////////////////////////////
	// Deletes the JSON document instance. Pointers to the instance are no 
	// longer valid after this function is called.
	//////////////////////////////////////////////////////////////////////////
	void Close();

	//////////////////////////////////////////////////////////////////////////
	// Returns whether the file was properly opened.
	//////////////////////////////////////////////////////////////////////////
	bool IsOpen() const;

	//////////////////////////////////////////////////////////////////////////
	// Returns the root value. From this value you can extract others.
	//////////////////////////////////////////////////////////////////////////
	JSONValue& GetRoot() const;

	void WriteToString( std::string& _Output, EJSONOutputType _OutputType = EJO_HUMANREADABLE ) const;
private:
	JSONDocument( const std::string& _File );
	~JSONDocument();

	bool m_IsOpen;
	std::string m_File;
	Json::Value m_JSONRoot;
	static Json::Reader m_JSONReader;
	static Json::StyledWriter m_JSONStyledWriter;
	static Json::FastWriter m_JSONFastWriter;
	JSONValue* m_Root;
};

}

}

#endif