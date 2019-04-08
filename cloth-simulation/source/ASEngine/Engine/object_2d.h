//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_OBJECT_2D_H_201409231644
#define GUARD_AIDEN_STOREY_OBJECT_2D_H_201409231644

// Library Includes
#include <D3DX10.h>
#include <string>

// Local Includes
#include "drawable.h"

// Prototypes
class CObject2D : public CDrawable
{
	friend class CObject2DManager;

	// Member Functions
public:
	// Accessors
	std::wstring GetName() const;
	void SetName( const std::wstring& _kstrName );

	D3DXVECTOR2 GetPosition() const;
	float GetPositionX() const;
	float GetPositionY() const;

	D3DXVECTOR2 GetScale() const;
	float GetScaleX() const;
	float GetScaleY() const;

	D3DXVECTOR2 GetSize() const;
	float GetSizeX() const;
	float GetSizeY() const;

	float GetRotation() const;

	void SetPosition( const D3DXVECTOR2 _vPosition );
	void SetPosition( const float _kfX, const float _kfY );
	void SetPositionX( const float _kfX );
	void SetPositionY( const float _kfY );

	void SetScale( const D3DXVECTOR2 _vScale );
	void SetScale( const float _kfX, const float _kfY );
	void SetScaleX( const float _kfX );
	void SetScaleY( const float _kfY );

	void SetSize( const D3DXVECTOR2 _vSize );
	void SetSize( const float _kfX, const float _kfY );
	void SetSizeX( const float _kfX );
	void SetSizeY( const float _kfY );

	void SetRotation( const float _kfRotation );

protected:
	// Constructors
	CObject2D( const std::wstring& _kstrName = L"", const float& _kfPositionX = 0.0f, const float& _kfPositionY = 0.0f, const float& _kfRotation = 0.0f, const float& _kfScaleX = 1.0f, const float& _kfScaleY = 1.0f );
	virtual ~CObject2D() = 0;

	// Other
	void ResetMatrix();

private:

	// Member Variables
public:
protected:
	D3DXVECTOR2					m_vPosition;
	D3DXVECTOR2					m_vScale;
	D3DXVECTOR2					m_vSize;
	float						m_fRotation;
	std::wstring				m_strName;

private:
};

#endif // GUARD_AIDEN_STOREY_OBJECT_2D_H_201409231644