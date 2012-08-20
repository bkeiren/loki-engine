#version 140

uniform sampler2D tLight;
in float zFar;

void main( void )
{
	gl_FragColor = texture2D( tLight, gl_TexCoord[0].xy ) /** zFar*/;
}