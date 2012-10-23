//#include "core/filesystem/filesystem.h"	// Precompiled header takes care of this.

namespace loki
{

namespace filesystem
{

LkFile::LkFile()	:
	m_File(NULL),
	m_Path(NULL),
	m_Buffer(NULL),
	m_BufferSize(0)
{

}

LkFile::LkFile( const char* _File )	:
	m_File(NULL),
	m_Path(NULL),
	m_Buffer(NULL),
	m_BufferSize(0)
{
	fopen_s(&m_File, _File, "r");
	if (m_File)
	{
		int32 pathlength = strlen(_File);
		m_Path = new char[pathlength + 1];
		memcpy(m_Path, _File, pathlength);
		m_Path[pathlength] = '\0';
	}
}

LkFile::~LkFile()
{
	Close();
}

bool LkFile::IsOpen()
{
	return (m_File != NULL);
}

void LkFile::Close()
{
	fclose(m_File);
	
	// Free the buffer.
	delete[] m_Buffer;
	m_Buffer = NULL;
}

const char* const LkFile::GetPath()
{
	return m_Path;
}

char* LkFile::GetBuffer()
{
	if (!IsOpen())
	{
		return NULL;
	}

	// If the buffer has not yet been copied, copy it to m_Buffer.
	if (!m_Buffer)
	{
		fseek(m_File, 0, SEEK_END);
		long count = ftell(m_File);	// Returns incorrect results on some occasions.
		rewind(m_File);

		m_BufferSize = count + 1;
		m_Buffer = new char[m_BufferSize + 1];	// Allocate space.
		memset(m_Buffer, 0, m_BufferSize + 1);		// Fill the allocated with 0's.
		fread_s(m_Buffer, m_BufferSize + 1, 1, m_BufferSize, m_File);	// Read the contents into the buffer.
	}	

	return m_Buffer;
}

unsigned long LkFile::GetBufferSize()
{
	return m_BufferSize;
}

NativeFilePointer LkFile::GetNativeFilePointer()
{
	return m_File;
}

LkFile* OpenFile( const char* _File )
{
	if (!_File)
	{
		LOG(VL_WARN, "OpenFile: File path is empty");
		return NULL;
	}

	LkFile* file = new LkFile(_File);
	return file;
}

LkFile* OpenFile( const std::string& _File )
{
	return OpenFile(_File.c_str());
}

bool CreateDirectory( const char* _Directory )
{
	bool res = (bool)::CreateDirectoryA(_Directory, NULL);

	if (!res)
	{
		DWORD err = GetLastError();

		switch (err)
		{
		case ERROR_ALREADY_EXISTS:
			{
				LOG(VL_WARN, "CreateDirectory: Directory already exists");
				res = true;
				break;
			}
		case ERROR_PATH_NOT_FOUND:
			{
				LOG(VL_ERROR, "CreateDirectory: Path could not be found. Do all intermediate directories exist? Directory was not created");
				break;
			}
		}
	}
	
	return res;
}

void FindFiles( const char* _SearchString, std::list<std::string>& _List )
{
	// Clear the list.
	_List.clear();

	if (_SearchString == NULL || CSTRING_ISEMPTY(_SearchString))
	{
		LOG(VL_NORMAL, "FindFiles: Search string is empty or NULL");
		return;
	}

	WIN32_FIND_DATAA data;
	HANDLE searchhandle = FindFirstFileA(_SearchString, &data);
	DWORD err;

	err = GetLastError();

	if (err == ERROR_FILE_NOT_FOUND)
	{
		LOG(VL_NORMAL, "FindFiles: No matches found for '%s'", _SearchString);
	}

	_List.push_back(std::string(data.cFileName));

	do
	{
		if (FindNextFileA(searchhandle, &data))
		{
			_List.push_back(std::string(data.cFileName));
		}

		err = GetLastError();
	}
	while (err != ERROR_NO_MORE_FILES);
}

}	// Namespace filesystem.

}	// Namespace loki.