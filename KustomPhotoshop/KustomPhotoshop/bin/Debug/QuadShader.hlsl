struct VertexIn
{
	float3 posL : POSITION;
	float4 color : COLOR;
};


struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};


cbuffer cbQuad : register(b0)
{
	float4x4 viewProj;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.color = vertexIn.color;

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	return vertexOut.color;
}