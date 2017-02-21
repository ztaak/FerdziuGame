
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

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float4 Color : COLOR )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Color = Color;
	

	float4x4 modelView = mul(view, world);
	float4x4 modelViewProjection = mul(proj, modelView);
	float4 pos = mul(modelViewProjection, Pos);

	output.Pos = pos;
	
	
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}
