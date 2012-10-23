#include "util/util.h"
#include "core/engine.h"
#include <Windows.h>
#include "core/window.h"

#include <sstream>

namespace loki
{

namespace util
{

std::wstring ToWideString(const std::string& str)
{
	// First we need to know how large the wide-string buffer needs to be.
	// We can obtain this information by passing 0 as argument cchWideChar. According to
	// MSDN documentation, this will make MultiByteToWideChar return the required buffer size.
	int32 stringLength = MultiByteToWideChar(CP_UTF8/*CP_ACP*/, 0, str.data(), str.length(), 0, 0);

	// Create an appropriately sized buffer.
	std::wstring wstr(stringLength, 0);

	// Convert the string.
	MultiByteToWideChar(CP_UTF8/*CP_ACP*/, 0,  str.data(), str.length(), &wstr[0], stringLength);
	return wstr;
}

std::string ToMultiByteString(const std::wstring& wstr)
{
	// In order to know the required buffer size later on, we need to pass 0 as the argument cbMultiByte
	// to WideCharToMultiByte. According to MSDN documentation, this will result in WideCharToMultiByte
	// returning the required buffer size.
	int32 stringLength = WideCharToMultiByte(CP_UTF8/*CP_ACP*/, 0, wstr.data(), wstr.length(), 0, 0, 0, 0);

	// Create an appropiately sized buffer.
	std::string str(stringLength, 0);

	// Convert the string.
	WideCharToMultiByte(CP_UTF8/*CP_ACP*/, 0, wstr.data(), wstr.length(), &str[0], stringLength, 0, 0);
	return str;
}

void MessageBoxNotify( const char* _Text, const char* _Caption /*= NULL*/ )
{
	MessageBoxA(g_Engine->GetWindow()->GetHWND(), _Text, _Caption, MB_OK);
}

int32 MessageBoxConfirmation( const char* _Text, const char* _Caption /*= NULL*/, bool _CanCancel /*= false*/ )
{
	switch (MessageBoxA(g_Engine->GetWindow()->GetHWND(), _Text, _Caption, (_CanCancel)?(MB_YESNOCANCEL):(MB_YESNO) | MB_TASKMODAL))
	{
	case IDCANCEL:
		{
			return CONFIRMATION_CANCEL;
			break;
		}
	case IDNO:
		{
			return CONFIRMATION_NO;
			break;
		}
	case IDYES:
		{
			return CONFIRMATION_YES;
			break;
		}
	default:
		{
			assert("MessageBoxConfirmation: Something went wrong. MessageBoxA returned a value other than IDCANCEL, IDNO or IDYES." && 0);
			return CONFIRMATION_CANCEL;
			break;
		}
	}
}

void GetTimeStamp( std::string& _Output )
{
	/*static*/ time_t rawtime;
	/*static*/ tm LocalTime;
	time(&rawtime);
	localtime_s(&LocalTime, &rawtime);

	/*static*/ char time_and_date[64];

	sprintf_s(time_and_date, "[%02i/%02i/%02i %02i:%02i:%02i] ",	
				LocalTime.tm_mday,
				LocalTime.tm_mon + 1,	// + 1 because the month is counted from 0 to 11, 
				// so February would be 1 (while we want to have 2).
				LocalTime.tm_year - 100,	// - 100 because the year is counted from 1900, so 2011 would be 111.
				LocalTime.tm_hour,
				LocalTime.tm_min,
				LocalTime.tm_sec);

	_Output = time_and_date;
}

bool StringReplace( std::string& _String, const std::string& _From, const std::string& _To, int32 _Index /*= 0*/ )
{
	if(_From.empty())
	{
		return false;
	}
	size_t start_pos = 0;
	int32 idx = 0;
	while((start_pos = _String.find(_From, start_pos)) != std::string::npos) 
	{
		if (idx == _Index)
		{
			_String.replace(start_pos, _From.length(), _To);
			start_pos += _To.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
			return true;
		}
	}
	return false;
}

void StringReplaceAll( std::string& _String, const std::string& _From, const std::string& _To )
{
	if(_From.empty())
	{
		return;
	}
	size_t start_pos = 0;
	while((start_pos = _String.find(_From, start_pos)) != std::string::npos) 
	{
		_String.replace(start_pos, _From.length(), _To);
		start_pos += _To.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

}	// Namespace util.

}	// Namespace loki.