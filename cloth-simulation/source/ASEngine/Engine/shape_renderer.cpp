//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "engine.h"
#include "math_functions.h"
#include "safe_release.h"
#include "shader_default_2d.h"
#include "shader_id_list.h"
#include "vertex.h"

// This Include
#include "shape_renderer.h"

// Implementation
CShapeRenderer::CShapeRenderer() :
	m_MaxVertices( 100 )
{
	D3DXMatrixIdentity( &this->m_IdentityMatrix );
}

CShapeRenderer::~CShapeRenderer()
{
	ReleaseCOM( this->m_pVertexBuffer );
}

void CShapeRenderer::Initialise()
{
	m_pDevice = &CEngine::GetInstance().GetDevice();

	// Fill vertex buffer desctiption
	D3D10_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D10_USAGE_DYNAMIC;
	vbDesc.ByteWidth = sizeof( VDefault2D ) * m_MaxVertices;
	vbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;

	// Create vertex buffer
	m_pDevice->CreateBuffer( &vbDesc, 0, &m_pVertexBuffer );
}

void CShapeRenderer::DrawRectangle2D( CCamera* _pRenderCamera, const D3DXMATRIX& _kmatWorld,
									  const D3DXVECTOR2& _kvSize, const D3DXCOLOR _kColor )
{
	const unsigned kuNumVertices = 5;

	float fhX = _kvSize.x * 0.5f;
	float fhY = _kvSize.y * 0.5f;

	D3DXVECTOR2 vertices[ kuNumVertices ] =
	{
		D3DXVECTOR2( + fhX, + fhY ),
		D3DXVECTOR2( + fhX, - fhY ),
		D3DXVECTOR2( - fhX, - fhY ),
		D3DXVECTOR2( - fhX, + fhY ),
		D3DXVECTOR2( + fhX, + fhY ),
	};

	this->DrawLines2D( _pRenderCamera, _kmatWorld, vertices, kuNumVertices, _kColor );
}

void CShapeRenderer::DrawRectangle2D( CCamera* _pRenderCamera, const D3DXVECTOR2& _kvPosition, const float& _kfRotation,
									  const D3DXVECTOR2& _kvSize, const D3DXCOLOR _kColor )
{
	const unsigned kuNumVertices = 5;

	float fhX = _kvSize.x * 0.5f;
	float fhY = _kvSize.y * 0.5f;

	D3DXVECTOR2 vertices[ kuNumVertices ] =
	{
		D3DXVECTOR2( _kvPosition.x + fhX, _kvPosition.y + fhY ),
		D3DXVECTOR2( _kvPosition.x + fhX, _kvPosition.y - fhY ),
		D3DXVECTOR2( _kvPosition.x - fhX, _kvPosition.y - fhY ),
		D3DXVECTOR2( _kvPosition.x - fhX, _kvPosition.y + fhY ),
		D3DXVECTOR2( _kvPosition.x + fhX, _kvPosition.y + fhY ),
	};

	for( auto& aVertex : vertices )
	{
		aVertex = Math::RotatePointAroundPoint( _kvPosition, aVertex, _kfRotation );
	}

	this->DrawLines2D( _pRenderCamera, this->m_IdentityMatrix, vertices, kuNumVertices, _kColor );
}

void CShapeRenderer::DrawCircle2D( CCamera* _pRenderCamera, const D3DXMATRIX& _kmatWorld, const float& _kfRadius, const D3DXCOLOR _kColor )
{
	const unsigned kuNumVertices = 31;

	D3DXVECTOR2 vertices[ kuNumVertices ];

	float fAngle = ( 2.0f * Math::PI ) / ( kuNumVertices - 1 );

	for( unsigned i = 0; i < kuNumVertices; ++i )
	{
		vertices[ i ] = D3DXVECTOR2(  _kfRadius * cosf( fAngle * i ),  _kfRadius * sinf( fAngle * i ) );
	}

	this->DrawLines2D( _pRenderCamera, _kmatWorld, vertices, kuNumVertices, _kColor );
}

void CShapeRenderer::DrawCircle2D( CCamera* _pRenderCamera, const D3DXVECTOR2& _kvPosition, const float& _kfRadius, const D3DXCOLOR _kColor )
{
	const unsigned kuNumVertices = 31;

	D3DXVECTOR2 vertices[ kuNumVertices ];

	float fAngle = ( 2.0f * Math::PI ) / ( kuNumVertices - 1 );

	for( unsigned i = 0; i < kuNumVertices; ++i )
	{
		vertices[ i ] = D3DXVECTOR2( _kvPosition.x + _kfRadius * cosf( fAngle * i ), _kvPosition.y + _kfRadius * sinf( fAngle * i ) );
	}

	this->DrawLines2D( _pRenderCamera, this->m_IdentityMatrix, vertices, kuNumVertices, _kColor );
}

void CShapeRenderer::DrawLines2D( CCamera* _pRenderCamera, const D3DXMATRIX& _kmatWorld, const D3DXVECTOR2* _pVertices,
								  const unsigned& _kuNumVertices, const D3DXCOLOR _kColor )
{
	MapBuffer( _pVertices, _kuNumVertices, _kColor );

	CShaderDefault2D* pShader = reinterpret_cast< CShaderDefault2D* >( CEngine::GetInstance().GetShaderManager().GetShader( SHADER_DEFAULT2D ) );

	unsigned stride = sizeof( VDefault2D );
	unsigned offset = 0;

	m_pDevice->IASetInputLayout( pShader->GetInputLayout() );
	D3DXMATRIX matWVP = _kmatWorld * _pRenderCamera->GetViewProjMatrix();
	pShader->GetVarWVP()->SetMatrix( reinterpret_cast< float* >( &matWVP ) );
	pShader->GetTechnique()->GetPassByIndex( 0 )->Apply( 0 );

	m_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP );
	m_pDevice->IASetVertexBuffers( 0, 1, &this->m_pVertexBuffer, &stride, &offset );
	m_pDevice->Draw( _kuNumVertices, 0 );
}

void CShapeRenderer::MapBuffer( const D3DXVECTOR2* _pVertices, const unsigned& _kuNumVertices, const D3DXCOLOR _kColor )
{
	VDefault2D* pVertex = nullptr;
	this->m_pVertexBuffer->Map( D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast< void** >( &pVertex ) );

	for( unsigned i = 0; i < _kuNumVertices; ++i )
	{
		pVertex[ i ].pos = _pVertices[ i ];
		pVertex[ i ].color = _kColor;
	}

	this->m_pVertexBuffer->Unmap();
}