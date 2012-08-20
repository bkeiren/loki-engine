#pragma once

#ifndef LOGGER_H
#define LOGGER_H

// These includes are here for convenience, so that clients need only include logger.h in order to be able to use it.
#include <list>
#include <string>

#include <Windows.h>

// To make it easier to log things, this define allows the user to make logging calls by using LOG().
#define LOG(verbosity, format, ...)		loki::g_Logger->Log(verbosity, format, __VA_ARGS__)

namespace loki
{

enum VerbosityLevel
{
	VL_NORMAL = 0,
	VL_WARN,
	VL_ERROR,
	//VL_EXCEPTION,	// Does not do anything special currently.
	VL_COMMAND,	// Special type that is only to be used by the console in order to log console commands.

	VL_ALWAYS	// Keep as last. Messages with this verbosity level will always be logged, 
				// regardless of the current verbosity level.
};

class LkLogger
{
public:
	LkLogger();
	virtual ~LkLogger();

	//////////////////////////////////////////////////////////////////////////
	//	Logs a message. If the priority level is set to VL_WARN, prepends a warning
	//	keyword, if the level is VL_ERROR, prepends an error keyword, if the level is VL_EXCEPTION
	//	prepends an exception keyword and throws an exception.
	//////////////////////////////////////////////////////////////////////////
	void Log( const VerbosityLevel _Level, const char* _Message, va_list _ArgList );
	void Log( const VerbosityLevel _Level, const char* _Message, ... );

	//////////////////////////////////////////////////////////////////////////
	// Sets the current verbosity level. The verbosity level dictates which messages
	// actually get logged and which don't. The higher the verbosity level, the less messages are logged.
	// For example, if the verbosity level is set to VL_WARN, only warnings and errors will be logged.
	//////////////////////////////////////////////////////////////////////////
	void SetVerbosityLevel( const VerbosityLevel _VerbosityLevel );

	//////////////////////////////////////////////////////////////////////////
	// Writes the contents of the log buffer to a specified file and flushes (empties) the buffer afterwards.
	// Parameter _Append determines whether the contents of the buffer are appended to whatever is in
	// the file already, or whether the file is cleaned first before dumping the log buffer.		
	//////////////////////////////////////////////////////////////////////////
	void FlushToFile( const char* _File = DefaultLogFile, bool _Append = true );

	//////////////////////////////////////////////////////////////////////////
	// Flushes (empties) the log buffer.
	//////////////////////////////////////////////////////////////////////////
	void Flush();

	//////////////////////////////////////////////////////////////////////////
	// Writes the contents of the log buffer to a specified file. Does NOT flush (empty) the log buffer.
	// Parameter _Append determines whether the contents of the buffer are appended to whatever is in
	// the file already, or whether the file is cleaned first before dumping the log buffer.
	//////////////////////////////////////////////////////////////////////////
	void DumpToFile( const char* _File = DefaultLogFile, bool _Append = true );

	//////////////////////////////////////////////////////////////////////////
	// Returns the current size of the log buffer in bytes.
	//////////////////////////////////////////////////////////////////////////
	unsigned int GetBufferSize();

	//////////////////////////////////////////////////////////////////////////
	// Returns the number of messages that the logger has received.
	//////////////////////////////////////////////////////////////////////////
	unsigned int GetNumMessages();

	//////////////////////////////////////////////////////////////////////////
	// Static strings that can be used to easily access default log files are defined here.
	// These are public so that clients can use them.
	//////////////////////////////////////////////////////////////////////////
	static const char* DefaultLogFile;
	static const char* DefaultErrorLogFile;
	static const char* DefaultWarningLogFile;
private:
	void _Init();
	void _Shutdown();

	static const char* m_WarningPrefix;
	static const char* m_ErrorPrefix;
	static const char* m_CommandPrefix;
	static const char* m_DumpSeperator;
	VerbosityLevel m_CurrentVerbosityLevel;

	std::list<std::string>* m_Buffer;

	unsigned int m_BufferSize;

	CRITICAL_SECTION m_CriticalSection;
};

extern LkLogger* g_Logger;

}

#endif