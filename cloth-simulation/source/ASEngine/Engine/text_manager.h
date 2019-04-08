//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_TEXT_MANAGER_H_201409202235
#define GUARD_AIDEN_STOREY_TEXT_MANAGER_H_201409202235

// Library Includes
#include <unordered_map>

// Local Includes
#include "text.h"

class CTextManager
{
	// Member Functions
public:
	// Constructors
	CTextManager();
	~CTextManager();

	// Other
	CText* Create( const float _kfX = 0, const float _kfY = 0, const bool _kbShow = true );
	CText* Create( const std::wstring& _kstrText, const float _kfX = 0, const float _kfY = 0, const bool _kbShow = true );

	bool CreateD3DFont( const std::wstring& _krFontLabel, const int _iHeight = 16, const int _iWidth = 0, const int _iWeight = 0,
						std::wstring _strFaceName = L"Courier New", int _iMipLevels = 1, bool _bItalic = false );
	bool SetFont( CText* _pText, const std::wstring& _krFontLabel );

protected:
private:

	// Member Variables
public:
protected:
private:
	std::vector< CText* >								m_TextList;
	std::unordered_map< std::wstring, ID3DX10Font* >	m_FontList;
};

#endif // GUARD_AIDEN_STOREY_TEXT_MANAGER_H_201409202235