Texture2D gShadowMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3 posL :	POSITION;
	float2 texC	:	TEXCOORD;
};

struct PS_IN
{
	float4 posH	:	SV_POSITION;
	float2 texC	:	TEXCOORD;
};

PS_IN VS( VS_IN vIn )
{
	PS_IN pIn;

	pIn.posH = float4( vIn.posL, 1.0f );
	pIn.texC = vIn.texC;

	return ( pIn );
}

float4 PS( PS_IN pIn ) : SV_Target
{
	return gShadowMap.Sample( gTriLinearSam, pIn.texC );
	float r = gShadowMap.Sample( gTriLinearSam, pIn.texC ).r;
	return float4( r, r, r, 1 );
}

technique10 DrawShadowMapTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}