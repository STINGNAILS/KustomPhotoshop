struct VertexIn
{
	float3 posL : POSITION;
	float4 color: COLOR;
	float2 uv : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color: COLOR;
	float2 uv : TEXCOORD;
};


cbuffer cbPolyLine : register(b0)
{
	float4x4 viewProj;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;
	
	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.color = vertexIn.color;
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}

float4 PS_LINES(VertexOut vertexOut) : SV_Target
{
	return vertexOut.color;
}

float4 PS_POINTS(VertexOut vertexOut) : SV_Target
{
	float u = vertexOut.uv.x;
	float v = vertexOut.uv.y;
	float u2v2 = u * u + v * v;
	
	if(u2v2 < 1.0f)
	{
		return float4(vertexOut.color.xyz, 1.0f);
	}
	else
	{
		return float4(vertexOut.color.xyz, 0.0f);
	}
}