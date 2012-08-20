#pragma once

#ifndef RESOURCEMANAGER_INL
#define RESOURCEMANAGER_INL

#include "util/hash/hash.h"

namespace loki
{

template< class _ResType >
LkResourceManager<_ResType>::LkResourceManager()
{
	_Init();
}

template< class _ResType >
LkResourceManager<_ResType>::~LkResourceManager()
{
	_Shutdown();
}

template< class _ResType >
bool LkResourceManager<_ResType>::_Init()
{
	return true;
}

template< class _ResType >
void LkResourceManager<_ResType>::_Shutdown()
{

}

template< class _ResType >
_ResType LkResourceManager<_ResType>::GetResource( const char* _Res )
{
	unsigned int Hash = HASH(_Res);

	stdext::hash_map<ResourceID, _ResType>::iterator it = m_Resources.find(Hash);
	if (it != m_Resources.end())
	{
		// Texture already exists.
		++it->second->m_RefCount;
		return it->second;
	}
	else
	{
		_ResType res = _LoadResource(_Res);
		if (res)
		{
			++res->m_RefCount;
			m_Resources.insert(std::pair<ResourceID, _ResType>(Hash, res));
			return res;
		}
		else
		{
			LOG(VL_WARN, "ResourceManager::GetResource: _LoadResource returned a NULL pointer");
		}
	}

	return NULL;
}

template< class _ResType >
void LkResourceManager<_ResType>::ReleaseResource( _ResType* _Res )
{
	if (!*_Res)
	{
		return;
	}

	--(*_Res)->m_RefCount;

	// Assert that the refcount is greater or equal to 0. If it is less, the texture was released more
	// than the number of times it was requested through GetTexture().
	assert((*_Res)->m_RefCount >= 0);

	// If the Texture's ref count is 0, the TextureManager is free to delete it.
	if ((*_Res)->m_RefCount == 0)
	{
		unsigned int Hash = HASH((*_Res)->m_Name.c_str());
		m_Resources.erase(Hash);
		delete (*_Res);
	}

	// Set the value to 0.
	*_Res = 0;
}

template< class _ResType >
unsigned int LkResourceManager<_ResType>::GetNumLoadedResources()
{
	return m_Resources.size();
}

template< class _ResType >
_ResType LkResourceManager<_ResType>::_LoadResource( const char* _Res )
{
	return _ResType(_Res);
}

}

#endif