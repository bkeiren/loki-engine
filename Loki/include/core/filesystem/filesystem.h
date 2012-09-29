#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <string.h>

// This typedef is here to allow clients to write
// code that utilizes a file pointer native to the OS
// with great ease because the only thing that needs to be
// changed for a different OS is this typedef.
typedef FILE*	NativeFilePointer;

namespace loki
{

namespace filesystem
{

class LkFile
{
public:
	bool IsOpen();
	void Close();
	const char* const GetPath();
	char* GetBuffer();
	unsigned long GetBufferSize();

	//////////////////////////////////////////////////////////////////////////
	// Returns a file pointer native to the OS.
	// Should be implemented differently for every OS.
	//////////////////////////////////////////////////////////////////////////
	NativeFilePointer GetNativeFilePointer();

	friend LkFile* OpenFile( const char* _File );
	friend LkFile* OpenFile( const std::string& _File );
private:
	LkFile();
	LkFile( const char* _File );
	~LkFile();

	FILE* m_File;
	char* m_Path;
	char* m_Buffer;
	unsigned long m_BufferSize;
};

//////////////////////////////////////////////////////////////////////////
// Attempts to open a file and returns a File*. If the specified file does
// not exist, an attempt is made to create the file.
// If all fails and the file could not be opened or created, File::IsOpen()
// will return false and can therefore be used to check whether the file
// has been opened.
//////////////////////////////////////////////////////////////////////////
LkFile* OpenFile( const char* _File );
LkFile* OpenFile( const std::string& _File );

//////////////////////////////////////////////////////////////////////////
// Attempts to create the specified directory. 
// NOTE: Only the last folder in the path will be created, if any 
// intermediate directory does not exist, the function will fail.
// NOTE: If the directory already exists, the function will log a warning
// but will return true (because it is assumed that the goal of a call
// is to create a directory, and the directory WILL have been created).
//////////////////////////////////////////////////////////////////////////
bool CreateDirectory( const char* _Directory );

//////////////////////////////////////////////////////////////////////////
// Finds all files or subdirectories matching _SearchString (can contain
// wildcards such as * (asterisk)). Matches are stored in _List.
// NOTE: _List is cleared by this function.
// NOTE: If you wish to list all files within a directory, you can specify
// the directory and append '/*'.
//////////////////////////////////////////////////////////////////////////
void FindFiles( const char* _SearchString, std::list<std::string>& _List );

}

}


#endif	// FILESYSTEM_H