/*
	This shader simply outputs the data contained in the normals component
	of the deferred render targets. It is used as a debug shader when all 
	individual buffers need to be visualized by the engine.
*/

#version 140

uniform sampler2D tNormals;

void main( void )
{
	vec4 normal = texture2D( tNormals, gl_TexCoord[0].xy );
	//gl_FragColor = normal;
	gl_FragColor = (normal + 1.0) * 0.5;	// Pack normals for displaying.
}