#include "core/logger.h"	// Even though our precompiled header already includes this file (And that header is #force included),
							// we need to include it here because we're using an extern in that header file.
#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <ctime>
#include "core/console/console.h"

namespace loki
{

LkLogger* g_Logger = NULL;

//////////////////////////////////////////////////////////////////////////
// Initialize static member data.
//////////////////////////////////////////////////////////////////////////
const char* LkLogger::DefaultLogFile = "logs\\log.log";
const char* LkLogger::DefaultErrorLogFile = "logs\\errors.log";
const char* LkLogger::DefaultWarningLogFile = "logs\\warnings.log";
const char* LkLogger::m_WarningPrefix = "WARNING ";
const char* LkLogger::m_ErrorPrefix = "ERROR ";
const char* LkLogger::m_CommandPrefix = ">> ";
const char* LkLogger::m_DumpSeperator = "^ LOG DUMP---------------------------------------------------------------------";
// VerbosityLevel Logger::m_CurrentVerbosityLevel = VL_NORMAL;
// std::list<std::string>* Logger::m_Buffer = NULL;
// uint32 Logger::m_BufferSize = 0;
// CRITICAL_SECTION Logger::m_CriticalSection;

//////////////////////////////////////////////////////////////////////////
// C-tor.
//////////////////////////////////////////////////////////////////////////
LkLogger::LkLogger()	:
	m_CurrentVerbosityLevel(VL_NORMAL),
	m_Buffer(NULL),
	m_BufferSize(0)
{
	_Init();
}

//////////////////////////////////////////////////////////////////////////
// D-tor.
//////////////////////////////////////////////////////////////////////////
LkLogger::~LkLogger()
{
	_Shutdown();
}

void LkLogger::_Init()
{
	m_Buffer = new std::list<std::string>();
	InitializeCriticalSection(&m_CriticalSection);

	/*
	// Create a default log file if non exists.
	std::ofstream file;
	
	file.open(DefaultLogFile, std::ios::out);
	if (!file.is_open())
	{
		LOG(VL_ERROR, "Logger::Init: Could not open default log file ('%s')", DefaultLogFile);
	}
	file.close();

	file.open(DefaultWarningLogFile, std::ios::out);
	if (!file.is_open())
	{
		LOG(VL_ERROR, "Logger::Init: Could not open default warning logo file ('%s')", DefaultWarningLogFile);
	}
	file.close();

	file.open(DefaultErrorLogFile, std::ios::out);
	if (!file.is_open())
	{
		LOG(VL_ERROR, "Logger::Init: Could not open default error log file ('%s')", DefaultErrorLogFile);
	}
	file.close();
	*/
}

void LkLogger::_Shutdown()
{
	// Dump the final logging data to disk.
	LkLogger::DumpToFile(LkLogger::DefaultLogFile, false);

	DeleteCriticalSection(&m_CriticalSection);
}

void LkLogger::Log( const VerbosityLevel _Level, const char* _Message, ... )
{
	va_list v1;
	va_start(v1, _Message);
	Log(_Level, _Message, v1);
	va_end(v1);
}

//////////////////////////////////////////////////////////////////////////
// Logs a message with the specified priority level. A newline
// character ('\n') is appended to the end of the message.
//////////////////////////////////////////////////////////////////////////
void LkLogger::Log( const VerbosityLevel _Level, const char* _Message, va_list _ArgList )
{
	static const int32 msg_buffer_size = 2048;
	/*static*/ char msg_buffer[msg_buffer_size];

	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Only log the message if it is at an equal or higher verbosity level as the current verbosity level of the logger.
	if (_Level >= m_CurrentVerbosityLevel)
	{
		// vsnprintf() parses a string and replaces special symbols with their counterparts from the argument list.
		vsnprintf_s(msg_buffer, msg_buffer_size, msg_buffer_size - 1, _Message, _ArgList);

		std::string str("");

		std::string time_and_date;
		util::GetTimeStamp(time_and_date);

		str += time_and_date;

		EnterCriticalSection(&m_CriticalSection);
		switch (_Level)
		{
		case VL_WARN:
			{
				str += m_WarningPrefix;

				//SETCONSOLETEXTCOLOR(CONSOLETEXTCOLOR_YELLOW);
				SetConsoleTextAttribute(hConsole, CONSOLETEXTCOLOR_YELLOW);
				// TODO: Remove these temporary prints to the console?
#ifdef _DEBUG
				printf(m_WarningPrefix);
#endif
				break;
			}
		case VL_ERROR:
			{
				str += m_ErrorPrefix;
				
				//SETCONSOLETEXTCOLOR(CONSOLETEXTCOLOR_RED);
				SetConsoleTextAttribute(hConsole, CONSOLETEXTCOLOR_RED);
				// TODO: Remove these temporary prints to the console?
#ifdef _DEBUG
				printf(m_ErrorPrefix);
#endif
				break;
			}
		case VL_COMMAND:
			{
				str += m_CommandPrefix;

				//SETCONSOLETEXTCOLOR(CONSOLETEXTCOLOR_TURQOISE);
				SetConsoleTextAttribute(hConsole, CONSOLETEXTCOLOR_TURQOISE);
				// TODO: Remove these temporary prints to the console?
#ifdef _DEBUG
				printf(m_CommandPrefix);
#endif
				break;
			}
		case VL_NORMAL:
			{
				//SETCONSOLETEXTCOLOR(CONSOLETEXTCOLOR_GREYLIGHT);
				SetConsoleTextAttribute(hConsole, CONSOLETEXTCOLOR_GREYLIGHT);
				break;
			}
		case VL_ALWAYS:
		default:
			{
				SetConsoleTextAttribute(hConsole, CONSOLETEXTCOLOR_GREYLIGHT);
				break;
			}
		}

		str += msg_buffer;

		// TODO!!!: Change the Logger class so that messages are pushed to a queue
		// that a separate thread picks messages in order to log them properly.
		// That approach should be less stalling (Although messages might not be logged
		// at the actual time that the log calls are made).
		m_Buffer->push_back(str);
		m_BufferSize += str.length();

		g_Console->Print(msg_buffer);

		// TODO: Remove these temporary prints to the console (?).
#ifdef _DEBUG
		printf(msg_buffer);
		printf("\n");
#endif
		LeaveCriticalSection(&m_CriticalSection);
	}
}

//////////////////////////////////////////////////////////////////////////
// Sets the current verbosity level. The verbosity level dictates which 
// messages actually get logged and which don't. 
// The higher the verbosity level, the less messages are logged.
// For example, if the verbosity level is set to WARN, only warnings, 
// errors and exceptions will be logged.
// NOTE: Do NOT use VL_ALWAYS here, as it is only used to indicate messages
// that may always be logged. Passing VL_ALWAYS to this function will not
// set the verbosity level to this, but will print a warning indicating that
// this occurred.
//////////////////////////////////////////////////////////////////////////
void LkLogger::SetVerbosityLevel( const VerbosityLevel _VerbosityLevel )
{
	if (_VerbosityLevel != VL_ALWAYS)
	{
		m_CurrentVerbosityLevel = _VerbosityLevel;
	}
	else
	{
		LOG(VL_WARN, "Logger::SetVerbosityLevel: VL_ALWAYS can only be used for messages, not as message filter. Verbosity level has not changed.");
	}
}

//////////////////////////////////////////////////////////////////////////
// Writes the contents of the log buffer to a specified file and flushes 
// (empties) the buffer afterwards. Parameter _Append determines whether 
// the contents of the buffer are appended to whatever is in the file 
// already, or whether the file is cleaned first before dumping 
// the log buffer.
//////////////////////////////////////////////////////////////////////////
void LkLogger::FlushToFile( const char* _File /*= DefaultLogFile*/, bool _Append /*= true*/ )
{
	DumpToFile(_File, _Append);
	Flush();
}

//////////////////////////////////////////////////////////////////////////
// Flushes (empties) the log buffer.
//////////////////////////////////////////////////////////////////////////
void LkLogger::Flush()
{
	LOG(VL_NORMAL, "Logger::Flush: Flushing log buffer.");

	CleanSTLList(m_Buffer);

	m_BufferSize = 0;
}

//////////////////////////////////////////////////////////////////////////
// Writes the contents of the log buffer to a specified file. 
// Does NOT flush (empty) the log buffer. Parameter _Append determines 
// whether the contents of the buffer are appended to whatever is in the 
// file already, or whether the file is cleaned first before dumping 
// the log buffer.
//////////////////////////////////////////////////////////////////////////
void LkLogger::DumpToFile( const char* _File /*= DefaultLogFile*/, bool _Append /*= true*/ )
{
	std::ofstream outputStream;
	outputStream.open(_File, std::ios::out | ((_Append)?(std::ios::app):(std::ios::trunc)) );

	// If the file has not been opened yet, try creating it.
	if (!outputStream.is_open())
	{
		outputStream.close();
		outputStream.open(_File, std::ios::out);
	}

	if (outputStream.is_open())
	{
		LOG(VL_NORMAL, "Logger::DumpToFile: Dumping log buffer to file '%s' (%u bytes)", _File, GetBufferSize());

		// TODO: Find out why the std::ios::trunc flag does not ensure that the log file is cleared before writing
		// and then remove this check and call to clear(). (As this is just a workaround).
		if (!_Append)
		{
			outputStream.clear();
		}
		else
		{
			// If the current buffer data must be appended to the log file (as opposed to 
			// clearing the file before dumping), add a line of dashes to separate between dumps.
			LOG(VL_ALWAYS, LkLogger::m_DumpSeperator);
		}

		for (std::list<std::string>::iterator str_it = m_Buffer->begin(); str_it != m_Buffer->end(); ++str_it)
		{
			outputStream << (*str_it).c_str() << '\n';
		}

		long begin, end;
		outputStream.seekp(0);
		begin = outputStream.tellp();
		outputStream.seekp(0, std::ios::end);
		end = outputStream.tellp();
		LOG(VL_NORMAL, "Logger::DumpToFile: Log file size ('%s'): %li bytes", _File, (end - begin));

		outputStream.close();
	}
	else
	{
		LOG(VL_WARN, "Logger::DumpToFile: Could not open log file '%s' for writing", _File);
	}
}

//////////////////////////////////////////////////////////////////////////
// Returns the current size of the log buffer in bytes.
//////////////////////////////////////////////////////////////////////////
uint32 LkLogger::GetBufferSize()
{
	return m_BufferSize;	
}

//////////////////////////////////////////////////////////////////////////
// Returns the number of messages that the logger has received.
//////////////////////////////////////////////////////////////////////////
uint32 LkLogger::GetNumMessages()
{
	return m_Buffer->size();
}

}