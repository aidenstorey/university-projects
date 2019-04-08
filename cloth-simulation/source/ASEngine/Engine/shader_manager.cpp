//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "engine.h"

// This Include
#include "shader_manager.h"

// Implementation
CShaderManager::CShaderManager()
{}

CShaderManager::~CShaderManager()
{
	for( auto& aShader : m_ShaderList )
	{
		delete aShader;
	}
}

CShader* CShaderManager::GetShader( const int _kiShaderID )
{
	CShader* pReturn{ nullptr };

	for( auto& aShader : m_ShaderList )
	{
		if( aShader->GetShaderID() == _kiShaderID )
		{
			pReturn = aShader;
			break;
		}
	}

	return ( pReturn );
}

std::wstring CShaderManager::GetShaderPath() const
{
	return ( this->m_strShaderPath );
}

void CShaderManager::SetShaderPath( const std::wstring _kstrPath )
{
	this->m_strShaderPath = _kstrPath;
}