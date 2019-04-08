//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local includes
#include "engine.h"
#include "safe_release.h"

// This Include
#include "text_manager.h"

// Implementation
CTextManager::CTextManager()
{}

CTextManager::~CTextManager()
{
	for( auto& aText : this->m_TextList )
	{
		delete aText;
	}

	for( auto& aFont : this->m_FontList )
	{
		ReleaseCOM( aFont.second );
	}
}

CText* CTextManager::Create( const float _kfX, const float _kfY, const bool _kbShow )
{
	return ( this->Create( L"", _kfX, _kfY, _kbShow ) );
}

CText* CTextManager::Create( const std::wstring& _kstrText, const float _kfX, const float _kfY, const bool _kbShow )
{
	CText* pText = new CText( _kstrText, _kfX, _kfY );
	pText->SetVisible( _kbShow );

	try
	{
		this->m_TextList.push_back( pText );
		this->SetFont( pText, L"DefaultFont" );
	}
	catch( ... )
	{
		delete pText;
		pText = nullptr;
	}

	return ( pText );
}

bool CTextManager::CreateD3DFont( const std::wstring& _krFontLabel, const int _iHeight, const int _iWidth, const int _iWeight,
							   std::wstring _strFaceName, int _iMipLevels, bool _bItalic )
{
	bool bReturn{ false };

	if( this->m_FontList.find( _krFontLabel ) == this->m_FontList.end() )
	{
		D3DX10_FONT_DESC fontDesc;
		fontDesc.Width = _iWidth;
		fontDesc.Height = _iHeight;
		fontDesc.Weight = _iWeight;
		fontDesc.MipLevels = _iMipLevels;
		fontDesc.Italic = _bItalic;
		fontDesc.CharSet = DEFAULT_CHARSET;
		fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDesc.Quality = DEFAULT_QUALITY;
		fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		wcscpy_s( fontDesc.FaceName, _strFaceName.c_str() );

		ID3DX10Font* pFont{ nullptr };
		D3DX10CreateFontIndirect( &CEngine::GetInstance().GetDevice(), &fontDesc, &pFont );

		this->m_FontList.insert( std::make_pair( _krFontLabel, pFont ) );

		bReturn = true;
	}

	return ( bReturn );
}

bool CTextManager::SetFont( CText* _pText, const std::wstring& _krFontLabel )
{
	bool bReturn{ true };
	
	auto aFont = this->m_FontList.find( _krFontLabel );
	if( aFont != this->m_FontList.end() )
	{
		_pText->m_pFont = aFont->second;
		bReturn = true;
	}
	else
	{
		_pText->m_pFont = nullptr;
		bReturn = false;
	}
	
	return ( bReturn );
}