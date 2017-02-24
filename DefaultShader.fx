
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
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 Normal : NORMAL;
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

	output.Normal = mul(input.Normal, (float3x3)transpose(world));
	output.Normal = normalize(output.Normal);
	
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input ) : SV_Target
{
	float4 textureColor = diffuseTex.Sample(samplerState, input.Tex);

	float3 lightDir = float3(-0.8f, -0.5f, -0.4f);
	float lightIntensity = saturate(dot(input.Normal, lightDir));
	float4 diffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	if (lightIntensity <= 0.3f)lightIntensity = 0.3f; // CHANGE THIS

	float4 color = saturate(diffuseColor * lightIntensity);

	color = color * textureColor;

	return color;
}
