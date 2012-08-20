/*
	This vertex shader simply transforms vertex data by the proper matrices. Can be
	used as a dummy shader when only the pixel shader has important stuff to do.
*/

out varying float zFar;
out varying float zNear;
in vec3 LightPosition;
out varying vec3 LightPositionSS;

void main( void )
{
	zFar = 500.0;
	zNear = 0.1;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_FrontColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	// Calculate light position in screen space.
	LightPositionSS = (gl_ModelViewMatrix * vec4(LightPosition.xyz, 1.0)).xyz;
}