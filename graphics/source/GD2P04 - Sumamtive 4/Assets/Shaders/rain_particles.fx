cbuffer cbPerFrame
{
	float4 gEyePositionW;

	float4 gEmitPositionW;
	float4 gEmitDirectionW;

	float gGameTime;
	float gTimeStep;
	float4x4 gViewProj;
};

cbuffer cbFixed
{
	float3 gAccelerationW = { -1.0f, -9.8f, 0.0f };
}

Texture1D gRandomTexture;

SamplerState gTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

// Creates random vector between -1 and 1from sampling texture
float3 RandomUintVec3( float offset )
{
	// Sample random texture
	float u = ( gGameTime + offset );

	// coords
	float3 v = gRandomTexture.SampleLevel( gTriLinearSam, u, 0 );

	return ( normalize( v ) );
}

float3 RandomVec3( float offset )
{
	// Sample random texture
	float u = ( gGameTime + offset );

	// coords
	float3 v = gRandomTexture.SampleLevel( gTriLinearSam, u, 0 );

	return ( v );
}


// Stream out

struct Particle
{
	float3	posW	: POSITION;
	float3	velW	: VELOCITY;
	float2	sizeW	: SIZE;
	float	age		: AGE;
	uint	type	: TYPE;
};

Particle SO_VS( Particle vIn )
{
	return ( vIn );
}

[ maxvertexcount( 6 ) ]
void SO_GS( point Particle gIn[ 1 ], inout PointStream< Particle > pointStream )
{
	gIn[ 0 ].age += gTimeStep;

	if( gIn[ 0 ].type == 0 ) // initialiser particle
	{
		// Checks emitter ready to create new particle
		if( gIn[ 0 ].age > 0.001f )
		{
			// Loops to create new particles
			for( int i = 0; i < 5; ++i )
			{
				float3 fRand = 20.0f * RandomVec3( ( float ) i / 5.0f ); // TODO:	Look into scalar value
				fRand.y = 10.0f;

				Particle p;
				p.posW	= gEmitPositionW.xyz + fRand;
				p.velW	= float3( 0.0f, 0.0f, 0.0f );
				p.sizeW	= float2( 1.0f, 1.0f );
				p.age	= 0.0f;
				p.type	= 1;

				pointStream.Append( p );
			}

			// Reset age to wait for emit
			gIn[ 0 ].age = 0.0f;
		}

		// Maintains emitter
		pointStream.Append( gIn[ 0 ] );
	}
	else
	{
		if( gIn[ 0 ].age <= 2.0f )
		{
			pointStream.Append( gIn[ 0 ] );
		}
	}
}

GeometryShader gsStreamOut = ConstructGSWithSO( CompileShader( gs_4_0, SO_GS() ), "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x" );

technique10 SOTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, SO_VS() ) );
		SetGeometryShader( gsStreamOut );
		SetPixelShader( NULL );
		SetDepthStencilState( DisableDepth, 0 );
	}
}

// Draw tech

struct GS_IN
{
	float3	posW	: POSITION;
	uint	type	: TYPE;
};

struct PS_IN
{
	float4 posH	: SV_Position;
};

GS_IN D_VS( Particle vIn )
{
	GS_IN gIn;
	
	float fAge = vIn.age;

	// Constant acceleration equation....
	gIn.posW = 0.5f * fAge * fAge * gAccelerationW + fAge * vIn.velW + vIn.posW;
	gIn.type = vIn.type;

	return ( gIn );
}

[ maxvertexcount( 2 ) ]
void D_GS( point GS_IN gIn[ 1 ], inout LineStream< PS_IN > lineStream )
{
	// Wont draw if emitter
	if( gIn[ 0 ].type != 0 )
	{
		PS_IN pInB;
		pInB.posH = mul( float4( gIn[ 0 ].posW, 1.0f ), gViewProj );
		lineStream.Append( pInB );

		PS_IN pInE;
		pInE.posH = mul( float4( gIn[ 0 ].posW + 0.07f * gAccelerationW, 1.0f ), gViewProj );
		lineStream.Append( pInE );
	}
}

float4 D_PS( PS_IN pIn ) : SV_TARGET
{
	return ( float4( 0.6f, 0.6f, 1.0f, 1.0f ) );
}

technique10 DrawTech
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, D_VS() ) );
		SetGeometryShader( CompileShader( gs_4_0, D_GS() ) );
		SetPixelShader( CompileShader( ps_4_0, D_PS() ) );

		SetDepthStencilState( NoDepthWrites, 0 );
	}
}