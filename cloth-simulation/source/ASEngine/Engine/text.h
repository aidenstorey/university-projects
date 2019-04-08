//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_TEXT_H_201409202214
#define GUARD_AIDEN_STOREY_TEXT_H_201409202214

// Library Includes
#include <D3DX10.h>
#include <string>

// Local Includes
#include "drawable.h"

// Prototypes
class CText : public CDrawable
{
	friend class CTextManager;
	// Member Functions
public:
	// Accessors
	void SetText( const std::wstring& _kstrText );

	void SetTextColour( const D3DXCOLOR& _krColor );

	void SetPosition( const float& _kfX, const float& _kfY );
	void SetPosition( const D3DXVECTOR2& _krPosition );

	// Other
	void Draw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset ) override;
protected:
private:
	// Constructors
	CText( const std::wstring& _kstrText, const float _kfX, const float _kfY );
	virtual ~CText();

	// Member Variables
public:
protected:
private:
	std::wstring	m_strText;
	D3DXCOLOR		m_TextColor{ 0.0f, 0.0f, 0.0f, 1.0f };

	ID3DX10Font*	m_pFont{ nullptr };
	RECT			m_DrawRect;
};

#endif // GUARD_AIDEN_STOREY_TEXT_H_201409202214