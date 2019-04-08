//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "engine.h"
#include "viewport.h"

// This Include 
#include "input_manager.h"

// Implementation
CInputManager::CInputManager()
{}

CInputManager::~CInputManager()
{}

void CInputManager::SetKeyState( const unsigned _kuKey, const bool _bPressed )
{
	this->m_bPrevKeyState[ _kuKey ] = this->m_bCurrKeyState[ _kuKey ];
	this->m_bCurrKeyState[ _kuKey ] = _bPressed;
}

bool CInputManager::IsDown( const char _cKey )
{
	return ( m_bCurrKeyState[ _cKey ] );
}

bool CInputManager::IsPressed( const char _cKey )
{
	return ( m_bCurrKeyState[ _cKey ] && !m_bPrevKeyState[ _cKey ] );
}

bool CInputManager::IsReleased( const char _cKey )
{
	return ( !m_bCurrKeyState[ _cKey ] && m_bPrevKeyState[ _cKey ] );
}

D3DXVECTOR2 CInputManager::MouseScreenPosition2D()
{
	POINT position;
	GetCursorPos( &position );

	return ( D3DXVECTOR2( static_cast< float >( position.x ), static_cast< float >( position.y ) ) );
}

D3DXVECTOR3 CInputManager::MouseScreenPosition3D()
{
	D3DXVECTOR2 position = this->MouseScreenPosition2D();
	return ( D3DXVECTOR3( position.x, position.y, 0.0f ) );
}

D3DXVECTOR2 CInputManager::MouseClientPosition2D()
{
	POINT position;
	GetCursorPos( &position );
	ScreenToClient( m_hWnd, &position );

	return ( D3DXVECTOR2( static_cast< float >( position.x ), static_cast< float >( position.y ) ) );
}

D3DXVECTOR3 CInputManager::MouseClientPosition3D()
{
	D3DXVECTOR2 position = this->MouseScreenPosition2D();
	return ( D3DXVECTOR3( position.x, position.y, 0.0f ) );
}

D3DXVECTOR2 CInputManager::MouseWorldPosition2D()
{
	CCamera* pCamera = &CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetCamera();
	CViewport* pViewport = pCamera->GetViewport();
	D3DXVECTOR2 vOffset( static_cast< float >( pViewport->GetWidth() ) * 0.5f, static_cast< float >( pViewport->GetHeight() ) * 0.5f );	

	float fScale{ 1.0f };	
	if( pCamera->GetCameraMode() == CAMERAMODE_ORTHOGONAL )
	{
		fScale = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetCamera().GetScale();
	}

	return ( D3DXVECTOR2( this->MouseClientPosition2D().x - vOffset.x, ( this->MouseClientPosition2D().y - vOffset.y ) * -1.0f ) * fScale );
}

D3DXVECTOR3 CInputManager::MouseWorldPosition3D()
{
	D3DXVECTOR2 position = this->MouseWorldPosition2D();
	return ( D3DXVECTOR3( position.x, position.y, 0.0f ) );
}

D3DXVECTOR2 CInputManager::MouseClampedWorldPosition2D()
{
	D3DXVECTOR2 vMousePos = this->MouseWorldPosition2D();

	float fWScalar = 2.0f / CEngine::GetInstance().GetClientWidth();
	float fHScalar = 2.0f / CEngine::GetInstance().GetClientHeight();

	return( D3DXVECTOR2( vMousePos.x * fWScalar, vMousePos.y * fHScalar ) );
}

D3DXVECTOR3 CInputManager::MouseClampedWorldPosition3D()
{
	D3DXVECTOR2 vMousePos = this->MouseWorldPosition2D();
	return( D3DXVECTOR3( vMousePos.x, vMousePos.y, 0.0f ) );
}

D3DXVECTOR2 CInputManager::MouseUIPosition2D()
{
	CCamera* pCamera = &CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetUICamera();
	CViewport* pViewport = pCamera->GetViewport();
	D3DXVECTOR2 vOffset( static_cast< float >( pViewport->GetWidth() ) * 0.5f, static_cast< float >( pViewport->GetHeight() ) * 0.5f );	

	float fScale{ 1.0f };	
	if( pCamera->GetCameraMode() == CAMERAMODE_ORTHOGONAL )
	{
		fScale = CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetUICamera().GetScale();
	}

	return ( D3DXVECTOR2( this->MouseClientPosition2D().x - vOffset.x, ( this->MouseClientPosition2D().y - vOffset.y ) * -1.0f ) * fScale );
}

D3DXVECTOR3 CInputManager::MouseUIPosition3D()
{
	D3DXVECTOR2 position = this->MouseUIPosition2D();
	return ( D3DXVECTOR3( position.x, position.y, 0.0f ) );
}

bool CInputManager::Initialise( HWND _hWnd )
{
	bool bReturn{ true };

	m_hWnd = _hWnd;

	return ( bReturn );
}

void CInputManager::Update()
{
	for( int key = 0; key < g_NumKeys; ++key )
	{
		m_bPrevKeyState[ key ] = m_bCurrKeyState[ key ];
	}
}