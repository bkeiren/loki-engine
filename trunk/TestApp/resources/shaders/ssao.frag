#version 150

in float zFar;
in float zNear;

float LinearizeDepth( float _NonLinearDepth );

float SSAO( sampler2D _Depth, sampler2D _Normals )
{
// 	float ao = 1.0;
// 	
// 	float x = 1.0 / 1280;
// 	float y = 1.0 / 720;
// 	
// 	// Sample the depth in a kernel around the center pixel, calculating the average distance in depth inside of the kernel.
// 	// The greater this average value is, the greater the difference at some two or more points in the kernel.
// 	float Depth = LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy).r);
// 	float AveragedDepth = 0.0;
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy - vec2(x, 0.0)).r);
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy + vec2(x, 0.0)).r);
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy - vec2(0.0, y)).r);
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy + vec2(0.0, y)).r);
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy - vec2(x, y)).r);
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy + vec2(-x, y)).r);
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy + vec2(x, -y)).r);
//  	AveragedDepth += LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy + vec2(x, y)).r);
// 	// 1.0 / (difference).
// 	ao = (((clamp(AveragedDepth - Depth, 0.0, 1.0)) - 0.5) * 1.0) + 0.5;
// 	//ao = Depth;
// 	
// // 	vec3 Normal = texture2D(_Normals, gl_TexCoord[0].xy).xyz;
// // 	float AveragedDot = 0.0;
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy - vec2(x, 0.0)).xyz) - 1.0);
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy + vec2(x, 0.0)).xyz) - 1.0);
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy - vec2(0.0, y)).xyz) - 1.0);
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy + vec2(0.0, y)).xyz) - 1.0);
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy - vec2(x, y)).xyz) - 1.0);
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy + vec2(-x, y)).xyz) - 1.0);
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy + vec2(x, -y)).xyz) - 1.0);
// // 	AveragedDot += abs(dot(Normal, texture2D(_Normals, gl_TexCoord[0].xy + vec2(x, y)).xyz) - 1.0);
// // 	AveragedDot /= 8;
// // 	
// // 	//AveragedDot = dot(vec3(0.0, 1.0, 0.0), Normal);
// // 	
// // 	ao = AveragedDot / 20;	
// 
// 	// The pixel color is multiplied by this value. An ao value of 0.0 means that the pixel is completely occluded (and thus will 
// 	// not contribute to the image). An ao value of 1.0 means the pixel is fully visible.
// 	return ao;
	
	
	float pos = sqrt( LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy).r) );
	float occlusion = 0;
	float sample;
	float pPos;
	float dist = mod((gl_TexCoord[0].x * 1280), 8);
	dist += mod((gl_TexCoord[0].y * 720), 8);
	dist += 1;
	vec3 pNorm;
	float SSnorm;
	vec3 vec;
	vec4 vecViewPort = vec4(0.0, 0.0, zNear, zFar);
	for(int j = 1; j < 3; j++)
	{
		for(float i = 0; i < 180; i += 45)
		{
			pPos = sqrt( LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy + vec2(cos(i), sin(i)) * j * dist * vecViewPort.zw).r) );
			SSnorm = sqrt( LinearizeDepth(texture2D(_Depth, gl_TexCoord[0].xy - vec2(cos(i), sin(i)) * j * dist * vecViewPort.zw).r) );
			//SSnorm -= pos;
			sample = (pos / pPos) - (SSnorm / pos);
			if((sample > 0.001) && (sample < 0.05))
					occlusion += (sample * 30);
		}
	}
	
	return occlusion;
}
