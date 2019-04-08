// Local Includes
#include "Vertex.h"

// This Include
#include "ndc_quad.h"

// Implementation
CNDCQuad::CNDCQuad() :
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr )
{}

CNDCQuad::~CNDCQuad()
{
	ReleaseCOM( m_pVertexBuffer );
}

void CNDCQuad::Initialise( ID3D10Device* _pDevice )
{
	m_pDevice = _pDevice;

	VQuad vertices[ 6 ];

	vertices[ 0 ] = VQuad( D3DXVECTOR3( 0.0f, -1.0f, 0.0f ), D3DXVECTOR2( 0.0f, 1.0f ) );
	vertices[ 1 ] = VQuad( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) );
	vertices[ 2 ] = VQuad( D3DXVECTOR3( 1.0f, 0.0f, 0.0f ), D3DXVECTOR2( 1.0f, 0.0f ) );
	vertices[ 3 ] = VQuad( D3DXVECTOR3( 0.0f, -1.0f, 0.0f ), D3DXVECTOR2( 0.0f, 1.0f ) );
	vertices[ 4 ] = VQuad( D3DXVECTOR3( 1.0f, 0.0f, 0.0f ), D3DXVECTOR2( 1.0f, 0.0f ) );
	vertices[ 5 ] = VQuad( D3DXVECTOR3( 1.0f, -1.0f, 0.0f ), D3DXVECTOR2( 1.0f, 1.0f ) );

	// Fill buffer desc
	D3D10_BUFFER_DESC vbDesc;
	vbDesc.Usage			= D3D10_USAGE_IMMUTABLE;
	vbDesc.ByteWidth		= sizeof( VQuad ) * 6;
	vbDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags	= 0;
	vbDesc.MiscFlags		= 0;

	// Set vertices to memory
	D3D10_SUBRESOURCE_DATA viData;
	viData.pSysMem = vertices;

	HR( m_pDevice->CreateBuffer( &vbDesc, &viData, &m_pVertexBuffer ) );
}

void CNDCQuad::Draw()
{
	UINT stride = sizeof( VQuad );
	UINT offset = 0;
	m_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pDevice->Draw( 6, 0 );
}