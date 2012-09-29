#pragma once

#ifndef IRENDERER_H
#define IRENDERER_H

class IRenderer
{
public:
	
	virtual const int4& GetViewport() const = 0;
	virtual void SetViewport( const int4& _Viewport ) = 0;

	virtual const mat4& GetModelMatrix() const = 0;
	virtual void SetModelMatrix( const mat4& _ModelMatrix ) = 0;

	virtual void EnableEffect( LkEffect* _Effect );
	virtual void DisableEffect();

	virtual void RenderMesh( LkMesh* _Mesh );
private:
};

#endif