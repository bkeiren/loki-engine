#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "core/resourcemanager/resourcemanager.h"
#include "core/renderer/texture/texture.h"

namespace loki
{

typedef LkResourceManager<renderer::LkTexture*>		LkTextureManager;

template<>
renderer::LkTexture* LkTextureManager::_LoadResource( const char* _Res );

extern LkTextureManager* g_TextureManager;

}

#endif