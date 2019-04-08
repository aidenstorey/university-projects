//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "engine.h"

// This Include
#include "button.h"

// Implementation
CButton::CButton( const std::wstring& _kstrName, std::wstring _krTextureName, const float _kfPositionX, const float _kfPositionY, const float _kfRotation, const float _kfScaleX, const float _kfScaleY ) :
	CTextured2D( _kstrName, _krTextureName, _kfPositionX, _kfPositionY, _kfRotation, _kfScaleX, _kfScaleY )
{}

CButton::~CButton()
{}

void CButton::SetClickFunction( void( *_pFunc )( ) )
{
	this->m_ClickFunction = _pFunc;
}

void CButton::OnClick( const D3DXVECTOR2 _kvMousePos )
{
	if( this->IsOver( _kvMousePos ) )
	{
		m_ClickFunction();
	}
}

bool CButton::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	if( CEngine::GetInstance().GetInputManager().IsReleased( VK_LBUTTON ) )
	{
		this->OnClick( CEngine::GetInstance().GetInputManager().MouseUIPosition2D() );
	}

	return( bReturn );
}