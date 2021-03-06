
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
	float4 lightDiffuse = float4(0.9f, 0.9f, 0.9f, 1.0f);
	float4 lightAmbient = float4(0.7f, 0.7f, 0.7f, 1.0f);

	float3 lightDir = float3(0.3f, 0.3f, 0.7f);
	float lightIntensity = saturate(dot(input.Normal, lightDir));

	lightIntensity /= 0.5f;
	lightIntensity += 0.25f;
	
	
	float4 diffuseColor = (material.Diffuse * lightDiffuse) * lightIntensity;
	float4 ambientColor = material.Ambient * lightAmbient;

	float4 color = saturate(diffuseColor + ambientColor);

	if (material.UseTexture == false || false) {
		//float4 textureColor = diffuseTex.Sample(samplerState, input.Tex);
		//color = color * textureColor;
	}

	return color;
}
