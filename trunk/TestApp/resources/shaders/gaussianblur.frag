#version 140

in float zFar;
in float zNear;

//////////////////////////////////////////////////////////////////////////
// Gaussian blur.
//////////////////////////////////////////////////////////////////////////
vec3 GaussianBlur( sampler2D _Image )
{
	vec3 col = texture2D(_Image, gl_TexCoord[0].xy).rgb;
	
	int SAMPLES = 50;
	
	float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
	float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);
	
	for (int i = 0; i < SAMPLES; ++i)
	{
		for (int i = 0; i < 3; ++i)
		{
			col += texture2D(_Image, gl_TexCoord[0].xy + vec2(0.0, offset[i])).rgb * weight[i];
			col += texture2D(_Image, gl_TexCoord[0].xy - vec2(0.0, offset[i])).rgb * weight[i];
		}
		
		for (int i = 0; i < 3; ++i)
		{
			col += texture2D(_Image, gl_TexCoord[0].xy + vec2(offset[i], 0.0)).rgb * weight[i];
			col += texture2D(_Image, gl_TexCoord[0].xy - vec2(offset[i], 0.0)).rgb * weight[i];
		}
	}
	
	return col / SAMPLES;
}