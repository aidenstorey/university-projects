//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// This Include
#include "text.h"

// Implementation
CText::CText( const std::wstring& _kstrText, const float _kfX, const float _kfY ) :
	m_strText( _kstrText )
{
	this->SetPosition( _kfX, _kfY );
}

CText::~CText()
{}

void CText::SetText( const std::wstring& _kstrText )
{
	this->m_strText = _kstrText;
}

void CText::SetTextColour( const D3DXCOLOR& _krColor )
{
	this->m_TextColor = _krColor;
}

void CText::SetPosition( const float& _kfX, const float& _kfY )
{
	this->m_DrawRect.left = static_cast< LONG >( _kfX );
	this->m_DrawRect.top = static_cast< LONG >( _kfY );
}

void CText::SetPosition( const D3DXVECTOR2& _krPosition )
{
	this->SetPosition( _krPosition.x, _krPosition.y );
}

void CText::Draw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{
	if( this->m_pFont != nullptr )
	{
		this->m_pFont->DrawText( 0, this->m_strText.c_str(), -1, &this->m_DrawRect, DT_NOCLIP, this->m_TextColor );
	}
}