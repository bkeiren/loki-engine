/*
	This shader simply outputs the data contained in the depth component
	of the deferred render targets. It is used as a debug shader when all 
	individual buffers need to be visualized by the engine.
	NOTE: The shader does not directly render the data contained in the depth
	buffer. Instead, it linearizes the data (since OpenGL does not render linear
	z-values to the depth buffer in order to have differing quantities of
	precision, based on the distance of a pixel). The function LinearizeDepth is used
	to convert the non-linear depth value to a linear value (Where 0.0 = zNear and 
	1.0 = zFar).
*/

#version 140

uniform sampler2D tDepth;
in float zFar;
in float zNear;

float LinearizeDepth( float _NonLinearDepth )
{
	return (2*zFar*zNear / (zFar + zNear - (zFar - zNear)*(2*_NonLinearDepth -1))) / zFar;
}

void main( void )
{
	vec4 depth = texture2D( tDepth, gl_TexCoord[0].xy );
	float lineardepth = LinearizeDepth(depth.x);
	gl_FragColor = vec4(lineardepth, lineardepth, lineardepth, 1.0);;
}