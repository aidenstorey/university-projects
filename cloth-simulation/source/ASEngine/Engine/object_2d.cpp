//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// This Include 
#include "object_2d.h"

// Implementation
CObject2D::CObject2D( const std::wstring& _kstrName, const float& _kfPositionX, const float& _kfPositionY, const float& _kfRotation, const float& _kfScaleX, const float& _kfScaleY ) :
	m_strName( _kstrName )
{
	this->m_vPosition = D3DXVECTOR2( _kfPositionX, _kfPositionY );
	this->m_vScale = D3DXVECTOR2( _kfScaleX, _kfScaleY );
	this->m_vSize = D3DXVECTOR2( 0.0f, 0.0f );
	this->m_fRotation = _kfRotation;

	this->ResetMatrix();
}

CObject2D::~CObject2D()
{}

std::wstring CObject2D::GetName() const
{
	return( this->m_strName );
}

void CObject2D::SetName( const std::wstring& _kstrName )
{
	this->m_strName = _kstrName;
}

D3DXVECTOR2 CObject2D::GetPosition() const
{
	return ( this->m_vPosition );
}

float CObject2D::GetPositionX() const
{
	return ( this->m_vPosition.x );
}

float CObject2D::GetPositionY() const
{
	return ( this->m_vPosition.y );
}

D3DXVECTOR2 CObject2D::GetScale() const
{
	return ( this->m_vScale );
}

float CObject2D::GetScaleX() const
{
	return ( this->m_vScale.x );
}

float CObject2D::GetScaleY() const
{
	return ( this->m_vScale.y );
}

D3DXVECTOR2 CObject2D::GetSize() const
{
	return ( this->m_vSize );
}

float CObject2D::GetSizeX() const
{
	return ( this->m_vSize.x );
}

float CObject2D::GetSizeY() const
{
	return ( this->m_vSize.y );
}

float CObject2D::GetRotation() const
{
	return ( this->m_fRotation );
}

void CObject2D::SetPosition( const D3DXVECTOR2 _vPosition )
{
	this->m_vPosition = _vPosition;
	this->ResetMatrix();
}

void CObject2D::SetPosition( const float _kfX, const float _kfY )
{
	this->m_vPosition = D3DXVECTOR2( _kfX, _kfY );
	this->ResetMatrix();
}

void CObject2D::SetPositionX( const float _kfX )
{
	this->m_vPosition.x = _kfX;
	this->ResetMatrix();
}

void CObject2D::SetPositionY( const float _kfY )
{
	this->m_vPosition.y = _kfY;
	this->ResetMatrix();
}

void CObject2D::SetScale( const D3DXVECTOR2 _vScale )
{
	this->m_vScale = _vScale;
	this->ResetMatrix();
}

void CObject2D::SetScale( const float _kfX, const float _kfY )
{
	this->m_vScale = D3DXVECTOR2( _kfX, _kfY );
	this->ResetMatrix();
}

void CObject2D::SetScaleX( const float _kfX )
{
	this->m_vScale.x = _kfX;
	this->ResetMatrix();
}

void CObject2D::SetScaleY( const float _kfY )
{
	this->m_vScale.y = _kfY;
	this->ResetMatrix();
}

void CObject2D::SetSize( const D3DXVECTOR2 _vSize )
{
	this->m_vSize = _vSize;
}

void CObject2D::SetSize( const float _kfX, const float _kfY )
{
	this->m_vSize = D3DXVECTOR2( _kfX, _kfY );
}

void CObject2D::SetSizeX( const float _kfX )
{
	this->m_vSize.x = _kfX;
}

void CObject2D::SetSizeY( const float _kfY )
{
	this->m_vSize.y = _kfY;
}

void CObject2D::SetRotation( const float _kfRotation )
{
	this->m_fRotation = _kfRotation;
	this->ResetMatrix();
}

void CObject2D::ResetMatrix()
{
	D3DXMATRIX matScale;
	D3DXMATRIX matRotate;
	D3DXMATRIX matTranslate;

	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, 0.0f );
	D3DXMatrixRotationYawPitchRoll( &matRotate, 0.0f, 0.0f, m_fRotation );
	D3DXMatrixTranslation( &matTranslate, m_vPosition.x, m_vPosition.y, 0.0f );

	m_matWorld = matScale * matRotate * matTranslate;
}