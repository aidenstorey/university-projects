cbuffer cbPerFrame
{
	float4x4 gLightWVP;
};

Texture2DArray gDiffuseMap;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_IN
{
	float3	posL		: POSITION0;
	float3	tangentL	: TANGENT;
	float3	normalL		: NORMAL;
	float2	texC		: TEXCOORD;
	float3	posI		: POSITION1;
	uint	texID		: TEXID;
};

struct PS_IN
{
	float4 posH		: SV_POSITION;
	float2 texC		: TEXCOORD;
	uint   texID	: TEXID;
};

PS_IN VS( VS_IN vIn )
{
	PS_IN pIn;

	pIn.posH	= mul( float4( vIn.posL + vIn.posI, 1.0f ), gLightWVP );
	pIn.texC	= vIn.texC;
	pIn.texID	= vIn.texID;

	return ( pIn );
}

void PS( PS_IN pIn )
{
	float4 diffuse = gDiffuseMap.Sample( gTriLinearSam, float3( pIn.texC, pIn.texID ) );

	clip( diffuse.a - 0.15f );
}

technique10 CreateShadowMapTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}