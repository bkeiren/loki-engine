#pragma once

#ifndef INTERSECTIONRESULT_H
#define INTERSECTIONRESULT_H

namespace loki
{

class IntersectionResult
{
public:
	enum EResult
	{
		OUTSIDE = 0,
		INSIDE,
		INTERSECTS
	};
	
	IntersectionResult( EResult _Result = OUTSIDE )	:
		m_Result(_Result)
	{

	}

	~IntersectionResult()
	{

	}

	EResult m_Result;
};

}

#endif