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
#include "shader_id_list.h"

// This Include
#include "textured_2d.h"

// Implementation
CTextured2D::CTextured2D( const std::wstring& _kstrName, std::wstring _krTextureName, const float _kfPositionX, const float _kfPositionY, const float _kfRotation, const float _kfScaleX, const float _kfScaleY ) :
	CObject2D( _kstrName, _kfPositionX, _kfPositionY, _kfRotation, _kfScaleX, _kfScaleY )
{
	this->m_pTexture = CEngine::GetInstance().GetTextureManager().GetTexture( _krTextureName );
	this->m_pDevice = &CEngine::GetInstance().GetDevice();
	this->m_pShader = reinterpret_cast< CShaderTextured2D* >( CEngine::GetInstance().GetShaderManager().GetShader( SHADER_TEXTURED2D ) );
}

CTextured2D::~CTextured2D()
{
	ReleaseCOM( m_pVertexBuffer );
}

CTexture* CTextured2D::GetTexture() const
{
	return ( this->m_pTexture );
}

void CTextured2D::SetTexture( CTexture* _pTexture )
{
	this->m_pTexture = _pTexture;
}

const std::vector< VTextured2D >* CTextured2D::GetVertices() const
{
	return ( &this->m_VertexList );
}

void CTextured2D::OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{
	this->m_pDevice->IASetInputLayout( this->m_pShader->GetInputLayout() );
	this->m_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	D3DXMATRIX matWVP = ( this->m_matWorld * _krOffset ) * _pRenderCamera->GetViewProjMatrix();
	this->m_pShader->GetVarWVP()->SetMatrix( reinterpret_cast< float* >( &matWVP ) );
	this->m_pShader->GetVarTexture()->SetResource( this->m_pTexture->GetTexture() );
	this->m_pShader->GetTechnique()->GetPassByIndex( 0 )->Apply( 0 );

	UINT stride = sizeof( VTextured2D );
	UINT offset = 0;
	this->m_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	this->m_pDevice->Draw( this->m_VertexList.size(), 0 );
}

void CTextured2D::InitialiseObject()
{
	D3D10_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D10_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof( VTextured2D ) * m_VertexList.size();
	vbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA viData;
	viData.pSysMem = this->m_VertexList.data();

	this->m_pDevice->CreateBuffer( &vbDesc, &viData, &this->m_pVertexBuffer );
}