// #include "core/resourcemanager/texturemanager.h"
// #include "core/resourcemanager/resourcemanager.h"
// #include <SOIL/SOIL.h>
// #include <GLEW\\glew.h>
// 
// namespace loki
// {
// 
// 	namespace
// 	{
// 		//////////////////////////////////////////////////////////////////////////
// 		// Attempts to load and create an OpenGL texture and stores
// 		// it's OpenGL ID in _ID. Returns true if successful, otherwise false.
// 		//////////////////////////////////////////////////////////////////////////
// 		bool LoadGLTexture( const char* _Path, GLuint& _ID )
// 		{
// 			_ID = SOIL_load_OGL_texture(_Path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
// 
// 			if (_ID == 0)
// 			{
// 				LOG(VL_ERROR, "LoadGLTexture: Failed to load texture '%s'", _Path);
// 				return false;
// 			}
// 
// 			glBindTexture(GL_TEXTURE_2D, _ID);
// 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 
// 			return true;
// 		}
// 	}
// 
// LkTextureManager* g_TextureManager = NULL;
// 
// // Declare the static member m_Resources.
// //template<>
// //std::map<TextureManager::ResourceID, renderer::Texture*> TextureManager::m_Resources;
// 
// // _LoadResource Texture specialization.
// template<>
// renderer::LkTexture* LkTextureManager::_LoadResource( const char* _Res )
// {
// 	GLuint id = 0;
// 	if (LoadGLTexture(_Res, id))
// 	{
// 		renderer::LkTexture* res = new renderer::LkTexture(_Res, id);		
// 		return res;
// 	}
// 	else
// 	{
// 		static renderer::LkTexture* DefaultTexture = NULL;
// 		// If no texture is found, use the default texture (Which is made
// 		// to be very clearly visible so that it's obvious something is missing).
// 		// The default texture's ref count doesn't need to be increased.
// 		if (!DefaultTexture)
// 		{
// 			if (!LoadGLTexture("resources//textures//default.bmp", id))
// 			{
// 				assert("ResourceManager::_LoadResource: Failed to load default texture." && 0);
// 				return NULL;
// 			}
// 			else
// 			{
// 				DefaultTexture = new renderer::LkTexture("default", id);
// 			}
// 		}		
// 		return DefaultTexture;
// 	}
// 
// 	return NULL;
// }
// 
// 
// }