uniform vec3 LightColor;
in vec3 LightPositionSS;
uniform float LightRadius;
uniform sampler2D tRT0;	// Diffuse.
uniform sampler2D tRT1;	// Positions.
uniform sampler2D tRT2;	// Normals.
uniform sampler2D tRTDepth;
uniform vec2 ScreenDimensions;
in float zFar;

vec2 GetNormalizedScreenCoords()
{
	return (gl_FragCoord.xy / ScreenDimensions);	// Divide by screen width and height.
}

void main()
{
	// Calculate normalized screen coordinates for sampling from the input textures.
	// TODO: Pass screen width and height to the shader instead of having the resolution hardcoded.
	vec2 normFragCoords = GetNormalizedScreenCoords();
	
	// Sample surface position at fragment.
	vec3 SurfacePosition = texture2D(tRT1, normFragCoords.xy).xyz /** zFar*/;
	
	// Sample surface normal at fragment.
	vec3 SurfaceNormal = texture2D(tRT2, normFragCoords.xy).xyz;
	//vec3 SurfaceNormal = (texture2D(tRT2, normFragCoords.xy).xyz * 2) - 1.0;
	
	// Calculate direction vector from light origin to fragment position.
	vec3 LightDir = LightPositionSS - SurfacePosition;
	
	// Calculate (linear) light attenuation.
	float distanceAtt = 1.0 - clamp((length(LightDir) / LightRadius), 0.0, 1.0);	// Value in range [0 .. 1], where 0 = at edge of radius and 1 = at origin of light.
	
	// Variable to hold the final fragment color.
	vec4 final_color = vec4(0.0, 0.0, 0.0, 1.0);
	
	SurfaceNormal = normalize(SurfaceNormal);	// TODO: Is this normalization actually necessary?
	LightDir = normalize(LightDir);
		
	float LambertTerm = dot(SurfaceNormal, LightDir);
	
	if(LambertTerm > 0.0)
	{
		vec4 diffCol = texture2D(tRT0, normFragCoords.xy);
		final_color += vec4(diffCol.rgb, 1.0) * vec4(LightColor.xyz, 1.0) * LambertTerm;
		
		float shininess = 50.0;	// TODO: Sample this value from a texture, or pass as per-object parameter.
		vec4 lightSpecular = vec4(0.5, 0.5, 0.5, 1.0);	// TODO: Should this be passed to the shader or can it remain hardcoded?
		vec4 matSpecular = vec4(diffCol.a, diffCol.a, diffCol.a, 1.0);
		vec3 EyeVec = normalize(SurfacePosition);
		//vec3 R = reflect(-LightDir, SurfaceNormal);
		vec3 R = reflect(LightDir, SurfaceNormal);
		float specular = pow( max(dot(R, EyeVec), 0.0), shininess );
		final_color += lightSpecular * matSpecular * specular;
	}

	gl_FragColor = final_color * distanceAtt;
}