//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "camera.h"

// This Include 
#include "viewport.h"

// Implementation
CViewport::CViewport() :
	m_vClearColor( 0.0f, 0.0f, 0.0f, 1.0f ),
	m_pActiveCamera( nullptr )
{}

CViewport::~CViewport()
{
	for ( auto& aCamera : m_mapCameras )
	{
		delete aCamera.second;
	}
}

CCamera* CViewport::GetCamera( unsigned int _uiCameraID )
{
	CCamera* pReturn = nullptr;

	if( _uiCameraID < m_mapCameras.size() )
	{
		pReturn = m_mapCameras[ _uiCameraID ];
	}

	return ( pReturn );
}

bool CViewport::SetActiveCamera( unsigned int _uiCameraID )
{
	bool bReturn = false;

	if( _uiCameraID < m_mapCameras.size() )
	{
		m_pActiveCamera = m_mapCameras[ _uiCameraID ];
		bReturn = true;
	}

	return ( bReturn );
}

CCamera* CViewport::GetActiveCamera()
{
	return ( m_pActiveCamera );
}

D3D10_VIEWPORT* CViewport::GetD3DViewport()
{
	return ( &m_D3DViewport );
}

void CViewport::SetViewport( const D3D10_VIEWPORT&	_D3DViewport )
{
	m_D3DViewport = _D3DViewport;
}

unsigned int CViewport::GetX() const
{
	return ( m_D3DViewport.TopLeftX );
}

void CViewport::SetX( const unsigned int _uiX )
{
	m_D3DViewport.TopLeftX = _uiX;
}

unsigned int CViewport::GetY() const
{
	return ( m_D3DViewport.TopLeftY );
}

void CViewport::SetY( const unsigned int _uiY )
{
	m_D3DViewport.TopLeftY = _uiY;
}

unsigned int CViewport::GetWidth() const
{
	return ( m_D3DViewport.Width );
}

void CViewport::SetWidth( const unsigned int _uiWidth )
{
	m_D3DViewport.Width = _uiWidth;
}

unsigned int CViewport::GetHeight() const
{
	return ( m_D3DViewport.Height );
}

void CViewport::SetHeight( const unsigned int _uiHeight )
{
	m_D3DViewport.Height = _uiHeight;
}

D3DXCOLOR CViewport::GetClearColor() const
{
	return ( m_vClearColor );
}

void CViewport::SetClearColor( const D3DXCOLOR _vColor )
{
	m_vClearColor = _vColor;
}

void CViewport::Initialise( const unsigned int _uiX, const unsigned int _uiY, const unsigned int _uiWidth, 
	const unsigned int _uiHeight, D3DXCOLOR _vClearColor )
{
	// Store local copy of parameter
	m_vClearColor	= _vClearColor;

	// Store parameter into viewport
	m_D3DViewport.Width		= _uiWidth;
	m_D3DViewport.Height	= _uiHeight;
	m_D3DViewport.TopLeftX	= _uiX;
	m_D3DViewport.TopLeftY	= _uiY;
	m_D3DViewport.MinDepth	= 0.0f;
	m_D3DViewport.MaxDepth	= 1.0f;
}

CCamera* CViewport::CreateCamera( float _fFOV, ECameraMode _eMode, float _fNearClip, float _fFarClip, float _fScale, unsigned int& _ruiCameraID )
{
	CCamera* pCamera = new CCamera();

	if( pCamera != nullptr )
	{
		pCamera->Initialise( this, _fFOV, _eMode, _fNearClip, _fFarClip, _fScale, _ruiCameraID );
		m_mapCameras.insert( std::make_pair( _ruiCameraID, pCamera ) );
	}

	if( m_pActiveCamera == nullptr )
	{
		m_pActiveCamera = pCamera;
	}

	return ( pCamera );
}