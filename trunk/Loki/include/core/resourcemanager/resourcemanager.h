#pragma once

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <hash_map>

namespace loki
{

//////////////////////////////////////////////////////////////////////////
// Templated resource manager class. To create a resource manager
// for a certain type of resource, typedef this class with the correct
// template parameter to your new manager type.
// For example, the texture manager is typedeffed as:
// typedef ResourceManager<Texture*>	TextureManager;
// This means that the type TextureManager will be able to keep track of
// Texture instances (and stores pointers). Functions that need
// special attention (Typically this is the _LoadResource() function,
// can be specialized for their template parameter.
//////////////////////////////////////////////////////////////////////////
template< class _ResType >
class LkResourceManager
{
	typedef unsigned int							ResourceID;
public:
	LkResourceManager();
	~LkResourceManager();

	_ResType GetResource( const char* _Res );
	void ReleaseResource( _ResType* _Res );

	unsigned int GetNumLoadedResources();
private:
	bool _Init();
	void _Shutdown();

	//////////////////////////////////////////////////////////////////////////
	// This function must be specialized to provide a different way of loading
	// for each type of resource.
	//////////////////////////////////////////////////////////////////////////
	_ResType _LoadResource( const char* _Res );

	stdext::hash_map<ResourceID, _ResType> m_Resources;
};

}

#include "core/resourcemanager/resourcemanager.inl"

#endif