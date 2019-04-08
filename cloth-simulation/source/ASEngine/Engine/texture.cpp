//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "engine.h"
#include "safe_release.h"

// This Include
#include "texture.h"

// Implementation
CTexture::CTexture()
{}

CTexture::~CTexture()
{
	ReleaseCOM( m_pTexture );
}

std::wstring CTexture::GetName() const
{
	return ( this->m_strName );
}

void CTexture::SetName( const std::wstring _kwstrName )
{
	this->m_strName = _kwstrName;
}

ID3D10ShaderResourceView* CTexture::GetTexture()
{
	return ( m_pTexture );
}

bool CTexture::Initialise( const std::wstring& _kstrTextureFile )
{
	this->m_pTexture = nullptr;

	HRESULT HResult;

	D3DX10CreateShaderResourceViewFromFile( &CEngine::GetInstance().GetDevice(), _kstrTextureFile.c_str(), nullptr, nullptr, &this->m_pTexture, &HResult );

	return( this->m_pTexture != nullptr );
}