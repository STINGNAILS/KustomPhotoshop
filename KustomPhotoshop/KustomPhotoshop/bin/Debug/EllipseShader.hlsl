struct VertexIn
{
	float3 posL : POSITION;
	float4 color: COLOR0;
	float4 borderColor : COLOR1;
	float2 uv : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color: COLOR0;
	float4 borderColor: COLOR1;
	float2 uv : TEXCOORD;
};


cbuffer cbEllipse : register(b0)
{
	float4x4 viewProj;
	float4 abWidth;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.color = vertexIn.color;
	vertexOut.borderColor = vertexIn.borderColor;
	vertexOut.uv = vertexIn.uv;

	return vertexOut;
}

float4 PS(VertexOut vertexOut) : SV_Target
{
	const float a0 = abWidth.x - abWidth.z / 2.0f;
	const float a1 = abWidth.x + abWidth.z / 2.0f;
	const float b0 = abWidth.y - abWidth.z / 2.0f;
	const float b1 = abWidth.y + abWidth.z / 2.0f;

	const float u = vertexOut.uv.x;
	const float v = vertexOut.uv.y;

	const float r0 = (u * u) / (a0 * a0) + (v * v) / (b0 * b0);
	const float r1 = (u * u) / (a1 * a1) + (v * v) / (b1 * b1);

	if (r0 < 1.0f)
	{
		return float4(vertexOut.color.xyz, 1.0f);
	}
	else if(r1 < 1.0f)
	{
		return float4(vertexOut.borderColor.xyz, 1.0f);
	}
	else
	{
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}