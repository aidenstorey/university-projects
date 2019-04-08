cbuffer cbPerFrame
{
	float3 gEyePositionW;
	float4x4 gViewProj;
};

Texture2DArray gTextureArray;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState gAnisotropic
{
	Filter = ANISOTROPIC;
};

struct VS_INOUT
{
	float3	posW	: POSITION;
	float2	sizeW	: SIZE;
	uint	texID	: TEXID;
};

struct PS_IN
{
	float4	posH	: SV_POSITION;
	float2  texC	: TEXCOORD;
	uint	texID	: TEXID;
};

VS_INOUT VS( VS_INOUT vIn )
{
	return ( vIn );
}

[ maxvertexcount( 4 ) ]
void GS( point VS_INOUT gIn[ 1 ], inout TriangleStream< PS_IN > triStream )
{
	// Half width/height
	float fHW = gIn[ 0 ].sizeW.x * 0.5f;
	float fHH = gIn[ 0 ].sizeW.y * 0.5f;

	// offset values
	float4 v[ 4 ] = {
		float4( -fHW, -fHH, 0.0f, 1.0f ),
		float4( +fHW, -fHH, 0.0f, 1.0f ),
		float4( -fHW, +fHH, 0.0f, 1.0f ),
		float4( +fHW, +fHH, 0.0f, 1.0f ),
	};

	// tex coords
	float2 texC[ 4 ] = {
		float2( 0.0f, 1.0f ),
		float2( 1.0f, 1.0f ),
		float2( 0.0f, 0.0f ),
		float2( 1.0f, 0.0f ),
	};

	// Align billboiard to y-axis of camera
	float3 up = float3( 0.0f, 1.0f, 0.0f );
	float3 look = gEyePositionW - gIn[ 0 ].posW;
	look = normalize( look );
	float3 right = cross( up, look );

	float4x4 W = {
		float4( right, 0.0f ),
		float4( up, 0.0f ),
		float4( look, 0.0f ),
		float4( gIn[ 0 ].posW, 1.0f ),
	};

	float4x4 WVP = mul( W, gViewProj );

	PS_IN pIn;
	[unroll]
	for( int i = 0; i < 4; ++i )
	{
		pIn.posH = mul( v[ i ], WVP );
		pIn.texC	= texC[ i ];
		pIn.texID	= gIn[ 0 ].texID;

		triStream.Append( pIn );
	}
}

float4 PS( PS_IN pIn ) : SV_TARGET
{
	return ( gTextureArray.Sample( gTriLinearSam, float3( pIn.texC, pIn.texID ) ) );
}

technique10 BillboardTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( CompileShader( gs_4_0, GS() ) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}