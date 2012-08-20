/*
	This fragment shader shades pixels based on the data present in the textures that have
	been rendered to by an earlier phase in the deferred rendering pipeline.
*/

#version 150

uniform sampler2D tRT0;
uniform sampler2D tRT1;
uniform sampler2D tRT2;
uniform sampler2D tRTDepth;	// The depth values are not linear!
uniform sampler2D tLighting;	// Lighting accumulation texture.
uniform vec2 ScreenDimensions;
in float zFar;
in float zNear;

//////////////////////////////////////////////////////////////////////////
// This variable will be used to set the final pixel color. Any
// post processing effects must alter this value in order to appear
// on screen.
//////////////////////////////////////////////////////////////////////////
vec4 Pixel;

vec2 FragCoord;

float LinearizeDepth( float _NonLinearDepth )
{
	return (2*zFar*zNear / (zFar + zNear - (zFar - zNear)*(2*_NonLinearDepth -1))) / zFar;
}

vec2 GetNormalizedScreenCoords()
{
	return (gl_FragCoord.xy / ScreenDimensions);	// Divide by screen width and height.
}

//////////////////////////////////////////////////////////////////////////
// Forward function declarations.
// These functions are defined in different shader files.
//////////////////////////////////////////////////////////////////////////
float SSAO( sampler2D _Depth, sampler2D _Normals );
vec3 LensFlare( sampler2D _Image );
//vec3 GaussianBlur( sampler2D _Image );

//////////////////////////////////////////////////////////////////////////
// Handles post processing calls.
//////////////////////////////////////////////////////////////////////////
void PostProcess()
{
	//Pixel.rgb = vec3(SSAO(tRTDepth, tRT2));
	//Pixel.rgb += LensFlare(tLighting);
}

//////////////////////////////////////////////////////////////////////////
// Shader entry point. Should NOT need to be altered. Any additional
// effects can be written outside of the main() function and added into
// the PostProcess() function.
//////////////////////////////////////////////////////////////////////////
void main( void )
{
	//////////////////////////////////////////////////////////////////////////
	// 'Initialize' the variable with the calculated lighting at this point.
	// Post processing can alter the value if needed.
	//////////////////////////////////////////////////////////////////////////
	Pixel = texture2D( tLighting, gl_TexCoord[0].xy );
	
	FragCoord = GetNormalizedScreenCoords();
	
	//////////////////////////////////////////////////////////////////////////
	// Apply post processing.
	//////////////////////////////////////////////////////////////////////////
	PostProcess();
	
	//////////////////////////////////////////////////////////////////////////
	// Set the fragment color.
	//////////////////////////////////////////////////////////////////////////
	gl_FragColor = Pixel;
}