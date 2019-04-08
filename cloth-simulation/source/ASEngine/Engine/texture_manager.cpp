//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// This Include
#include "texture_manager.h"

// Implementation
CTextureManager::CTextureManager() :
	m_strTexturePath( L"" )
{}

CTextureManager::~CTextureManager()
{
	for( auto& aTexture : this->m_Textures )
	{
		delete aTexture.second;
	}
}

CTexture* CTextureManager::GetTexture( const std::wstring& _kstrTextureFile )
{
	CTexture* pReturn{ nullptr };

	pReturn = this->m_Textures[ _kstrTextureFile ];

	if( pReturn == nullptr )
	{
		pReturn = new CTexture();

		if( !pReturn->Initialise( this->m_strTexturePath + _kstrTextureFile ) )
		{
			delete pReturn;
			pReturn = nullptr;
		}
		else
		{
			try
			{
				this->m_Textures[ _kstrTextureFile ] = pReturn;
				pReturn->SetName( _kstrTextureFile );
			}
			catch( ... )
			{
				delete pReturn;
				pReturn = nullptr;
			}
		}
	}

	return ( pReturn );
}

std::wstring CTextureManager::GetTexturePath() const
{
	return ( this->m_strTexturePath );
}

void CTextureManager::SetTexturePath( const std::wstring _kstrPath )
{
	this->m_strTexturePath = _kstrPath;
}