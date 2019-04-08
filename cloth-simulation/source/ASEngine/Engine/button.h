//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_BUTTON_H_201409251152
#define GUARD_AIDEN_STOREY_BUTTON_H_201409251152

// Library Includes
#include <string>

// Local Includes
#include "text.h"
#include "textured_2d.h"

// Prototypes
class CButton : public CTextured2D
{
	friend class CObject2DManager;

public:
	// Accessors
	void SetClickFunction( void( *_pFunc )( ) );

	virtual bool IsOver( const D3DXVECTOR2 _kvMousePos ) = 0;
	
	// Other
	void OnClick( const D3DXVECTOR2 _kvMousePos );

protected:
	// Constructors
	CButton( const std::wstring& _kstrName = L"", std::wstring _krTextureName = L"DefaultTexture.png", const float _kfPositionX = 0.0f, const float _kfPositionY = 0.0f,
			 const float _kfRotation = 0.0f, const float _kfScaleX = 1.0f, const float _kfScaleY = 1.0f );
	~CButton();

	// Other 
	bool OnUpdate( const float& _kfTimeDelta ) override;
	
private:

public:
protected:
private:
	void( *m_ClickFunction )( );
	CText*		m_pHoverText;
};

#endif // GUARD_AIDEN_STOREY_BUTTON_H_201409251152