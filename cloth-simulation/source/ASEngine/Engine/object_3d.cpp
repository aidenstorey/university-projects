//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// This Include
#include "object_3d.h"

// Implementation
CObject3D::CObject3D( const float& _kfPositionX, const float& _kfPositionY, const float& _kfPositionZ, 
	const float& _kfRotationX, const float& _kfRotationY, const float& _kfRotationZ, 
	const float _kfScaleX, const float& _kfScaleY, const float _kfScaleZ ) :
	m_vPosition( _kfPositionX, _kfPositionY, _kfPositionZ ),
	m_vRotation( _kfRotationX, _kfRotationY, _kfRotationZ ),
	m_vScale( _kfScaleX, _kfScaleY, _kfScaleZ )
{
	this->ResetMatrix();
}

CObject3D::~CObject3D()
{}

D3DXVECTOR3 CObject3D::GetPosition() const
{
	return ( this->m_vPosition );
}

float CObject3D::GetPositionX() const
{
	return ( this->m_vPosition.x );
}

float CObject3D::GetPositionY() const
{
	return ( this->m_vPosition.y );
}

float CObject3D::GetPositionZ() const
{
	return ( this->m_vPosition.z );
}

D3DXVECTOR3 CObject3D::GetScale() const
{
	return ( this->m_vScale );
}

float CObject3D::GetScaleX() const
{
	return ( this->m_vScale.x );
}

float CObject3D::GetScaleY() const
{
	return ( this->m_vScale.y );
}

float CObject3D::GetScaleZ() const
{
	return ( this->m_vScale.z );
}

D3DXVECTOR3 CObject3D::GetRotation() const
{
	return ( this->m_vRotation );
}

float CObject3D::GetRotationX() const
{
	return ( this->m_vRotation.x );
}
 
float CObject3D::GetRotationY() const
{
	return ( this->m_vRotation.y );
}

float CObject3D::GetRotationZ() const
{
	return ( this->m_vRotation.z );
}

void CObject3D::SetPosition( const D3DXVECTOR3& _kvPosition )
{
	this->m_vPosition = _kvPosition;
}

void CObject3D::SetPosition( const float& _kfX, const float& _kfY, const float& _kfZ )
{
	this->m_vPosition = D3DXVECTOR3( _kfX, _kfY, _kfZ );
}

void CObject3D::SetPositionX( const float& _kfX )
{
	this->m_vPosition.x = _kfX;
}

void CObject3D::SetPositionY( const float& _kfY )
{
	this->m_vPosition.y = _kfY;
}

void CObject3D::SetPositionZ( const float& _kfZ )
{
	this->m_vPosition.z = _kfZ;
}

void CObject3D::SetScale( const D3DXVECTOR3& _kvScale )
{
	this->m_vScale = _kvScale;
}

void CObject3D::SetScale( const float& _kfX, const float& _kfY, const float& _kfZ )
{
	this->m_vScale = D3DXVECTOR3( _kfX, _kfY, _kfZ );
}

void CObject3D::SetScaleX( const float& _kfX )
{
	this->m_vScale.x = _kfX;
}

void CObject3D::SetScaleY( const float& _kfY )
{
	this->m_vScale.y = _kfY;
}

void CObject3D::SetScaleZ( const float& _kfZ )
{
	this->m_vScale.z = _kfZ;
}

void CObject3D::SetRotation( const D3DXVECTOR3& _kvRotation )
{
	this->m_vRotation = _kvRotation;
}

void CObject3D::SetRotation( const float& _kfX, const float& _kfY, const float& _kfZ )
{
	this->m_vRotation = D3DXVECTOR3( _kfX, _kfY, _kfZ );
}

void CObject3D::SetRotationX( const float& _kfX )
{
	this->m_vRotation.x = _kfX;
}

void CObject3D::SetRotationY( const float& _kfY )
{
	this->m_vRotation.y = _kfY;
}

void CObject3D::SetRotationZ( const float& _kfZ )
{
	this->m_vRotation.z = _kfZ;
}

void CObject3D::ResetMatrix()
{
	D3DXMATRIX matScale;
	D3DXMATRIX matRotate;
	D3DXMATRIX matTranslate;

	D3DXMatrixScaling( &matScale, this->m_vScale.x, this->m_vScale.y, this->m_vScale.z );
	D3DXMatrixRotationYawPitchRoll( &matRotate, this->m_vRotation.y, this->m_vRotation.x, this->m_vRotation.z );
	D3DXMatrixTranslation( &matTranslate, this->m_vPosition.x, this->m_vPosition.y, this->m_vPosition.z );

	this->m_matWorld = matScale * matRotate * matTranslate;
}