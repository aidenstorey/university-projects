//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "viewport.h"

// This Include
#include "camera.h"

// Implementation
CCamera::CCamera() :
	m_vPosition( 0.0f, 0.0f, 0.0f ), m_vTarget( 0.0f, 0.0f, 1.0f ), m_vUp( 0.0f, 1.0f, 0.0f ),
	m_fFOV( 45.0f ), m_fNearPlane( 1.0f ), m_fFarPlane( 100.0f ),
	m_eMode( CAMERAMODE_PERSPECTIVE ), m_pViewport( nullptr )
{
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matProj );
}

CCamera::~CCamera() 
{}

D3DXMATRIX CCamera::GetViewMatrix() const
{
	return ( m_matView );
}

void CCamera::SetViewMatrix( const D3DXMATRIX _matView )
{
	m_matView = _matView;
}

D3DXMATRIX CCamera::GetProjMatrix() const
{
	return ( m_matProj );
}

void CCamera::SetProjMatrix( const D3DXMATRIX _matProj )
{
	m_matProj = _matProj;
}

D3DXMATRIX CCamera::GetViewProjMatrix() const
{
	return( m_matView * m_matProj );
}

D3DXVECTOR3 CCamera::GetPosition() const
{
	return ( m_vPosition );
}

void CCamera::SetPosition( const D3DXVECTOR3 _vPosition )
{
	m_vPosition = _vPosition;
	ResetViewMatrix();
}

D3DXVECTOR3 CCamera::GetTarget() const
{
	return ( m_vTarget );
}

void CCamera::SetTarget( const D3DXVECTOR3 _vTarget )
{
	m_vTarget = _vTarget;
	ResetViewMatrix();
}

D3DXVECTOR3 CCamera::GetUp() const
{
	return ( m_vUp );
}

void CCamera::SetUp( const D3DXVECTOR3 _vUp )
{
	m_vUp = _vUp;
	ResetViewMatrix();
}

float CCamera::GetNearPlane() const
{
	return ( m_fNearPlane );
}

void CCamera::SetNearPlane( const float _fNear )
{
	m_fNearPlane = _fNear;
	ResetProjMatrix();
}

float CCamera::GetFarPlane() const
{
	return ( m_fFarPlane );
}

void CCamera::SetFarPlane( const float _fFar )
{	
	m_fFarPlane = _fFar;
	ResetProjMatrix();
}

void CCamera::SetClipPlanes( const float _fNear, const float _fFar )
{
	m_fNearPlane	= _fNear;
	m_fFarPlane		= _fFar;
	ResetProjMatrix();
}

float CCamera::GetScale() const
{
	return ( m_fScale );
}

void CCamera::SetScale( const float _fScale )
{
	m_fScale = 1.0f / _fScale;
}

CViewport* CCamera::GetViewport() const
{
	return ( m_pViewport );
}

ECameraMode CCamera::GetCameraMode() const
{
	return ( m_eMode );
}

void CCamera::SetCameraMode( const ECameraMode _eMode )
{
	m_eMode = _eMode;
	ResetProjMatrix();
}

void CCamera::Initialise( CViewport* _pViewport, float _fFOV, ECameraMode _eMode, float _fNearClip, float _fFarClip, float _fScale, unsigned int _uiID )
{
	// Store local copy of parameters
	m_uiID			= _uiID;
	m_pViewport		= _pViewport;
	m_fFOV			= _fFOV;
	m_fNearPlane	= _fNearClip;
	m_fFarPlane		= _fFarClip;
	m_fScale		= 1.0f / _fScale;
	m_eMode			= _eMode;

	ResetViewMatrix();
	ResetProjMatrix();
}

void CCamera::ResetViewMatrix()
{
	D3DXMatrixLookAtLH( &m_matView, &m_vPosition, &m_vTarget, &m_vUp );
}

void CCamera::ResetProjMatrix()
{
	switch( m_eMode )
	{
	case CAMERAMODE_ORTHOGONAL:
		{
			D3DXMatrixOrthoLH( &m_matProj, static_cast< float >( m_pViewport->GetWidth() ) * m_fScale, static_cast< float >( m_pViewport->GetHeight() ) * m_fScale, m_fNearPlane, m_fFarPlane );
		}
		break;

	case CAMERAMODE_PERSPECTIVE:
		{
			D3DXMatrixPerspectiveFovLH( &m_matProj, static_cast< float >( D3DXToRadian( m_fFOV ) ), static_cast< float >( m_pViewport->GetWidth() ) / 
				static_cast< float >( m_pViewport->GetHeight() ), m_fNearPlane, m_fFarPlane );
		}
		break;

	default: break;
	}
}