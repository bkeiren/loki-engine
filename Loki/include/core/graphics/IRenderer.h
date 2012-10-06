#pragma once

#ifndef IRENDERER_H
#define IRENDERER_H

namespace loki
{

namespace graphics
{

class IRenderer
{
public:
	enum ECullMode
	{
		CULL_MODE_FRONT = 0,
		CULL_MODE_BACK,
		CULL_MODE_FRONT_AND_BACK
	};

	enum EDepthMode
	{
		DEPTH_MODE_NEVER = 0,
		DEPTH_MODE_LESS,
		DEPTH_MODE_EQUAL,
		DEPTH_MODE_LEQUAL,
		DEPTH_MODE_GREATER,
		DEPTH_MODE_NOTEQUAL,
		DEPTH_MODE_GEQUAL,
		DEPTH_MODE_ALWAYS
	};

	enum EAlphaMode
	{
		ALPHA_MODE_NEVER = 0,
		ALPHA_MODE_LESS,
		ALPHA_MODE_EQUAL,
		ALPHA_MODE_LEQUAL,
		ALPHA_MODE_GREATER,
		ALPHA_MODE_NOTEQUAL,
		ALPHA_MODE_GEQUAL,
		ALPHA_MODE_ALWAYS
	};

	enum EStencilMode
	{
		STENCIL_MODE_NEVER = 0,
		STENCIL_MODE_LESS,
		STENCIL_MODE_EQUAL,
		STENCIL_MODE_LEQUAL,
		STENCIL_MODE_GREATER,
		STENCIL_MODE_NOTEQUAL,
		STENCIL_MODE_GEQUAL,
		STENCIL_MODE_ALWAYS
	};

	enum EStencilOperation
	{
		STENCIL_OPERATION_KEEP = 0,
		STENCIL_OPERATION_ZERO,
		STENCIL_OPERATION_REPLACE,
		STENCIL_OPREATION_INCREMENT,
		STENCIL_OPERATION_INCREMENT_WRAP,
		STENCIL_OPERATION_DECREMENT,
		STENCIL_OPERATION_DECREMENT_WRAP,
		STENCIL_OPERATION_INVERT
	};

	virtual void SetColorMask( bool _Red, bool _Green, bool _Blue, bool _Alpha ) const = 0;
	virtual void SetDepthMask( bool _Mask ) const = 0;
	virtual void SetStencilMask( bool _Mask ) const = 0;

	virtual void SetDepthTestEnabled( bool _Enabled ) const = 0;
	virtual void SetDepthMode( EDepthMode _Mode ) const = 0;
	virtual void SetDepthRange( float _Near, float _Far ) const = 0;

	virtual void SetAlphaTestEnabled( bool _Enabled ) const = 0;
	virtual void SetAlphaMode( EAlphaMode _Mode, float _ReferenceValue ) const = 0;

	virtual void SetStencilTestEnabled( bool _Enabled ) const = 0;
	virtual void SetStencilMode( EStencilMode _Mode, int _ReferenceValue, uint32 _Mask ) const = 0;
	virtual void SetStencilOperation( EStencilOperation _StenciLFail, EStencilOperation _DepthFail, EStencilOperation _DepthPass ) const = 0;
	
	virtual void SetCullFaceEnabled( bool _Enabled ) const = 0;
	virtual void SetCullMode( ECullMode _Mode ) const = 0;
	
protected:
	IRenderer();
	virtual ~IRenderer() = 0;

private:
};

}

}

#endif