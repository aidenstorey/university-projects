// Local Includes
#include <ctime>

// This Include
#include "controlled_light.h"
#include "services.h"

// Implementation
CControlledLight::CControlledLight() :
	m_vFocus( 0.0f, 0.0f, 0.0f ), m_fSunTime( 0.0f ), m_fMoonTime( 0.0f ), m_bRaining ( false ), m_bStartedRaining( false ),
	m_bDayTime( false ), m_vSunPos( 0.0f, 0.0f, 0.0f ), m_vMoonPos( 0.0f, 0.0f, 0.0f ), m_pKeyboard( nullptr ), m_eTimeState( TS_DAY ),
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ), m_fxTextureArray( nullptr ), m_BVertexLayout( nullptr ), m_fTimeSinceMoonUp( 0.0f )
{
	D3DXMatrixIdentity( &m_matSunView );
	D3DXMatrixIdentity( &m_matMoonView );	
}

CControlledLight::~CControlledLight()
{
	ReleaseCOM( m_BillboardFX );

	ReleaseCOM( m_BVertexLayout );
	ReleaseCOM( m_pVertexBuffer );
}

Light CControlledLight::GetLight()
{
	return ( m_bDayTime ? m_SunLight : m_MoonLight );
}

D3DXVECTOR3 CControlledLight::GetFocus() const
{
	return ( m_vFocus );
}

void CControlledLight::SetFocus( const D3DXVECTOR3 _vFocus )
{
	m_vFocus = _vFocus;
}

D3DXVECTOR3	CControlledLight::GetDirection() const
{
	return ( m_bDayTime ? m_SunLight.dir : m_MoonLight.dir );
}

void CControlledLight::SetDirection( const D3DXVECTOR3 _vDirection )
{
	m_bDayTime ? m_SunLight.dir = _vDirection : m_MoonLight.dir = _vDirection;
}

D3DXMATRIX CControlledLight::GetViewMatrix() const
{
	return ( m_bDayTime ? m_matSunView : m_matMoonView );
}

D3DXMATRIX CControlledLight::GetVolume() const
{
	return ( m_matLightVolume );
}

bool CControlledLight::IsRaining()
{
	return ( m_bRaining );
}

bool CControlledLight::IsStartedRaining()
{
	return ( m_bStartedRaining );
}

D3DXCOLOR CControlledLight::GetSkyColor() const
{
	return ( m_vSkyColor );
}

void CControlledLight::Initialise()
{
	// Get device from services
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	m_pDevice = static_cast< ID3D10Device* >( vpDevice );

	// Get device from services
	void* vpKeyboard;
	CServices::GetService( SERVICETYPE_KEYBOARD, vpKeyboard );
	m_pKeyboard = static_cast< CKeyboardInput* >( vpKeyboard );

	// Get start time of program
	std::time_t InitTime = time ( 0 );
	struct tm StartTime;
	localtime_s( &StartTime, &InitTime );

	// Caclulate time in seconds
	m_fSunTime = m_fMoonTime = static_cast< float > ( ( StartTime.tm_hour * g_TimeMPH + StartTime.tm_min ) * g_TimeMPH + StartTime.tm_sec );

	// Determine if day time
	m_bDayTime = m_fSunTime >= g_TimeSR && m_fSunTime <= g_TimeSS;

	// Calculate light positions
	m_vSunPos	= CalculatePosition( g_LightSunRadius, CalculateSunAngle( m_fSunTime ) );	
	m_vMoonPos	= CalculatePosition( g_LightMoonRadius, CalculateMoonAngle( m_fMoonTime ) );

	// Calculate time state 
	m_eTimeState = CalculateTimeState( m_fSunTime );

	// Create lights
	ZeroMemory( &m_SunLight, sizeof( Light ) );
	m_SunLight.ambient	= D3DXCOLOR( 0.4f, 0.4f, 0.4f, 1.0f );
	m_SunLight.diffuse	= D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
	m_SunLight.specular	= D3DXCOLOR( 0.1f, 0.1f, 0.1f, 1.0f );
	D3DXVec3Normalize( &m_SunLight.dir, &( m_vFocus - m_vSunPos ) );	

	ZeroMemory( &m_MoonLight, sizeof( Light ) );
	m_MoonLight.ambient		= D3DXCOLOR( 0.3f, 0.3f, 0.3f, 1.0f );
	m_MoonLight.diffuse		= D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.0f );
	m_MoonLight.specular	= D3DXCOLOR( 0.4f, 0.4f, 0.4f, 1.0f );
	D3DXVec3Normalize( &m_MoonLight.dir, &( m_vFocus - m_vMoonPos ) );

	D3DXMatrixOrthoLH( &m_matLightVolume, 1024.0f, 1024.0f, -1000.0f, 1000.0f );
	D3DXMatrixLookAtLH( &m_matSunView, &m_SunLight.pos,&m_SunLight.dir, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	D3DXMatrixLookAtLH( &m_matMoonView, &m_MoonLight.pos,&m_MoonLight.dir, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

	InitialiseGraphics();
}

void CControlledLight::InitialiseGraphics()
{
	// Initialise appropriate devices
	BuildFX();
	BuildVertexLayout();
	LoadTextures();

	// Create vertices for sun/moon billboard
	m_SunMoon[ 0 ].pos	= m_vSunPos;
	m_SunMoon[ 0 ].size = D3DXVECTOR2( 50.0f, 50.0f );
	m_SunMoon[ 0 ].texID = 0;

	m_SunMoon[ 1 ].pos	= m_vMoonPos;
	m_SunMoon[ 1 ].size = D3DXVECTOR2( 10.0f, 10.0f );
	m_SunMoon[ 1 ].texID = 1;

	// Fill vertex buffer description
	D3D10_BUFFER_DESC vbDesc;
	vbDesc.Usage			= D3D10_USAGE_DYNAMIC;
	vbDesc.ByteWidth		= sizeof( VBillboard ) * 2;
	vbDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags		= 0;

	// Set vertices to memory
	D3D10_SUBRESOURCE_DATA viData;
	viData.pSysMem = m_SunMoon;

	// Create device
	HR( m_pDevice->CreateBuffer( &vbDesc, &viData, &m_pVertexBuffer ) );
}

float CControlledLight::CalculateSunAngle( float _fTime )
{
	float fReturn = PI;

	if( _fTime >= g_TimeSRStart && _fTime <= g_TimeSSFinish )
	{
		float fTimeSinceSR = _fTime - g_TimeSRStart;
		fReturn = ( PI * 1.5f ) + ( ( PI / ( g_TimeSSFinish - g_TimeSRStart ) ) * fTimeSinceSR );
	}

	return ( fReturn );
}

float CControlledLight::CalculateMoonAngle( float _fTime )
{
	float fReturn = PI;

	if( _fTime > 0.0f && _fTime < g_TimeMoonUp )
	{
		fReturn = ( PI * 1.5f ) + ( ( PI / g_TimeMoonUp ) * _fTime );
	}

	return ( fReturn );
}

D3DXVECTOR3 CControlledLight::CalculatePosition( float _fRadius, float _fAngle )
{
	D3DXVECTOR3 vReturn;

	vReturn.x = _fRadius * sinf( _fAngle ) * sinf( g_LightTheta );
	vReturn.y = _fRadius * cosf( _fAngle );
	vReturn.z = -_fRadius * sinf( _fAngle ) * cosf( g_LightTheta );

	return ( vReturn );
}

D3DXCOLOR CControlledLight::CalculateSkyColor( float _fTime )
{
	D3DXCOLOR vReturn = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );

	switch( m_eTimeState )
	{
	case TS_DAY:
		{
			vReturn = g_LightColorDay;
		}
		break;

	case TS_NIGHT:
		{
			vReturn = g_LightColorNight;
		}
		break;

	case TS_DAYIN:
		{
			D3DXCOLOR vTime = ( ( g_LightColorDay - g_LightColorDusk ) / g_TimeTransition ) * ( _fTime - g_TimeSR );
			vReturn = g_LightColorDusk + vTime;
		}
		break;  

	case TS_DAYOUT:
		{
			D3DXCOLOR vTime = ( ( g_LightColorDusk - g_LightColorDay ) / g_TimeTransition ) * ( _fTime - g_TimeSSStart );
			vReturn = g_LightColorDay + vTime;
		}
		break;

	case TS_NIGHTIN:
		{
			D3DXCOLOR vTime = ( ( g_LightColorNight - g_LightColorDusk ) / g_TimeTransition ) * ( _fTime - g_TimeSS );
			vReturn = g_LightColorDusk + vTime;
		}
		break;

	case TS_NIGHTOUT:
		{
			D3DXCOLOR vTime = ( ( g_LightColorDusk - g_LightColorNight ) / g_TimeTransition ) * ( _fTime - g_TimeSRStart );
			vReturn = g_LightColorNight + vTime;
		}
		break;

	default: break;
	}

	if( m_bRaining )
	{
		vReturn -= D3DXCOLOR( 0.1f, 0.1f, 0.4f, 1.0f );
	}

	return ( vReturn );
}

ETimeState	CControlledLight::CalculateTimeState( float _fTime )
{
	ETimeState eReturn = TS_NIGHT;

	if( _fTime >= g_TimeSRStart && _fTime < g_TimeSR )
	{
		eReturn = TS_NIGHTOUT;
	}
	else if( _fTime >= g_TimeSR && _fTime < g_TimeSRFinish )
	{
		eReturn = TS_DAYIN;
	}
	else if( _fTime >= g_TimeSRFinish && _fTime < g_TimeSSStart )
	{
		eReturn = TS_DAY;
	}
	else if( _fTime >= g_TimeSSStart && _fTime < g_TimeSS )
	{
		eReturn = TS_DAYOUT;
	}
	else if( _fTime >= g_TimeSS && _fTime < g_TimeSSFinish )
	{
		eReturn = TS_NIGHTIN;
	}

	return ( eReturn );
}

void CControlledLight::Update( float _fDT )
{	
	m_bStartedRaining = false;

	// Progress sun time
	m_fSunTime += _fDT;
	if( m_fSunTime > g_TimeSPD )
	{
		m_fSunTime -= g_TimeSPD;
	}

	// Progress moon time
	m_fMoonTime += _fDT;
	if( m_fMoonTime > g_TimeSPD )
	{
		m_fMoonTime -= g_TimeSPD;
	}
		
	// Determine if day time
	m_bDayTime = m_fSunTime >= g_TimeSRStart && m_fSunTime <= g_TimeSSFinish;

	// Calculate sun position / light direction
	m_vSunPos	= CalculatePosition( g_LightSunRadius, CalculateSunAngle( m_fSunTime ) );
	D3DXVec3Normalize( &m_SunLight.dir, &( m_vFocus - m_vSunPos ) );
	m_SunMoon[ 0 ].pos = m_vSunPos;	

	// Calculate the state of day 
	m_eTimeState = CalculateTimeState( m_fSunTime );

	// Calculate sky color
	m_vSkyColor = CalculateSkyColor( m_fSunTime );

	if( m_eTimeState == TS_NIGHTOUT || m_eTimeState == TS_DAYIN )
	{
		D3DXCOLOR vTime = ( D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f ) / g_TimeTransition ) * ( m_fSunTime - g_TimeSRStart );
		m_MoonLight.diffuse = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f ) - vTime;
	}

	if( m_eTimeState == TS_DAYOUT || m_eTimeState == TS_NIGHTIN )
	{
		D3DXCOLOR vTime = ( D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f ) / g_TimeTransition ) * ( m_fSunTime - g_TimeSSStart );
		m_MoonLight.diffuse = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ) - vTime;
	}

	if( m_fMoonTime > g_TimeSSStart || m_fMoonTime < g_TimeSRFinish )
	{
		m_fTimeSinceMoonUp += _fDT;
	}
	else
	{
		m_fTimeSinceMoonUp = 0.0f;
	}

	// Calculate moon position / light direction
	m_vMoonPos	= CalculatePosition( g_LightMoonRadius, CalculateMoonAngle( m_fTimeSinceMoonUp ) );
	D3DXVec3Normalize( &m_MoonLight.dir, &( m_vFocus - m_vMoonPos ) );
 	m_SunMoon[ 1 ].pos = m_vMoonPos;

	// Toggle rain on / off 
	if( m_pKeyboard->KeyPressed( 0x54 ) ) // T
	{
		if( m_bRaining )
		{
			m_SunLight.specular	= D3DXCOLOR( 0.1f, 0.1f, 0.1f, 1.0f );
		}
		else
		{
			m_SunLight.specular	= D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f );
		}

		m_bRaining = !m_bRaining;
		m_bStartedRaining = true;
	}


	if( m_pKeyboard->KeyDown( 0x47 ) ) // G
	{
		m_fSunTime += g_LightUserAddition;
	}

	D3DXMatrixLookAtLH( &m_matSunView, &m_SunLight.pos,&m_SunLight.dir, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	D3DXMatrixLookAtLH( &m_matMoonView, &m_MoonLight.pos,&m_MoonLight.dir, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	MapBuffer();
}

void CControlledLight::Draw( D3DXMATRIX _matView, D3DXMATRIX _matProj, D3DXVECTOR3 _vEyePos )
{
	// Set Constants
	m_fxViewProjVar->SetMatrix( ( float* ) &( _matView * _matProj ) );		
	m_fxEyePositionVar->SetFloatVector( ( float* ) &_vEyePos );
	m_fxTextureArray->SetResource( m_TextureArrayRV );

	// Set IA state
	m_pDevice->IASetInputLayout( m_BVertexLayout );
	m_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	// Stride / offset
	UINT stride = sizeof( VBillboard );
	UINT offset = 0;

	// Set vertex buffer
	m_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	
	// Get tech demo
	D3D10_TECHNIQUE_DESC techDesc;
	m_BillboardTech->GetDesc( &techDesc );

	for( UINT i = 0; i < techDesc.Passes; ++i )
	{
		m_BillboardTech->GetPassByIndex( i )->Apply( 0 );

		if( !m_bRaining )
		{
			m_pDevice->Draw( 2, 0 );
		}
	}
}

void CControlledLight::BuildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile( L"Assets/Shaders/billboard.fx", 0, 0, "fx_4_0", shaderFlags, 0,
		m_pDevice, 0, 0, &m_BillboardFX, &compilationErrors, 0 );

	if( FAILED( hr ) )
	{
		if( compilationErrors )
		{
			MessageBoxA( 0, ( char* )compilationErrors->GetBufferPointer(), 0, 0 );
			ReleaseCOM( compilationErrors );
		}
		DXTrace( __FILE__, ( DWORD ) __LINE__, hr, L"D3DX10CreateEffectFromFile", true );
	}

	m_BillboardTech	= m_BillboardFX->GetTechniqueByName( "BillboardTech" );
		
	m_fxViewProjVar			= m_BillboardFX->GetVariableByName( "gViewProj" )->AsMatrix();
	m_fxEyePositionVar		= m_BillboardFX->GetVariableByName( "gEyePositionW" )->AsVector();
	m_fxTextureArray		= m_BillboardFX->GetVariableByName( "gTextureArray" )->AsShaderResource();
}

void CControlledLight::BuildVertexLayout()
{
	D3D10_INPUT_ELEMENT_DESC pvDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXID", 0, DXGI_FORMAT_R32_UINT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D10_PASS_DESC pDesc;
	m_BillboardFX->GetTechniqueByName( "BillboardTech" )->GetPassByIndex( 0 )->GetDesc( &pDesc );
	HR( m_pDevice->CreateInputLayout( pvDesc, 3, pDesc.pIAInputSignature, pDesc.IAInputSignatureSize, &m_BVertexLayout ) );
}

void CControlledLight::LoadTextures()
{
	const int iNumTextures = 2;

	// Textures to be used
	std::wstring fileNames[ iNumTextures ] =
	{
		L"Assets/Textures/Surface/sun.dds",		
		L"Assets/Textures/Surface/moon.dds",
	};

	// Load textures from file
	ID3D10Texture2D* textures[ iNumTextures ];
	for( int i = 0; i < iNumTextures; ++i )
	{
		D3DX10_IMAGE_LOAD_INFO loadInfo;

		loadInfo.Width			= D3DX10_FROM_FILE;
		loadInfo.Height			= D3DX10_FROM_FILE;
		loadInfo.Depth			= D3DX10_FROM_FILE;
		loadInfo.FirstMipLevel	= 0;
		loadInfo.MipLevels		= D3DX10_FROM_FILE;
		loadInfo.Usage			= D3D10_USAGE_STAGING;
		loadInfo.BindFlags		= 0;
		loadInfo.CpuAccessFlags	= D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
		loadInfo.MiscFlags		= 0;
		loadInfo.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter			= D3DX10_FILTER_NONE;
		loadInfo.MipFilter		= D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo		= 0;

		HR( D3DX10CreateTextureFromFile( m_pDevice, fileNames[ i ].c_str(), &loadInfo, 
			0, ( ID3D10Resource** )&textures[ i ], 0 ) );
	}

	// Get texture description
	D3D10_TEXTURE2D_DESC textElementDesc;
	textures[ 0 ]->GetDesc( &textElementDesc );

	// Fill texture array description
	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width				= textElementDesc.Width;
	texArrayDesc.Height				= textElementDesc.Height;
	texArrayDesc.MipLevels			= textElementDesc.MipLevels;
	texArrayDesc.ArraySize			= iNumTextures;
	texArrayDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count	= 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage				= D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags			= D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags		= 0;
	texArrayDesc.MiscFlags			= 0;

	// Create texture array
	ID3D10Texture2D* textArray = nullptr;
	HR( m_pDevice->CreateTexture2D( &texArrayDesc, NULL, &textArray) );

	// Put textures into array
	for( int i = 0; i < iNumTextures; ++i )
	{
		for( unsigned j = 0; j < textElementDesc.MipLevels; ++j )
		{
			D3D10_MAPPED_TEXTURE2D mappedText2D;
			textures[ i ]->Map( j, D3D10_MAP_READ, 0, &mappedText2D );

			m_pDevice->UpdateSubresource( textArray, D3D10CalcSubresource( j, i, textElementDesc.MipLevels ),
				0, mappedText2D.pData, mappedText2D.RowPitch, 0 );
			
			textures[ i ]->Unmap( j );
		}
	}

	// Fill shader resource descrition
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension	= D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = iNumTextures;

	// Create Texture array resource view
	HR( m_pDevice->CreateShaderResourceView( textArray, &viewDesc, &m_TextureArrayRV ) );

	ReleaseCOM( textArray );
	for( int i = 0; i < iNumTextures; ++i )
	{
		ReleaseCOM( textures[ i ] );
	}
}

void CControlledLight::MapBuffer()
{
	VBillboard* pBillboard = nullptr;
	m_pVertexBuffer->Map( D3D10_MAP_WRITE_NO_OVERWRITE, 0, ( void** ) &pBillboard );

	for( unsigned i = 0; i < 2; ++i )
	{
		pBillboard[ i ].pos = m_SunMoon[ i ].pos;
	}

	m_pVertexBuffer->Unmap();
}