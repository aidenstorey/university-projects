//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_OBJECT_3D_H_201410240950
#define GUARD_AIDEN_STOREY_OBJECT_3D_H_201410240950

// Library Includes
#include <D3DX10.h>
#include <string>

// Local Includes
#include "drawable.h"

// Prototypes
class CObject3D : public CDrawable
{
	friend class CObject3DManager;

	// Member Functions
public:
	// Accessors
	D3DXVECTOR3 GetPosition() const;
	float GetPositionX() const;
	float GetPositionY() const;
	float GetPositionZ() const;

	D3DXVECTOR3 GetScale() const;
	float GetScaleX() const;
	float GetScaleY() const;
	float GetScaleZ() const;

	D3DXVECTOR3 GetRotation() const;
	float GetRotationX() const; 
	float GetRotationY() const;
	float GetRotationZ() const;

	void SetPosition( const D3DXVECTOR3& _kvPosition );
	void SetPosition( const float& _kfX, const float& _kfY, const float& _kfZ );
	void SetPositionX( const float& _kfX );
	void SetPositionY( const float& _kfY );
	void SetPositionZ( const float& _kfZ );

	void SetScale( const D3DXVECTOR3& _kvScale );
	void SetScale( const float& _kfX, const float& _kfY, const float& _kfZ );
	void SetScaleX( const float& _kfX );
	void SetScaleY( const float& _kfY );
	void SetScaleZ( const float& _kfZ );

	void SetRotation( const D3DXVECTOR3& _kvRotation );
	void SetRotation( const float& _kfX, const float& _kfY, const float& _kfZ );
	void SetRotationX( const float& _kfX );
	void SetRotationY( const float& _kfY );
	void SetRotationZ( const float& _kfZ );

protected:
	// Constructors
	CObject3D( const float& _kfPositionX = 0.0f, const float& _kfPositionY = 0.0f, const float& _kfPositionZ = 0.0f, 
		const float& _kfRotationX = 0.0f, const float& _kfRotationY = 0.0f, const float& _kfRotationZ = 0.0f, 
		const float _kfScaleX = 1.0f, const float& _kfScaleY = 1.0f, const float _kfScaleZ = 1.0f );
	virtual ~CObject3D() = 0;

	// Other
	void ResetMatrix();
private:

	// Member Variables
public:
protected:
	D3DXVECTOR3	m_vPosition;
	D3DXVECTOR3	m_vScale;
	D3DXVECTOR3	m_vRotation;

private:
};

#endif // GUARD_AIDEN_STOREY_OBJECT_3D_H_201410240950