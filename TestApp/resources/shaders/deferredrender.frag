/*
	This shader renders geometry data into different render targets which can later be used
	when deferred shading occurs.
*/
#version 150


in vec3 normal;
in vec4 position;
in mat3 TBN;
uniform sampler2D tDiffuseMap;
uniform sampler2D tNormalMap;
uniform sampler2D tSpecularMap;
uniform sampler2D tEmissiveMap;
in mat4 ModelViewMatrix;
in mat4 ModelViewProjectionMatrix;
in mat3 NormalMatrix;
in float zFar;

void main( void )
{
	// Alpha component is specularity (Monochrome, stored in diffuse alpha channel).
	gl_FragData[0]		= vec4(texture2D(tDiffuseMap, gl_TexCoord[0].st).rgb, texture2D(tSpecularMap, gl_TexCoord[0].st).r);
	
	// Screen-space position.
	gl_FragData[1]		= vec4(position.xyz, 0) /*/ zFar*/;

	// Screen-space normals.
	vec3 normalpix = vec3(texture2D(tNormalMap, gl_TexCoord[0].st).rgb);	// Sampled normal in tangent space.
	vec3 normalpix_vs = normalize(TBN * normalpix); 					// The sampled normal in view space.
	gl_FragData[2]		= (normalpix.xyz == vec3(0.0, 0.0, 0.0)) ? 
						  //(vec4(mat3(ModelViewMatrix) * normal.xyz, 0)) :
						  //(vec4(mat3(ModelViewMatrix) * vec3(0.0, 1.0, 0.0), 0.0))	:
						  (vec4(normalize(TBN * vec3(0.0, 1.0, 0.0)), 0.0))	:
						  (vec4(normalpix_vs.xyz, 0.0));

	gl_FragData[3] = vec4(vec3(texture2D(tEmissiveMap, gl_TexCoord[0].st).rgb), 1.0);
}
