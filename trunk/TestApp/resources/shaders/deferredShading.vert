/*
	This vertex shader is used to transform the geometry of the fullscreen quad that is
	used to shade pixels in the deferred shading pipeline.
*/

out varying float zFar;
out varying float zNear;

void main( void )
{
	// TODO: Pass this from application.
	zFar = 500.0;
	zNear = 0.1;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	//gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
}