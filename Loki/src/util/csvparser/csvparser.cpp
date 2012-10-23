#include "util/csvparser/csvparser.h"
#include <fstream>

namespace loki
{

namespace util
{

LkCSVParser* g_CSVParser = new LkCSVParser();

LkCSVParser::LkCSVParser()
{

}

LkCSVParser::~LkCSVParser()
{

}

LkCSVParser::LkCSVDataTable* LkCSVParser::ParseFile( const char* _File )
{
	std::ifstream file = std::ifstream(_File);
	if (!file.is_open())
	{
		// Failed to open file.
		LOG(VL_ERROR, "CSVParser::ParseFile: Could not open file '%s'", _File);
		return NULL;
	}
	LkCSVDataTable* table = new LkCSVDataTable();
	std::string str;
	std::string value;
	uint32 row = 0;
	uint32 column = 0;
	while (getline(file, str))
	{
		table->m_Data.push_back(std::vector<std::string>());
		std::vector<std::string>* list = &(*table->m_Data.rbegin());
		for (uint32 i = 0; i < str.length(); ++i)
		{
			if (str[i] == ',')
			{
				// Separator found.
				list->push_back(value);
				value.clear();
			}
			else
			{
				value += str[i];
			}
		}
		list->push_back(value);
		value.clear();
	}
	file.close();
	return table;
}

LkCSVParser::LkCSVDataTable::LkCSVDataTable()
{

}

LkCSVParser::LkCSVDataTable::~LkCSVDataTable()
{

}

bool LkCSVParser::LkCSVDataTable::GetDataAt( uint32 _Column, uint32 _Row, std::string& _Output )
{
	if (m_Data.size() > _Row && m_Data[_Row].size() > _Column)
	{
		_Output = m_Data[_Row][_Column];
		return true;
	}
	return false;
}

void LkCSVParser::LkCSVDataTable::SetDataAt( uint32 _Column, uint32 _Row, std::string& _Data )
{
	// Resize the number of rows if required.
	if (_Row > m_Data.size())
	{
		m_Data.resize(_Row + 1);
	}

	// Resize the number of columns if required.
	if (_Column > m_Data[_Row].size())
	{
		m_Data[_Row].resize(_Column + 1);
	}

	// Store the data.
	m_Data[_Row][_Column] = _Data;
}

uint32 LkCSVParser::LkCSVDataTable::GetNumRows()
{
	return m_Data.size();
}

uint32 LkCSVParser::LkCSVDataTable::GetNumColumnsAtRow( uint32 _Row )
{
	assert(_Row < m_Data.size());

	return m_Data[_Row].size();
}

uint32 LkCSVParser::LkCSVDataTable::GetNumEntries()
{
	uint32 entries = 0;
	uint32 rows = GetNumRows();
	for (uint32 i = 0; i < rows; ++i)
	{
		entries += GetNumColumnsAtRow(i);
	}
	return entries;
}

void LkCSVParser::LkCSVDataTable::Clear()
{
	for (std::vector<std::vector<std::string>>::iterator it = m_Data.begin(); it != m_Data.end(); ++it)
	{
		(*it).clear();
	}
	m_Data.clear();
}

bool LkCSVParser::LkCSVDataTable::IsEmpty()
{
	// Zero-sized lists might be present, which is why we need to 
	// check all lists (if there are any). We can return false if 
	// any of these lists has a size greater than 0.
	if (m_Data.size() > 0)
	{
		for (std::vector<std::vector<std::string>>::iterator it = m_Data.begin(); it != m_Data.end(); ++it)
		{
			if ((*it).size() > 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool LkCSVParser::LkCSVDataTable::Export( const char* _File, bool _AllowIfEmpty /*= false*/ )
{
	if (!_AllowIfEmpty && IsEmpty())
	{
		return false;
	}

	std::ofstream file = std::ofstream(_File);
	if (!file.is_open())
	{
		// Failed to open/create file.
		LOG(VL_ERROR, "CSVDataTable::Export: Could not open/create output file '%s'", _File);
		return false;
	}
	file.clear();	// Empty the file.
	for (std::vector<std::vector<std::string>>::iterator it = m_Data.begin(); it != m_Data.end(); ++it)
	{
		bool b = false;
		for (std::vector<std::string>::iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			if (b)
				file << ',';
			else
				b = true;

			file << (*it2);
		}
		file << '\n';
	}
	file.close();
	return true;
}

}

}