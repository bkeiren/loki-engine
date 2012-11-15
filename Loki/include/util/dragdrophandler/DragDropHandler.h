#pragma once

#ifdef _WIN32

#ifndef DRAGDROPHANDLER_H
#define DRAGDROPHANDLER_H

#include <deque>

namespace loki
{

namespace util
{

namespace system
{

class DragDropHandler
{
	friend class LokiEngine;
public:
	struct DroppedFileInfo
	{
		// Name of the file that was dropped.
		std::string m_File;

		// Position of the cursor in window-coordinates at the moment the file was dropped.
		int2 m_CursorPosition;
	};

	static bool QueryOldest( DroppedFileInfo& _Output, bool _Remove = true );
	static bool QueryNewest( DroppedFileInfo& _Output, bool _Remove = true );
	static void PopOldest();
	static void PopNewest();
	static uint32 GetNumFiles();
	static void FlushFiles();
private:
	DragDropHandler();
	~DragDropHandler();

	static void _PushDroppedFile( const std::string& _File, const int2& _CursorPosition );

	static std::deque<DroppedFileInfo*> m_Files;
};

}

}

}

#endif

#endif