#pragma once

#ifndef RESOURCE_H
#define RESOURCE_H

#include "core/resourcemanager/resourcemanager.h"

namespace loki
{

class LkResource
{
	template< class _ResType >
	friend class LkResourceManager;
public:
	const std::string m_Name;

protected:
	LkResource( const char* _Name );
	LkResource();
	virtual ~LkResource();

private:
	int32 m_RefCount;
};

}

#endif