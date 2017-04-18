struct VertexIn
{
	float3 posL : POSITION;
	float4 color : COLOR0;
	float4 borderColor : COLOR1;
};


struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float4 color : COLOR0;
	float4 borderColor : COLOR1;
};


cbuffer cbTriangle : register(b0)
{
	float4x4 viewProj;
	float4 v1v2;
	float4 v3Width;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.posW = vertexIn.posL;
	vertexOut.color = vertexIn.color;
	vertexOut.borderColor = vertexIn.borderColor;

	return vertexOut;
}


//float DistanceToLine(float2 a, float2 b, float2 c)
//{
//	const float2 amc = a - c;
//	const float2 amb = a - b;
//	const float k = (amc.x * amb.x + amc.y * amb.y) / (amb.x * amb.x + amb.y * amb.y);
//
//	const float2 d = (1.0f - k) * a + k * b;
//	const float2 cmd = c - d;
//
//	return sqrt(cmd.x * cmd.x + cmd.y * cmd.y);
//}


float DistanceToLine(float2 a, float2 b, float2 c)
{
	const float2 amb = a - b;
	
	return abs(amb.y * c.x - amb.x * c.y + a.x * b.y - a.y * b.x) / sqrt(amb.x * amb.x + amb.y * amb.y);
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	const float2 c = vertexOut.posW.xy;

	float distanceToClosestLine = min(DistanceToLine(v1v2.xy, v1v2.zw, c), min(DistanceToLine(v1v2.zw, v3Width.xy, c), DistanceToLine(v3Width.xy, v1v2.xy, c)));
	float k = saturate(-distanceToClosestLine + width + 1.0f);

	return (1.0f - k) * vertexOut.color + k * vertexOut.borderColor;
}