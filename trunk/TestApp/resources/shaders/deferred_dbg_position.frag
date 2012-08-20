/*
	This shader simply outputs the data contained in the position component
	of the deferred render targets. It is used as a debug shader when all 
	individual buffers need to be visualized by the engine.
*/

#version 140

uniform sampler2D tPosition;
in float zFar;

void main( void )
{
	vec4 position = texture2D( tPosition, gl_TexCoord[0].xy ) /** zFar*/;
	gl_FragColor = position;
}