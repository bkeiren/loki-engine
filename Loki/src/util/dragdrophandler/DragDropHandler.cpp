#ifdef _WIN32

#include "util/dragdrophandler/DragDropHandler.h"

#ifdef _DEBUG
#define DRAGDROP_DEBUGINFO
#endif

namespace loki
{

namespace util
{

namespace system
{

std::deque<DragDropHandler::DroppedFileInfo*> DragDropHandler::m_Files;

DragDropHandler::DragDropHandler()
{

}

DragDropHandler::~DragDropHandler()
{

}

#define QUERYHELPER(PopFunction)	if (GetNumFiles() <= 0)								\
									{													\
										return false;									\
									}													\
									DroppedFileInfo* Info = m_Files.back();				\
									_Output.m_File = Info->m_File;						\
									_Output.m_CursorPosition = Info->m_CursorPosition;	\
									if (_Remove)										\
									{													\
										PopFunction();									\
									}													\
									return true;

bool DragDropHandler::QueryOldest( DragDropHandler::DroppedFileInfo& _Output, bool _Remove /* = true */ )
{
	QUERYHELPER(PopOldest)
}

bool DragDropHandler::QueryNewest( DragDropHandler::DroppedFileInfo& _Output, bool _Remove /* = true */ )
{
	QUERYHELPER(PopNewest)
}

#undef QUERYHELPER

void DragDropHandler::PopOldest()
{
	m_Files.pop_front();
#ifdef DRAGDROP_DEBUGINFO
	LOG(VL_NORMAL, "DragDropHandler::PopNewest: Popped oldest file");
#endif
}

void DragDropHandler::PopNewest()
{
	m_Files.pop_back();
#ifdef DRAGDROP_DEBUGINFO
	LOG(VL_NORMAL, "DragDropHandler::PopNewest: Popped most recent file");
#endif
}

uint32 DragDropHandler::GetNumFiles()
{
	return m_Files.size();
}

void DragDropHandler::FlushFiles()
{
	m_Files.clear();
#ifdef DRAGDROP_DEBUGINFO
	LOG(VL_NORMAL, "DragDropHandler::FlushFiles: Flushed all files");
#endif
}

void DragDropHandler::_PushDroppedFile( const std::string& _File, const int2& _CursorPosition )
{
	DroppedFileInfo* Info = new DroppedFileInfo();
	Info->m_File = _File;
	Info->m_CursorPosition = _CursorPosition;
	m_Files.push_back(Info);

#ifdef DRAGDROP_DEBUGINFO
	LOG(VL_NORMAL, "DragDropHandler::_PushDroppedFile: Pushed file '%s' at cursor position [%i, %i]", _File.c_str(), _CursorPosition.x, _CursorPosition.y);
#endif
}

}

}

}

#endif