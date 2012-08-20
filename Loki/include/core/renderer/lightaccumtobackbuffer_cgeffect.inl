//////////////////////////////////////////////////////////////////////////
// This file contains shader source code for a cgfx format file.
// Each string must end with a newline character ('\n'), as this is required
// by the Cg compiler (due to the function that is called to read a line
// from the source).
//////////////////////////////////////////////////////////////////////////
"struct VertexInput\n"	\
"{\n"	\
"	in float3 Pos	: POSITION;\n"	\
"	in float2 Tex	: TEXCOORD0;\n"	\
"};\n"	\
"struct VertexOutput\n"	\
"{\n"	\
"	float4 PosH	: HPOS;\n"	\
"	float2 Tex	: TEXCOORD0;\n"	\
"};\n"	\
"sampler2D Sampler	: LKLIGHTACCUMULATIONTEX;\n"	\
"VertexOutput mainVP( VertexInput IN )\n"	\
"{\n"	\
"	VertexOutput OUT;\n"	\
"	OUT.PosH = float4(IN.Pos, 1.0);	// Maybe 0.0?\n"	\
"	OUT.Tex = IN.Tex;\n"	\
"	return OUT;\n"	\
"}\n"	\
"float4 mainFP( VertexOutput IN )	: COLOR0\n"	\
"{\n"	\
"	return tex2D(Sampler, IN.Tex);\n"	\
"}\n"	\
"technique t0\n"	\
"{"	\
"	pass p0\n"	\
"	{\n"	\
"		CullFaceEnable = false;\n"	\
"		VertexProgram = compile latest mainVP();\n"	\
"		FragmentProgram = compile latest mainFP();\n"	\
"	}\n"	\
"}\n"