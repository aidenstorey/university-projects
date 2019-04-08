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
#include "shader_default_3d.h"

// Implementation
CShaderDefault3D::CShaderDefault3D( const int _kiShaderID, const std::wstring _kstrPath ) :
	CShader( _kiShaderID, _kstrPath )
{
	this->m_bIsInitialised = true;

	this->m_pEffect = CreateEffect( _kstrPath + L"default_shader_3d.fx" );

	if ( m_pEffect != nullptr )
	{
		m_pTechnique = m_pEffect->GetTechniqueByName( "DefaultTech" );
		m_pVarWVP = m_pEffect->GetVariableByName( "gWVP" )->AsMatrix();

		D3D10_PASS_DESC passDesc;

		D3D10_INPUT_ELEMENT_DESC ilDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );
		CEngine::GetInstance().GetDevice().CreateInputLayout( ilDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout );
	}
	else
	{
		m_bIsInitialised = false;
	}
}


ID3D10EffectMatrixVariable* CShaderDefault3D::GetVarWVP()
{
	return ( m_pVarWVP );
}