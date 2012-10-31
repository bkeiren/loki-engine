#include "JSONCpp/json.h"
#include "util/json/jsonvalue.h"
#include "util/json/jsondocument.h"

namespace loki
{

namespace util
{

namespace general
{

Json::Reader JSONDocument::m_JSONReader;
Json::StyledWriter JSONDocument::m_JSONStyledWriter;
Json::FastWriter JSONDocument::m_JSONFastWriter;

JSONDocument* JSONDocument::Open( const std::string& _File )
{
	JSONDocument* doc = new JSONDocument(_File);
	if (doc->IsOpen())
	{
		return doc;
	}
	delete doc;
	return 0;
}

JSONDocument::JSONDocument( const std::string& _File )	:
	m_IsOpen(false),
	m_File(_File),
	m_Root(0)
{
	filesystem::LkFile* file = filesystem::OpenFile(m_File);
	if (file->IsOpen())
	{
		char* buffer = file->GetBuffer();
		bool res = m_JSONReader.parse(buffer, buffer + file->GetBufferSize(), m_JSONRoot, true);
		if (!res)
		{
			LOG(VL_ERROR, "JSONDocument::JSONDocument: Failed to parse file '%s': %s", m_File.c_str(), m_JSONReader.getFormatedErrorMessages().c_str());
		}
		else
		{
			m_IsOpen = true;
			m_Root = new JSONValue(m_JSONRoot);
		}

		file->Close();
	}
}

JSONDocument::~JSONDocument()
{

}

void JSONDocument::Close()
{
	m_IsOpen = false;	// Even though the memory for the object is released after the call to delete,
						// the memory might still be accessed (erroneously) by clients before
						// the memory is actually re-used. In such case, m_IsOpen will be seen as being false
						// still so IsOpen() returns false.
	delete this;
}

bool JSONDocument::IsOpen() const
{
	return m_IsOpen;
}

JSONValue& JSONDocument::GetRoot() const
{
	return *m_Root;
}

void JSONDocument::WriteToString( std::string& _Output, EJSONOutputType _OutputType /*= EJO_HUMANREADABLE*/ ) const
{
	_Output.clear();

	switch (_OutputType)
	{
	case EJO_HUMANREADABLE:
		{
			_Output = m_JSONStyledWriter.write(m_JSONRoot);
			break;
		}
	case EJO_LIGHTWEIGHT:
		{
			_Output = m_JSONFastWriter.write(m_JSONRoot);
			break;
		}
	}
}

}

}

}