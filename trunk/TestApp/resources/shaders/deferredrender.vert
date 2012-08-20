/*
	This shader transforms data for the deferred rendering fragment shader, which
	renders geometry data into different render targets which can later be used
	when deferred shading occurs.
*/

#version 150

in vec3 AttribPosition;
in vec3 AttribNormal;
in vec3 AttribTangent;
in vec3 AttribBinormal;
in vec2 AttribTexcoord;
in float AttribZFar;
in float AttribZNear;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
mat4 ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
mat3 NormalMatrix = mat3(inverse(transpose(ModelViewMatrix)));
uniform vec2 UVScale = vec2(1.0, 1.0);
uniform vec3 Scale = vec3(1.0, 1.0, 1.0);

out vec3 normal;
out vec4 position;
out mat3 TBN;
out float zFar;

void main( void )
{
	zFar = AttribZFar;
	
	gl_Position = ModelViewProjectionMatrix * (gl_Vertex * vec4(Scale, 1.0));
	
	//gl_Position = gl_ModelViewProjectionMatrix * vec4(AttribPosition, 0.0);	// NOTE: AttribPosition doesn't work properly.
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[0] = vec4(AttribTexcoord * UVScale, 0.0, 0.0);
	position = ModelViewMatrix * (gl_Vertex * vec4(Scale, 1.0));

    gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	//normal = normalize(gl_NormalMatrix * gl_Normal);	// Normals in screen space.
	normal = normalize(AttribNormal);
	TBN = NormalMatrix * mat3(AttribTangent, AttribBinormal, normal);
}