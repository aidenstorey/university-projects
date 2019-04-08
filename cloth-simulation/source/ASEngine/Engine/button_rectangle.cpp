//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#define NOMINMAX

// Library Includes
#include <algorithm>

// Local Includes
#include "math_functions.h"

// This Include
#include "button_rectangle.h"

// Implementation
CButtonRectangle::CButtonRectangle( const std::wstring& _kstrName, std::wstring _krTextureName, const float _kfWidth, const float _kfHeight, const float _kfPositionX,
									const float _kfPositionY, const float _kfRotation, const float _kfScaleX, const float _kfScaleY ) :
	CButton( _kstrName, _krTextureName, _kfPositionX, _kfPositionY, _kfRotation, _kfScaleX, _kfScaleY )
{
	this->m_vSize = D3DXVECTOR2( _kfWidth, _kfHeight );
}

CButtonRectangle::~CButtonRectangle()
{}

bool CButtonRectangle::IsOver( const D3DXVECTOR2 _kvMousePos )
{
	bool bReturn{ false };

	D3DXVECTOR2 vhSize = this->m_vSize * 0.5f;

	D3DXVECTOR2 vTL = this->m_vPosition - vhSize;
	D3DXVECTOR2 vBR = this->m_vPosition + vhSize;

	D3DXVECTOR2 vRotatedPos = Math::RotatePointAroundPoint( this->m_vPosition, _kvMousePos, -( this->m_fRotation ) );

	if( vRotatedPos.x >= std::min( vTL.x, vBR.x ) &&
		vRotatedPos.x <= std::max( vTL.x, vBR.x ) &&
		vRotatedPos.y >= std::min( vTL.y, vBR.y ) &&
		vRotatedPos.y <= std::max( vTL.y, vBR.y ) )
	{
		bReturn = true;
	}

	return ( bReturn );
}