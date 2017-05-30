struct VertexIn
{
	float3 posL : POSITION;
	float4 color : COLOR;
};


struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float4 color : COLOR;
};


cbuffer cbBoundingBox : register(b0)
{
	float4x4 viewProj;
	float4 boundaries;
	float currentZoom;
	float aligner1;
	float aligner2;
	float aligner3;
};


VertexOut VS(VertexIn vertexIn)
{
	VertexOut vertexOut;

	vertexOut.posH = mul(float4(vertexIn.posL, 1.0f), viewProj);
	vertexOut.posW = vertexIn.posL;
	vertexOut.color = vertexIn.color;

	return vertexOut;
}


float4 PS(VertexOut vertexOut) : SV_Target
{
	if ((vertexOut.posW.x - boundaries.x) * currentZoom < 3.0f)
	{
		if (((vertexOut.posW.y - boundaries.y) * currentZoom < 3.0f) || ((boundaries.w - vertexOut.posW.y) * currentZoom < 3.0f))
		{
			return float4(vertexOut.color.xyz, 1.0f);
		}
		else if (fmod((boundaries.w - vertexOut.posW.y) * currentZoom, 12) <= 6.0f)
		{
			return float4(vertexOut.color.xyz, 1.0f);
		}
		else
		{
			return float4(vertexOut.color.xyz, 0.0f);
		}
	}
	else if ((vertexOut.posW.y - boundaries.y) * currentZoom < 3.0f)
	{
		if ((boundaries.z - vertexOut.posW.x) * currentZoom < 3.0f)
		{
			return float4(vertexOut.color.xyz, 1.0f);
		}
		else if (fmod((vertexOut.posW.x - boundaries.x) * currentZoom, 12) <= 6.0f)
		{
			return float4(vertexOut.color.xyz, 1.0f);
		}
		else
		{
			return float4(vertexOut.color.xyz, 0.0f);
		}
	}
	else if ((boundaries.z - vertexOut.posW.x) * currentZoom < 3.0f)
	{
		if ((boundaries.w - vertexOut.posW.y) * currentZoom < 3.0f)
		{
			return float4(vertexOut.color.xyz, 1.0f);
		}
		else if (fmod((vertexOut.posW.y - boundaries.y) * currentZoom, 12) <= 6.0f)
		{
			return float4(vertexOut.color.xyz, 1.0f);
		}
		else
		{
			return float4(vertexOut.color.xyz, 0.0f);
		}
	}
	else if ((boundaries.w - vertexOut.posW.y) * currentZoom < 3.0f)
	{
		if (fmod((boundaries.z - vertexOut.posW.x) * currentZoom, 12) <= 6.0f)
		{
			return float4(vertexOut.color.xyz, 1.0f);
		}
		else
		{
			return float4(vertexOut.color.xyz, 0.0f);
		}
	}

	return float4(vertexOut.color.xyz, 0.0f);
}