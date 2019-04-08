//=============================================================================
// tex.fx                                                                                                   
//
// Transforms, lights, and textures geometry.
//=============================================================================


#include "lighthelper.fx"

static const float SHADOW_EPSILON	= 0.003f;
static const float SHADOWMAP_SIZE	= 2048.0f;
static const float SHADOWMAP_DX		= 1.0f / SHADOWMAP_SIZE;

 
cbuffer cbFixed
{
	float gFogStart = 5.0f;
	float gFogRange = 400.0f;
}
 
cbuffer cbPerFrame
{
	Light gLight;
	float3 gFogColor;
	float3 gEyePosW;
};

bool gSpecularEnabled;

cbuffer cbPerObject
{
	float4x4 gLightWVP;
	float4x4 gWorld;
	float4x4 gWVP; 
	float4x4 gTexMtx;
};

Texture2DArray	gTextureArray;
Texture2D		gShadowMap;

SamplerState gShadowSam
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;	
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState gAnisotropic
{
	Filter = ANISOTROPIC;
};

struct VSI_IN
{
	float3	posL		: POSITION0;
	float3	tangentL	: TANGENT;
	float3	normalL		: NORMAL;
	float2	texC		: TEXCOORD;
	float3	posI		: POSITION1;
	uint	texID		: TEXID;
};

struct VSI_OUT
{
	float4	posH		: SV_POSITION;
    float3	posW		: POSITION;
	float3	tangentW	: TANGENT;
    float3	normalW		: NORMAL;
    float2	texC		: TEXCOORD0;
	uint	texID		: TEXID;
	float	fogLerp		: FOG;
	float4	projTexC	: TEXCOORD1;
};

float CalculateShadowFactor( float4 ptC )
{
	// Complete projection
	ptC.xyz /= ptC.w;

	// Points outside light volume are in shadow
	if( ptC.x < -1.0f || ptC.x > 1.0f || ptC.y < - 1.0f || ptC.y > 1.0f || ptC.z < 0.0f )
	{
		return ( 0.0f );
	}

	// Transform from NDC space to texture space
	ptC.x = 0.5f * ptC.x + 0.5f;
	ptC.y = -0.5f * ptC.y + 0.5f;

	// depth in NDC space
	float depth = ptC.z;
	
	float sTest = gShadowMap.Sample( gShadowSam, ptC.xy ).r;

	float rTest = depth <= sTest + SHADOW_EPSILON;

	return ( rTest );

}
 
VSI_OUT VS_I(VSI_IN vIn)
{
	VSI_OUT vOut;
	
	// Transform to world space space.
	float3 pos = float3( vIn.posL + vIn.posI );
	vOut.posW		= mul(float4(pos, 1.0f), gWorld);
	vOut.tangentW	= mul( float4( vIn.tangentL, 0.0f ), gWorld );
	vOut.normalW	= mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(pos, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	vOut.texC  = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);
	vOut.projTexC = mul( float4( pos, 1.0f ), gLightWVP );

	// Set texture to be sampled
	vOut.texID = vIn.texID;

	float d = distance( vOut.posW, gEyePosW );
	vOut.fogLerp = saturate( ( d - gFogStart ) / gFogRange );
	
	return vOut;
}


float4 PS_I(VSI_OUT pIn) : SV_Target
{
	uint texID = pIn.texID * 3;

	// Get materials from texture maps.
	float4 diffuse = gTextureArray.Sample( gAnisotropic, float3( pIn.texC, texID ) );

	clip( diffuse.a - 0.15f );

	float4 spec    = gTextureArray.Sample( gAnisotropic, float3( pIn.texC, texID + 1 ) );
	float4 normalT = gTextureArray.Sample( gAnisotropic, float3( pIn.texC, texID + 2 ) );
	
	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Uncompress
	normalT = 2.0f * normalT - 1.0f;

	// build orthonormal basis
	float3 N = normalize( pIn.normalW );
	float3 T = normalize( pIn.tangentW - dot( pIn.tangentW, N) * N );
	float3 B = cross( N, T );

	float3x3 TBN = float3x3( T, B, N );

	float3 bumpedNormalW = normalize( mul( normalT, TBN ) );
	    
	float shadowFactor = CalculateShadowFactor( pIn.projTexC );

	// Compute the lit color for this pixel.
    SurfaceInfo v = {pIn.posW, bumpedNormalW, diffuse, spec};
	float3 litColor = ParallelLight(v, gLight, gEyePosW, shadowFactor);

	float3 foggedColor = lerp( litColor, gFogColor, pIn.fogLerp );
    
    return float4(foggedColor, diffuse.a);
}

technique10 InstancedTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS_I() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_I() ) );
    }
}



// Non-instanced
struct VS_IN
{
	float3	posL		: POSITION0;
	float3	tangentL	: TANGENT;
	float3	normalL		: NORMAL;
	float2	texC		: TEXCOORD;
	uint	texID		: TEXID;
};

struct VS_OUT
{
	float4	posH		: SV_POSITION;
    float3	posW		: POSITION;
	float3	tangentW	: TANGENT;
    float3	normalW		: NORMAL;
    float2	texC		: TEXCOORD0;
	uint	texID		: TEXID;
	float	fogLerp		: FOG;
	float4	projTexC	: TEXCOORD1;
};
 
VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;
	
	vOut.posW		= mul(float4(vIn.posL, 1.0f), gWorld);
	vOut.tangentW	= mul( float4( vIn.tangentL, 0.0f ), gWorld );
	vOut.normalW	= mul(float4(vIn.normalL, 0.0f), gWorld);
		
	// Transform to homogeneous clip space.
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWVP);
	
	// Output vertex attributes for interpolation across triangle.
	vOut.texC  = mul(float4(vIn.texC, 0.0f, 1.0f), gTexMtx);

	// Set texture to be sampled
	vOut.texID = vIn.texID;
	vOut.projTexC = mul( float4( vIn.posL, 1.0f ), gLightWVP );

	float d = distance( vOut.posW, gEyePosW );
	vOut.fogLerp = saturate( ( d - gFogStart ) / gFogRange );
	
	return vOut;
}


float4 PS(VS_OUT pIn) : SV_Target
{
	uint texID = pIn.texID * 3;

	// Get materials from texture maps.
	float4 diffuse = gTextureArray.Sample( gAnisotropic, float3( pIn.texC, texID ) );
	float4 spec    = gTextureArray.Sample( gAnisotropic, float3( pIn.texC, texID + 1 ) );
	float4 normalT = gTextureArray.Sample( gAnisotropic, float3( pIn.texC, texID + 2 ) );
	
	// Map [0,1] --> [0,256]
	spec.a *= 256.0f;

	// Uncompress
	normalT = 2.0f * normalT - 1.0f;

	// build orthonormal basis
	float3 N = normalize( pIn.normalW );
	float3 T = normalize( pIn.tangentW - dot( pIn.tangentW, N) * N );
	float3 B = cross( N, T );

	float3x3 TBN = float3x3( T, B, N );

	float3 bumpedNormalW = normalize( mul( normalT, TBN ) );
	    
	float shadowFactor = CalculateShadowFactor( pIn.projTexC );
	// Compute the lit color for this pixel.
    SurfaceInfo v = {pIn.posW, bumpedNormalW, diffuse, spec};
	//float3 litColor = PointLight(v, gLight, gEyePosW);
	//float3 litColor = Spotlight(v, gLight, gEyePosW);
	float3 litColor = ParallelLight(v, gLight, gEyePosW,shadowFactor);

	float3 foggedColor = lerp( litColor, gFogColor, pIn.fogLerp );
    
    return float4(foggedColor, diffuse.a);
}

technique10 StandardTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
