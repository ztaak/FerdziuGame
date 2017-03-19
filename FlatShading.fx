
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

struct Material
{
	float4  Emissive;
	float4  Ambient;
	float4  Diffuse;
	float4  Specular;
	float   SpecularPower;
	bool    UseTexture;
	float2  Padding;
};

cbuffer cbMaterial : register(b3)
{
	Material material;
};

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
	float4 Position : SV_POSITION;
	float4 PositionWorld : TEXCOORD0;
	float2 TextureCoordinate : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.TextureCoordinate = input.Tex;

	float4 worldPosition = mul(input.Pos, world);
	float4x4 modelView = mul(view, world);
	float4x4 modelViewProjection = mul(proj, modelView);
	float4 pos = mul(modelViewProjection, input.Pos);

	output.Position = pos;
	output.PositionWorld = worldPosition;


	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float3 Normal = cross(ddy(input.PositionWorld.xyz), ddx(input.PositionWorld.xyz));
	Normal = normalize(Normal);

	float3 lightDir = float3(-0.3f, -0.3f, -1.0f);
	float lightIntensity = saturate(dot(Normal, lightDir));

	if (lightIntensity <= 0.6f)lightIntensity = 0.6f;

	float4 lightColor = lightIntensity * material.Diffuse * 1.0f + material.Ambient * 1.0f;
	lightColor.a = 1;


	float4 textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//textureColor = diffuseTex.Sample(samplerState, input.TextureCoordinate);
	//textureColor.a = 1;

	return saturate(textureColor * lightColor);
}
