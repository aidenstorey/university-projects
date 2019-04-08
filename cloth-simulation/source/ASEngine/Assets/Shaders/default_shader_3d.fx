cbuffer cbPerObject
{
	float4x4 gWVP;
};

struct VS_IN
{
	float3 pos		: POSITION;
	float4 color	: COLOR;
};

struct PS_IN
{
	float4 posH		: SV_POSITION;
	float4 color	: COLOR;
};

PS_IN VS( VS_IN vIn )
{
	PS_IN vOut;

	vOut.posH	= mul( float4( vIn.pos, 1.0f ), gWVP );
	vOut.color	= vIn.color;

	return ( vOut );
}

float4 PS( PS_IN vIn ) : SV_Target
{
	return vIn.color;
}

technique10 DefaultTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}