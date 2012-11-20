// #pragma once
// 
// #ifndef GRAPHICS_H
// #define GRAPHICS_H
// 
// namespace loki
// {
// 
// namespace graphics
// {
// 
// class Graphics
// {
// 	friend class LokiEngine;
// public:
// 	enum ECullMode
// 	{
// 		CULL_MODE_FRONT = 0,
// 		CULL_MODE_BACK,
// 		CULL_MODE_FRONT_AND_BACK
// 	};
// 
// 	enum EDepthMode
// 	{
// 		DEPTH_MODE_NEVER = 0,
// 		DEPTH_MODE_LESS,
// 		DEPTH_MODE_EQUAL,
// 		DEPTH_MODE_LEQUAL,
// 		DEPTH_MODE_GREATER,
// 		DEPTH_MODE_NOTEQUAL,
// 		DEPTH_MODE_GEQUAL,
// 		DEPTH_MODE_ALWAYS
// 	};
// 
// 	enum EAlphaMode
// 	{
// 		ALPHA_MODE_NEVER = 0,
// 		ALPHA_MODE_LESS,
// 		ALPHA_MODE_EQUAL,
// 		ALPHA_MODE_LEQUAL,
// 		ALPHA_MODE_GREATER,
// 		ALPHA_MODE_NOTEQUAL,
// 		ALPHA_MODE_GEQUAL,
// 		ALPHA_MODE_ALWAYS
// 	};
// 
// 	enum EStencilMode
// 	{
// 		STENCIL_MODE_NEVER = 0,
// 		STENCIL_MODE_LESS,
// 		STENCIL_MODE_EQUAL,
// 		STENCIL_MODE_LEQUAL,
// 		STENCIL_MODE_GREATER,
// 		STENCIL_MODE_NOTEQUAL,
// 		STENCIL_MODE_GEQUAL,
// 		STENCIL_MODE_ALWAYS
// 	};
// 
// 	enum EStencilOperation
// 	{
// 		STENCIL_OPERATION_KEEP = 0,
// 		STENCIL_OPERATION_ZERO,
// 		STENCIL_OPERATION_REPLACE,
// 		STENCIL_OPREATION_INCREMENT,
// 		STENCIL_OPERATION_INCREMENT_WRAP,
// 		STENCIL_OPERATION_DECREMENT,
// 		STENCIL_OPERATION_DECREMENT_WRAP,
// 		STENCIL_OPERATION_INVERT
// 	};
// 
// 	enum EBlendMode
// 	{
// 		BLEND_MODE_ZERO = 0,
// 		BLEND_MODE_ONE,
// 		BLEND_MODE_SRC_COLOR,
// 		BLEND_MODE_ONE_MINUS_SRC_COLOR,
// 		BLEND_MODE_DST_COLOR,
// 		BLEND_MODE_ONE_MINUS_DST_COLOR,
// 		BLEND_MODE_SRC_ALPHA,
// 		BLEND_MODE_ONE_MINUS_SRC_ALPHA,
// 		BLEND_MODE_DST_ALPHA,
// 		BLEND_MODE_ONE_MINUS_DST_ALPHA,
// 		BLEND_MODE_CONSTANT_COLOR,
// 		BLEND_MODE_ONE_MINUS_CONSTANT_COLOR,
// 		BLEND_MODE_CONSTANT_ALPHA,
// 		BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA
// 	};
// 
// 	void SetColorMask( bool _Red, bool _Green, bool _Blue, bool _Alpha ) const;
// 	void SetDepthMask( bool _Mask ) const;
// 	void SetStencilMask( bool _Mask ) const;
// 
// 	void SetDepthTestEnabled( bool _Enabled ) const;
// 	void SetDepthMode( EDepthMode _Mode ) const;
// 	void SetDepthRange( f32 _Near, f32 _Far ) const;
// 
// 	void SetAlphaTestEnabled( bool _Enabled ) const;
// 	void SetAlphaMode( EAlphaMode _Mode, f32 _ReferenceValue ) const;
// 
// 	void SetStencilTestEnabled( bool _Enabled ) const;
// 	void SetStencilMode( EStencilMode _Mode, int32 _ReferenceValue, uint32 _Mask ) const;
// 	void SetStencilOperation( EStencilOperation _StenciLFail, EStencilOperation _DepthFail, EStencilOperation _DepthPass ) const;
// 	
// 	void SetBlendingEnabled( bool _Enabled ) const;
// 	void SetBlendMode( EBlendMode _SourceFactor, EBlendMode _DestinationFactor );
// 
// 	void SetCullingEnabled( bool _Enabled ) const;
// 	void SetCullMode( ECullMode _Mode ) const;
// 	
// protected:
// 	Graphics();
// 	~Graphics();
// 
// private:
// };
// 
// }
// 
// }
// 
// #endif