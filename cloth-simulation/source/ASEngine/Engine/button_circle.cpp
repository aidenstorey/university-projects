//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "math_functions.h"

// This Include
#include "button_circle.h"

// Implementation
CButtonCircle::CButtonCircle( const std::wstring& _kstrName, std::wstring _krTextureName, const float& _kfRadius, const float _kfPositionX,
							  const float _kfPositionY, const float _kfRotation, const float _kfScaleX, const float _kfScaleY ) :
	CButton( _kstrName, _krTextureName, _kfPositionX, _kfPositionY, _kfRotation, _kfScaleX, _kfScaleY ),
	m_fRadius( _kfRadius )
{}

CButtonCircle::~CButtonCircle()
{}

bool CButtonCircle::IsOver( const D3DXVECTOR2 _kvMousePos )
{
	return ( Math::DistanceBetween( this->m_vPosition, _kvMousePos ) <= this->m_fRadius );
}