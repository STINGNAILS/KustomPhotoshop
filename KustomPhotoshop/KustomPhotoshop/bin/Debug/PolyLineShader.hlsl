struct VertexIn
{
	float3 PosL : POSITION;
	float4 Color: COLOR;
	float2 TexUV : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color: COLOR;
	float2 TexUV : TEXCOORD;
};

cbuffer cbQuads : register(b0)
{
	float4x4 viewProj;
};

VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;
	
	vertexOut.PosH = mul(float4(vertexIn.PosL, 1.0f), viewProj);
	vertexOut.Color = vertexIn.Color;
	vertexOut.TexUV = vertexIn.TexUV;

	return vertexOut;
}

float4 PS_LINES(VertexOut vertexOut) : SV_Target
{
	return vertexOut.Color;
}

float4 PS_POINTS(VertexOut vertexOut) : SV_Target
{
	float u = vertexOut.TexUV.x;
	float v = vertexOut.TexUV.y;
	float u2v2 = u * u + v * v;
	
	if(u2v2 < 1.0f)
	{
		return float4(vertexOut.Color.xyz, 1.0f);
	}
	else
	{
		return float4(vertexOut.Color.xyz, 0.0f);
	}
}