// This Include
#include "camera.h"

// Implementation
CCamera::CCamera() : 
	m_vEyePos( 0.0f, 0.0f, 0.0f ), 
	m_vLookAt( 0.0f, 0.0f, 0.0f ),
	m_vUp( 0.0f, 0.0f, 0.0f )
{
	D3DXMatrixIdentity( &m_matView );
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

D3DXVECTOR3 CCamera::GetEyePosition() const
{
	return ( m_vEyePos );
}

void CCamera::SetEyePos( const D3DXVECTOR3 _vEyePos )
{
	m_vEyePos = _vEyePos;
}

D3DXVECTOR3 CCamera::GetLookAt() const
{
	return ( m_vLookAt );
}

void CCamera::SetLookAt( const D3DXVECTOR3 _vLookAt )
{
	m_vLookAt = _vLookAt;
}

D3DXVECTOR3 CCamera::GetUp() const
{
	return ( m_vUp );
}

void CCamera::SetUp( const D3DXVECTOR3 _vUp )
{
	m_vUp = _vUp;
}

void CCamera::Initialise( const D3DXVECTOR3 _vEyePos, const D3DXVECTOR3 _vLookAt, const D3DXVECTOR3 _vUp )
{
	// Store local copies of parameters
	m_vEyePos	= _vEyePos;
	m_vLookAt	= _vLookAt;
	m_vUp		= _vUp;

	// Get view matrix
	D3DXMatrixLookAtLH( &m_matView, &m_vEyePos, &m_vLookAt, &m_vUp );	
}

void CCamera::Update( float _fDT )
{
	D3DXMATRIX matRotAxis;
	D3DXVECTOR3 vDir;

	D3DXVec3Normalize( &vDir, &( m_vLookAt - m_vEyePos ) );	

	if( GetAsyncKeyState( VK_UP ) ||
		GetAsyncKeyState( 0x57 ) ) // w
	{
		m_vEyePos += ( vDir * 10 * _fDT );
		m_vLookAt += ( vDir * 10 * _fDT );
	}

	if( GetAsyncKeyState( VK_DOWN ) ||
		GetAsyncKeyState( 0x53 ) ) //s
	{
		m_vEyePos -= ( vDir * 10 * _fDT );
		m_vLookAt += ( vDir * 10 * _fDT );
	}

	if( GetAsyncKeyState( VK_LEFT ) ||
		GetAsyncKeyState( 0x41 ) ) // a
	{
		D3DXMatrixRotationAxis( &matRotAxis, &m_vUp, -_fDT * 1.5f );
		D3DXVec3TransformCoord( &vDir, &vDir, &matRotAxis );
		m_vLookAt = vDir + m_vEyePos;
	}

	if( GetAsyncKeyState( VK_RIGHT ) ||
		GetAsyncKeyState( 0x44 ) ) // d
	{
		D3DXMatrixRotationAxis( &matRotAxis, &m_vUp, _fDT * 1.5f );
		D3DXVec3TransformCoord( &vDir, &vDir, &matRotAxis );
		m_vLookAt = vDir + m_vEyePos;
	}

	if( GetAsyncKeyState( VK_SPACE ) )
	{
		m_vEyePos += ( m_vUp * 10 * _fDT );
		m_vLookAt += ( m_vUp * 10 * _fDT );
	}

	if( GetAsyncKeyState( VK_SHIFT ) )
	{
		m_vEyePos -= ( m_vUp * 10 * _fDT );
		m_vLookAt -= ( m_vUp * 10 * _fDT );
	}

	D3DXMatrixLookAtLH(&m_matView, &m_vEyePos, &m_vLookAt, &m_vUp);
}