// Local Includes
#include "services.h"

// This Include 
#include "particle_system.h"

// Implemtentation
CParticles::CParticles() :
	m_bFirst( false ), m_fGameTime( 0.0f ), m_fTimeStep( 0.0f ), m_fAge( 0.0f ),
	m_vEyePositionW( 0.0f, 0.0f, 0.0f, 0.0f ), m_vEmitPositionW( 0.0f, 0.0f, 0.0f, 0.0f ), 
	m_vEmitDirectionW( 0.0f, 1.0f, 0.0f, 0.0f ), m_pDevice( nullptr ), m_pIVertexBuffer( nullptr ),
	m_pDVertexBuffer( nullptr ), m_pSVertexBuffer( nullptr ), m_RandomTextureRV( nullptr ), 
	m_ParticleFX( nullptr ), m_PVertexLayout( nullptr )
{}

CParticles::~CParticles()
{
	ReleaseCOM( m_ParticleFX );

	ReleaseCOM( m_PVertexLayout );

	ReleaseCOM( m_pIVertexBuffer );
	ReleaseCOM( m_pDVertexBuffer );
	ReleaseCOM( m_pSVertexBuffer );
}

float CParticles::GetAge() const
{
	return ( m_fAge );
}

void CParticles::SetEyePosition( const D3DXVECTOR3& _vPosition )
{
	m_vEyePositionW = D3DXVECTOR4( _vPosition.x, _vPosition.y, _vPosition.z, 1.0f );
}

void CParticles::SetEmitPosition( const D3DXVECTOR3& _vPosition )
{
	m_vEmitPositionW = D3DXVECTOR4( _vPosition.x, _vPosition.y, _vPosition.z, 1.0f );
}

void CParticles::SetEmitDirection( const D3DXVECTOR3& _vDirection )
{
	m_vEmitDirectionW = D3DXVECTOR4( _vDirection.x, _vDirection.y, _vDirection.z, 1.0f );
}

void CParticles::Initialise()
{	
	// Get device from services
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	m_pDevice = static_cast< ID3D10Device* >( vpDevice );

	// Create link to fx file
	BuildFX();
	BuildVertexLayout();
	CreateTexture();

	// Fill vertex buffer description
	D3D10_BUFFER_DESC vbDesc;
	vbDesc.Usage			= D3D10_USAGE_DEFAULT;
	vbDesc.ByteWidth		= sizeof( PVertex );
	vbDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags	= 0;
	vbDesc.MiscFlags		= 0;

	// Create initial vertex
	PVertex p;
	ZeroMemory( &p, sizeof( PVertex ) );
	p.age	= 0.0f;
	p.type	= 0;

	// Set vertex to memory
	D3D10_SUBRESOURCE_DATA viData;
	viData.pSysMem = &p;

	// Create vertex buffer
	HR( m_pDevice->CreateBuffer( &vbDesc, &viData, &m_pIVertexBuffer ) ); 

	// Create "Ping-pong" buffers
	vbDesc.ByteWidth = sizeof( PVertex ) * g_ParticleSystemMaxParticles;
	vbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;

	HR( m_pDevice->CreateBuffer( &vbDesc, 0, &m_pDVertexBuffer ) );
	HR( m_pDevice->CreateBuffer( &vbDesc, 0, &m_pSVertexBuffer ) );
}

void CParticles::BuildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile( L"Assets/Shaders/rain_particles.fx", 0, 0, "fx_4_0", shaderFlags, 0,
		m_pDevice, 0, 0, &m_ParticleFX, &compilationErrors, 0 );

	if( FAILED( hr ) )
	{
		if( compilationErrors )
		{
			MessageBoxA( 0, ( char* )compilationErrors->GetBufferPointer(), 0, 0 );
			ReleaseCOM( compilationErrors );
		}
		DXTrace( __FILE__, ( DWORD ) __LINE__, hr, L"D3DX10CreateEffectFromFile", true );
	}

	m_StreamTech	= m_ParticleFX->GetTechniqueByName( "SOTech" );
	m_DrawTech		= m_ParticleFX->GetTechniqueByName( "DrawTech" );
		
	m_fxViewProjVar			= m_ParticleFX->GetVariableByName( "gViewProj" )->AsMatrix();
	m_fxGameTimeVar			= m_ParticleFX->GetVariableByName( "gGameTime" )->AsScalar();
	m_fxTimeStepVar			= m_ParticleFX->GetVariableByName( "gTimeStep" )->AsScalar();
	m_fxEyePositionVar		= m_ParticleFX->GetVariableByName( "gEyePositionW" )->AsVector();
	m_fxEmitPositionVar		= m_ParticleFX->GetVariableByName( "gEmitPositionW" )->AsVector();
	m_fxEmitDirectionVar	= m_ParticleFX->GetVariableByName( "gEmitDirectionW" )->AsVector();
	m_fxRandomTextureVar	= m_ParticleFX->GetVariableByName( "gRandomTexture" )->AsShaderResource();
}

void CParticles::BuildVertexLayout()
{
	D3D10_INPUT_ELEMENT_DESC pvDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D10_PASS_DESC pDesc;
	m_ParticleFX->GetTechniqueByName( "SOTech" )->GetPassByIndex( 0 )->GetDesc( &pDesc );
	HR( m_pDevice->CreateInputLayout( pvDesc, 5, pDesc.pIAInputSignature, pDesc.IAInputSignatureSize, &m_PVertexLayout ) );
}

void CParticles::CreateTexture()
{
	// Define container
	D3DXVECTOR4 vRand[ 1024 ];

	// Generate random unit vectors
	for( int i = 0; i < 1024; ++i )
	{
		vRand[ i ].x = ( static_cast< float >( rand() % 2000 ) / 1000.0f ) - 1.0f;
		vRand[ i ].y = ( static_cast< float >( rand() % 2000 ) / 1000.0f ) - 1.0f;
		vRand[ i ].z = ( static_cast< float >( rand() % 2000 ) / 1000.0f ) - 1.0f;
		vRand[ i ].w = ( static_cast< float >( rand() % 2000 ) / 1000.0f ) - 1.0f;
	}

	// Assign array to memory for creation
	D3D10_SUBRESOURCE_DATA iData;
	iData.pSysMem			= vRand;
	iData.SysMemPitch		= 1024 * sizeof( D3DXVECTOR4 );
	iData.SysMemSlicePitch	= 2024 * sizeof( D3DXVECTOR4 );

	// Fill texture description
	D3D10_TEXTURE1D_DESC tDesc;
	tDesc.Width				= 1024; 
	tDesc.MipLevels			= 1;
	tDesc.Format			= DXGI_FORMAT_R32G32B32A32_FLOAT;
	tDesc.Usage				= D3D10_USAGE_IMMUTABLE;
	tDesc.BindFlags			= D3D10_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags	= 0;
	tDesc.MiscFlags			= 0;
	tDesc.ArraySize			= 1;

	// Create texture
	ID3D10Texture1D* RandomTexture = nullptr;
	HR( m_pDevice->CreateTexture1D( &tDesc, &iData, &RandomTexture ) );

	// Fill shader resource description
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format						= tDesc.Format;
	srDesc.ViewDimension				= D3D10_SRV_DIMENSION_TEXTURE1D;
	srDesc.Texture1D.MipLevels			= tDesc.MipLevels;
	srDesc.Texture1D.MostDetailedMip	= 0;

	// Create shader resource
	HR( m_pDevice->CreateShaderResourceView( RandomTexture, &srDesc, &m_RandomTextureRV ) );

	// Clean up
	ReleaseCOM( RandomTexture );
}

void CParticles::Reset()
{
	m_bFirst	= true;
	m_fAge		= 0.0f;
}

void CParticles::Update( float _fDT, float _fGT )
{
	m_fGameTime = _fGT;
	m_fTimeStep = _fDT;
	m_fAge += _fDT;
}

void CParticles::Draw( D3DXMATRIX _matView, D3DXMATRIX _matProj )
{
	// Set Constants
	m_fxViewProjVar->SetMatrix( ( float* ) &( _matView * _matProj ) );		
	m_fxGameTimeVar->SetFloat( m_fGameTime );
	m_fxTimeStepVar->SetFloat( m_fTimeStep );
	m_fxEyePositionVar->SetFloatVector( ( float* ) &m_vEyePositionW );
	m_fxEmitPositionVar->SetFloatVector( ( float* ) &m_vEmitPositionW );
	m_fxEmitDirectionVar->SetFloatVector( ( float* ) &m_vEmitDirectionW );
	m_fxRandomTextureVar->SetResource( m_RandomTextureRV );

	// Set IA Stage
	m_pDevice->IASetInputLayout( m_PVertexLayout );
	m_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	// Stride/offset
	UINT stride = sizeof( PVertex );
	UINT offset = 0;

	// Set Vertex buffer depending if initialised
	m_pDevice->IASetVertexBuffers( 0, 1, &( m_bFirst ? m_pIVertexBuffer : m_pDVertexBuffer ), &stride, &offset );

	// Draw to stream out for particle creation
	m_pDevice->SOSetTargets( 1, &m_pSVertexBuffer, &offset );

	D3D10_TECHNIQUE_DESC techDesc;
	m_StreamTech->GetDesc( &techDesc );

	for( UINT i = 0; i < techDesc.Passes; ++i )
	{
		m_StreamTech->GetPassByIndex( i )->Apply( 0 );

		// Draw appropriate buffer depending on initialised
		if( m_bFirst )
		{
			m_pDevice->Draw( 1, 0 );
			m_bFirst = false;
		}
		else
		{
			m_pDevice->DrawAuto();
		}
	}

	// Unbind vertex buffer
	ID3D10Buffer* bufferArray[ 1 ] = { 0 };
	m_pDevice->SOSetTargets( 1, bufferArray, &offset );

	// "Ping-pong" buffers
	std::swap( m_pDVertexBuffer, m_pSVertexBuffer );

	// Draw updated particles recieved from stream out
	m_pDevice->IASetVertexBuffers( 0, 1, &m_pDVertexBuffer, &stride, &offset );

	m_DrawTech->GetDesc( &techDesc );
	for( UINT i = 0; i < techDesc.Passes; ++i )
	{
		m_DrawTech->GetPassByIndex( i )->Apply( 0 );

		m_pDevice->DrawAuto();
	}
}