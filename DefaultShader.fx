
cbuffer cbPerFrame : register(b2)
{
	matrix view;
}

cbuffer cbPerApp : register(b1)
{
	matrix proj;
}

cbuffer cbPerObj : register(b0) 
{
	matrix world;
}

Texture2D diffuseTex : register(t0);
SamplerState samplerState : register(s0);

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
    output.Tex = input.Tex;
	

	float4x4 modelView = mul(view, world);
	float4x4 modelViewProjection = mul(proj, modelView);
	float4 pos = mul(modelViewProjection, input.Pos);

	output.Pos = pos;
	
	
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input ) : SV_Target
{
	return diffuseTex.Sample(samplerState, input.Tex);
}
