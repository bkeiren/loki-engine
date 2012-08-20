#version 140

in float zFar;
in float zNear;

//////////////////////////////////////////////////////////////////////////
// Lens flare.
//////////////////////////////////////////////////////////////////////////

vec3 TextureDistorted( sampler2D _Texture, vec2 _SampleCenter, vec2 _SampleVector, vec3 _Distortion )
{
	return vec3(texture2D(_Texture, _SampleCenter + _SampleVector * _Distortion.r).r,
				texture2D(_Texture, _SampleCenter + _SampleVector * _Distortion.g).g,
				texture2D(_Texture, _SampleCenter + _SampleVector * _Distortion.b).b);
}

vec3 Distort( sampler2D _Texture, vec2 _TexCoords )
{
	int NSAMPLES = 5;
	float FLARE_DISPERSAL = 0.65;
	float FLARE_HALO_WIDTH = 0.5;
	vec3 FLARE_CHROMA_DISTORTION = vec3(0.1, 0.0, -0.1);
	
	vec2 image_center = vec2(0.5);
	//vec2 sample_vector = (image_center - _TexCoords) / float(NSAMPLES);
	vec2 sample_vector = (image_center - _TexCoords) * FLARE_DISPERSAL;
	vec2 halo_vector = normalize(sample_vector) * FLARE_HALO_WIDTH;
		
	//vec3 result = texture2D(_Texture, _TexCoords + halo_vector).rgb;
	vec3 result = TextureDistorted(_Texture, _TexCoords + halo_vector, halo_vector, FLARE_CHROMA_DISTORTION);
	for (int i = 0; i < NSAMPLES; ++i) 
	{
		vec2 offset = sample_vector * float(i);
		//result += texture(_Texture, _TexCoords + offset).rgb;
		result += TextureDistorted(_Texture, _TexCoords + offset, offset, FLARE_CHROMA_DISTORTION);
	}

	return result / (float(NSAMPLES) * 0.35);
	//return result;
}

vec3 LensFlare( sampler2D _Image )
{
	vec3 RGB_Threshold = vec3(0.975, 0.975, 0.975);
	
	// Sample the lighting texture with the texture coords flipped both horizontally and vertically,
	// then apply a threshold, clamp it to the [0 .. 1] range and scale it back up to brighten it.
	//vec3 col = clamp(texture2D( tLighting, -gl_TexCoord[0].xy + 1.0 ).xyz - RGB_Threshold.xyz, 0.0, 1.0) * 4.0;
	vec3 col = clamp(Distort(_Image, -gl_TexCoord[0].xy + 1.0) - RGB_Threshold.xyz, 0.0, 1.0) * 1.25;
	
	
	return col;
}