#pragma once

#ifndef UTIL_H
#define UTIL_H

#include <list>
#include <hash_map>
#include <map>
#include <vector>
#include <string>
#include "Types.h"

#define BYTE_TO_KB(b)	(b / 1024)
#define BYTE_TO_MB(b)	(b / 1048576)
#define BYTE_TO_GB(b)	(b / 1073741824)
#define BYTE_TO_TB(b)	(b / 1099511627776)

//#define min(a, b)		((a > b) ? (b) : (a))
//#define max(a, b)		((a < b) ? (b) : (a))
//#define clamp(a, b, c)	((b < a) ? (a) : ((b > c) ? (c) : (b)))

//////////////////////////////////////////////////////////////////////////
// To be used within c-tor that shouldn't be called ever. For example, a class that has a default c-tor
// that shouldn't ever be called by anyone (But is still defined so that we can handle it's behavior
// ourselves). Logs an error message with the class name (which should be specified as argument 'c').
//////////////////////////////////////////////////////////////////////////
#define ILLEGAL_CTOR_ERROR(c)	{LOG(VL_ERROR, "class "c" instantiated with illegal c-tor"); __debugbreak();}

//////////////////////////////////////////////////////////////////////////
// To be used within initalization functions for classes.
// When initialization fails, this can be used to throw an assertion error.
//////////////////////////////////////////////////////////////////////////
#define INIT_FAIL(classname)		{util::system::MessageBoxNotify(classname##": Failed to initialize (Check log for error messages)", "Initialization error"); __debugbreak();}

//////////////////////////////////////////////////////////////////////////
// Tests whether a c-string (char*) is empty by checking whether the first 
// element is equal to a null-termating character (\0).
//////////////////////////////////////////////////////////////////////////
#define CSTRING_ISEMPTY(str)	(str[0] == '\0')

//#define SETCONSOLETEXTCOLOR(c)		{static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hConsole, c);}
#define CONSOLETEXTCOLOR_BLACK		0
#define CONSOLETEXTCOLOR_GREYLIGHT	7
#define CONSOLETEXTCOLOR_GREYDARK	8
#define CONSOLETEXTCOLOR_BLUE		9
#define CONSOLETEXTCOLOR_GREEN		10
#define CONSOLETEXTCOLOR_TURQOISE	11
#define CONSOLETEXTCOLOR_RED		12
#define CONSOLETEXTCOLOR_PINK		13
#define CONSOLETEXTCOLOR_YELLOW		14
#define CONSOLETEXTCOLOR_WHITE		15

#define CONTAINER_MACRO_HASH_MAP( _KeyType, _ValueType, name )		 typedef stdext::hash_map<_KeyType, _ValueType>		name;				\
																	 typedef std::pair<_KeyType, _ValueType>			name ## Pair;		\
																	 typedef name::iterator								name ## Iter;		\
																	 typedef name::const_iterator						name ## ConstIter;	\
																	 typedef name::reverse_iterator						name ## RIter;		\
																	 typedef name::const_reverse_iterator				name ## ConstRIter;

#define CONTAINER_MACRO_MAP( _KeyType, _ValueType, name )			 typedef std::map<_KeyType, _ValueType>		name;				\
																	 typedef std::pair<_KeyType, _ValueType>			name ## Pair;		\
																	 typedef name::iterator								name ## Iter;		\
																	 typedef name::const_iterator						name ## ConstIter;	\
																	 typedef name::reverse_iterator						name ## RIter;		\
																	 typedef name::const_reverse_iterator				name ## ConstRIter;

#define CONTAINER_MACRO_LIST( _ValueType, name )					 typedef std::list<_ValueType>						name;				\
																	 typedef name::iterator								name ## Iter;		\
																	 typedef name::const_iterator						name ## ConstIter;	\
																	 typedef name::reverse_iterator						name ## RIter;		\
																	 typedef name::const_reverse_iterator				name ## ConstRIter;

#define CONTAINER_MACRO_VECTOR( _ValueType, name )					 typedef std::vector<_ValueType>					name;				\
																	 typedef name::iterator								name ## Iter;		\
																	 typedef name::const_iterator						name ## ConstIter;	\
																	 typedef name::reverse_iterator						name ## RIter;		\
																	 typedef name::const_reverse_iterator				name ## ConstRIter;

namespace loki
{

namespace util
{

namespace general
{
	/*
	Removes each element in an std::list and deletes each element.
	This function works by utilizing the remove_if() method of the
	std::list class. The function DeleteAll (class ClassDeleteAll, 
	defined in util.inline.h) is passed to this method, 
	which always returns true and thereby ensures that each element
	in the list is removed. The function also calls delete on each 
	element to ensure that the resources used by it are cleaned up.
*/
template< class T >
static void CleanSTLList( std::list<T>* _List );

}

namespace strings
{

	//////////////////////////////////////////////////////////////////////////
	// Converts a multibyte string to a wide string 
	// (Which contains the wchar_t type).
	//////////////////////////////////////////////////////////////////////////
	std::wstring ToWideString(const std::string& str);

	//////////////////////////////////////////////////////////////////////////
	// Converts a wide string to a multibyte string
	// (Which contains the char type).
	//////////////////////////////////////////////////////////////////////////
	std::string ToMultiByteString(const std::wstring& wstr);

	//////////////////////////////////////////////////////////////////////////
	// StringReplace replaces the an occurrence of _From with _To, _Index 
	// specifies which occurence. By default this is the first occurence.
	// StringReplace returns true if something was replaced, false otherwise.
	// StringReplaceAll replaces all occurrences of _From with _To.
	//////////////////////////////////////////////////////////////////////////
	bool StringReplace( std::string& _String, const std::string& _From, const std::string& _To, int32 _Index = 0 );
	void StringReplaceAll( std::string& _String, const std::string& _From, const std::string& _To );

	//////////////////////////////////////////////////////////////////////////
	// Function provides functionality similar to boost::lexical_cast.
	//////////////////////////////////////////////////////////////////////////
	template< typename _T >
	std::string LexicalCast( _T _Argument );
}

namespace system
{
	//////////////////////////////////////////////////////////////////////////
	// Sleeps the calling thread for _ms number of milliseconds.
	// Can be implemented differently per platform.
	//////////////////////////////////////////////////////////////////////////
	inline void Sleep( uint32 _ms );

	//////////////////////////////////////////////////////////////////////////
	// Presents the user with a dialog box with an 'OK' button.
	//////////////////////////////////////////////////////////////////////////
	void MessageBoxNotify( const char* _Text, const char* _Caption = NULL );

	//////////////////////////////////////////////////////////////////////////
	// Presents the user with a dialog box asking for confirmation.
	// The _CanCanel argument specifies whether the user can press a 'cancel'
	// button in addition to 'yes' and 'no' buttons.
	// The functions returns an integer value depending on which button is
	// pressed:
	// -1 = 'cancel' (CONFIRMATION_CANCEL define)
	// 0 = 'no'		 (CONFIRMATION_NO define)
	// 1 = 'yes'	 (CONFIRMATION_YES define)
	//////////////////////////////////////////////////////////////////////////
#define CONFIRMATION_CANCEL	-1
#define CONFIRMATION_NO		0
#define CONFIRMATION_YES	1
	int32 MessageBoxConfirmation( const char* _Text, const char* _Caption = NULL, bool _CanCancel = false );

#ifdef _WIN32

	//////////////////////////////////////////////////////////////////////////
	// File opening/saving functions.
	//////////////////////////////////////////////////////////////////////////
// Some example filter strings. These can be combined simply by stringing (forgive the pun) them together
// separated by nothing else than a good ol' space.
#define OPENFILE_FILTER_ALLFILES	"All Files (*.*)\0*.*\0"
#define OPENFILE_FILTER_TXTFILES	"Text Files (*.txt)\0*.txt\0"
#define OPENFILE_FILTER_LMAFILES	"Loki Material Files (*.lma)\0*.lma\0"
#define OPENFILE_FILTER_LMOFILES	"Loki Model Files (*.lmo)\0*.lmo\0"
#define OPENFILE_FILTER_HELPER(DescriptionString, ExtensionString)	DescriptionString" (*."ExtensionString")\0*."ExtensionString"\0"

	bool OpenFileDialog( const char* _Filter, std::string& _Output );
	bool SaveFileDialog( const char* _Filter, std::string& _Output );

#endif
}

namespace time
{


	//////////////////////////////////////////////////////////////////////////
	// Stores a formatted time stamp in _Output.
	// Format: [DD/MM//YY hh:mm:ss]
	//////////////////////////////////////////////////////////////////////////
	void GetTimeStamp( std::string& _Output );

}

}	// Namespace util.

}	// Namespace loki.

#include "util.inline.h"

#endif