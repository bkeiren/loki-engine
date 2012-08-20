/*
	This vertex shader simply transforms vertex data by the proper matrices. Can be
	used as a dummy shader when only the pixel shader has important stuff to do.
*/

out float zFar;
out float zNear;

void main( void )
{
	zFar = 500.0f;
	zNear = 0.1f;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;

	gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
}