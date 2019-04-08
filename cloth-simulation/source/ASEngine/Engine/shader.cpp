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
#include "shader.h"

// Implementation
CShader::CShader( const int _kiShaderID, const std::wstring _kstrPath ) :
	m_iShaderID( _kiShaderID ),
	m_bIsInitialised( false ),
	m_pEffect( nullptr )
{}

CShader::~CShader()
{
	if ( m_bIsInitialised )
	{
		ReleaseCOM( m_pEffect );
		ReleaseCOM( m_pInputLayout );
	}
}

int CShader::GetShaderID() const
{
	return ( m_iShaderID );
}

bool CShader::IsInitialised() const
{
	return ( m_bIsInitialised );
}

ID3D10Effect* CShader::GetEffect()
{
	return ( m_pEffect );
}

ID3D10EffectTechnique* CShader::GetTechnique()
{
	return ( m_pTechnique );
}

ID3D10InputLayout* CShader::GetInputLayout()
{
	return ( m_pInputLayout );
}

ID3D10Effect* CShader::CreateEffect( const std::wstring _kstrShader )
{
	ID3D10Effect* pReturn{ nullptr };

	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob*		pCompileErrors = 0;
	HRESULT			hr = 0;

	hr = D3DX10CreateEffectFromFile( _kstrShader.c_str(), 0, 0, "fx_4_0", shaderFlags, 0, &CEngine::GetInstance().GetDevice(), 0, 0, &pReturn, &pCompileErrors, 0 );
	
	if( !SUCCEEDED( hr ) )
	{
		pReturn = nullptr;
	}

	return ( pReturn );
}