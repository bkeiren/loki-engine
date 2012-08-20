/*
	This shader simply outputs the data contained in the diffuse component
	of the deferred render targets. It is used as a debug shader when all 
	individual buffers need to be visualized by the engine.
*/

#version 140

uniform sampler2D tDiffuse;

void main( void )
{
	vec4 image = texture2D( tDiffuse, gl_TexCoord[0].xy );
	gl_FragColor = image;
}