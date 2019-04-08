//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local includes 
#include "engine.h"

// This Include
#include "shader_textured_2d.h"

// Implementation
CShaderTextured2D::CShaderTextured2D( const int _kiShaderID, const std::wstring _kstrPath ) :
	CShader( _kiShaderID, _kstrPath )
{
	this->m_bIsInitialised = true;

	this->m_pEffect = CreateEffect( _kstrPath + L"textured_shader_2d.fx" );

	if( m_pEffect != nullptr )
	{
		m_pTechnique = m_pEffect->GetTechniqueByName( "TexturedTech" );
		m_pVarWVP = m_pEffect->GetVariableByName( "gWVP" )->AsMatrix();
		m_pVarTexture = m_pEffect->GetVariableByName( "gTexture" )->AsShaderResource();

		D3D10_PASS_DESC passDesc;

		D3D10_INPUT_ELEMENT_DESC ilDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );
		CEngine::GetInstance().GetDevice().CreateInputLayout( ilDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout );
	}
	else
	{
		m_bIsInitialised = false;
	}
}

ID3D10EffectMatrixVariable* CShaderTextured2D::GetVarWVP()
{
	return ( m_pVarWVP );
}

ID3D10EffectShaderResourceVariable* CShaderTextured2D::GetVarTexture()
{
	return ( m_pVarTexture );
}