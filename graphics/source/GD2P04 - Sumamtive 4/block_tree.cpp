// Local Includes
#include "services.h"

// This Include
#include "block_tree.h"

// Implementation
CBlockTree::CBlockTree() :
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ),
	m_pInstanceBuffer( nullptr ), m_vPosition( 0.0f, 0.0f, 0.0f )
{}

CBlockTree::~CBlockTree()
{
	ReleaseCOM( m_pInstanceBuffer );
}

void CBlockTree::SetPosition( D3DXVECTOR3 _vPosition )
{
	m_vPosition = _vPosition;

	MapBuffer();
}

void CBlockTree::Initialise( CInstanceCube* _pInstance )
{
	// Local copy
	m_pVertexBuffer = _pInstance->GetVertexBuffer();

	// Get device from services
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	m_pDevice = static_cast< ID3D10Device* >( vpDevice );

	// TODO:	If enough time read this in from a file to a "instance model" class

	// Trunk
	m_Instances[ 0 ]  = VInstance( D3DXVECTOR3(  0.0f,  0.0f,  0.0f ), 1 );
	m_Instances[ 1 ]  = VInstance( D3DXVECTOR3(  0.0f,  1.0f,  0.0f ), 1 );

	// First leaves layer
	m_Instances[ 2 ]  = VInstance( D3DXVECTOR3(  1.0f,  2.0f,  1.0f ), 2 );
	m_Instances[ 3 ]  = VInstance( D3DXVECTOR3(  0.0f,  2.0f,  1.0f ), 2 );
	m_Instances[ 4 ]  = VInstance( D3DXVECTOR3( -1.0f,  2.0f,  1.0f ), 2 );
	m_Instances[ 5 ]  = VInstance( D3DXVECTOR3( -1.0f,  2.0f,  0.0f ), 2 );
	m_Instances[ 6 ]  = VInstance( D3DXVECTOR3( -1.0f,  2.0f, -1.0f ), 2 );
	m_Instances[ 7 ]  = VInstance( D3DXVECTOR3(  0.0f,  2.0f, -1.0f ), 2 );
	m_Instances[ 8 ]  = VInstance( D3DXVECTOR3(  1.0f,  2.0f, -1.0f ), 2 );
	m_Instances[ 9 ]  = VInstance( D3DXVECTOR3(  1.0f,  2.0f,  0.0f ), 2 );
	m_Instances[ 10 ] = VInstance( D3DXVECTOR3(  2.0f,  2.0f,  2.0f ), 2 );
	m_Instances[ 11 ] = VInstance( D3DXVECTOR3(  1.0f,  2.0f,  2.0f ), 2 );
	m_Instances[ 12 ] = VInstance( D3DXVECTOR3(  0.0f,  2.0f,  2.0f ), 2 );
	m_Instances[ 13 ] = VInstance( D3DXVECTOR3( -1.0f,  2.0f,  2.0f ), 2 );
	m_Instances[ 14 ] = VInstance( D3DXVECTOR3( -2.0f,  2.0f,  2.0f ), 2 );
	m_Instances[ 15 ] = VInstance( D3DXVECTOR3( -2.0f,  2.0f,  1.0f ), 2 );
	m_Instances[ 16 ] = VInstance( D3DXVECTOR3( -2.0f,  2.0f,  0.0f ), 2 );
	m_Instances[ 17 ] = VInstance( D3DXVECTOR3( -2.0f,  2.0f, -1.0f ), 2 );
	m_Instances[ 18 ] = VInstance( D3DXVECTOR3( -2.0f,  2.0f, -2.0f ), 2 );
	m_Instances[ 19 ] = VInstance( D3DXVECTOR3( -1.0f,  2.0f, -2.0f ), 2 );
	m_Instances[ 20 ] = VInstance( D3DXVECTOR3(  0.0f,  2.0f, -2.0f ), 2 );
	m_Instances[ 21 ] = VInstance( D3DXVECTOR3(  1.0f,  2.0f, -2.0f ), 2 );
	m_Instances[ 22 ] = VInstance( D3DXVECTOR3(  2.0f,  2.0f, -2.0f ), 2 );
	m_Instances[ 23 ] = VInstance( D3DXVECTOR3(  2.0f,  2.0f, -1.0f ), 2 );
	m_Instances[ 24 ] = VInstance( D3DXVECTOR3(  2.0f,  2.0f,  0.0f ), 2 );
	m_Instances[ 25 ] = VInstance( D3DXVECTOR3(  2.0f,  2.0f,  1.0f ), 2 );

	// Second leaves layer
	m_Instances[ 26 ] = VInstance( D3DXVECTOR3(  1.0f,  3.0f,  2.0f ), 2 );
	m_Instances[ 27 ] = VInstance( D3DXVECTOR3(  0.0f,  3.0f,  2.0f ), 2 );
	m_Instances[ 28 ] = VInstance( D3DXVECTOR3( -1.0f,  3.0f,  2.0f ), 2 );
	m_Instances[ 29 ] = VInstance( D3DXVECTOR3( -2.0f,  3.0f,  1.0f ), 2 );
	m_Instances[ 30 ] = VInstance( D3DXVECTOR3( -2.0f,  3.0f,  0.0f ), 2 );
	m_Instances[ 31 ] = VInstance( D3DXVECTOR3( -2.0f,  3.0f, -1.0f ), 2 );
	m_Instances[ 32 ] = VInstance( D3DXVECTOR3( -1.0f,  3.0f, -2.0f ), 2 );
	m_Instances[ 33 ] = VInstance( D3DXVECTOR3(  0.0f,  3.0f, -2.0f ), 2 );
	m_Instances[ 34 ] = VInstance( D3DXVECTOR3(  1.0f,  3.0f, -2.0f ), 2 );
	m_Instances[ 35 ] = VInstance( D3DXVECTOR3(  2.0f,  3.0f, -1.0f ), 2 );
	m_Instances[ 36 ] = VInstance( D3DXVECTOR3(  2.0f,  3.0f,  0.0f ), 2 );
	m_Instances[ 37 ] = VInstance( D3DXVECTOR3(  2.0f,  3.0f,  1.0f ), 2 );
	
	// Third leaves layer
	m_Instances[ 38 ] = VInstance( D3DXVECTOR3(  1.0f,  4.0f,  1.0f ), 2 );
	m_Instances[ 39 ] = VInstance( D3DXVECTOR3(  0.0f,  4.0f,  1.0f ), 2 );
	m_Instances[ 40 ] = VInstance( D3DXVECTOR3( -1.0f,  4.0f,  1.0f ), 2 );
	m_Instances[ 41 ] = VInstance( D3DXVECTOR3( -1.0f,  4.0f,  0.0f ), 2 );
	m_Instances[ 42 ] = VInstance( D3DXVECTOR3( -1.0f,  4.0f, -1.0f ), 2 );
	m_Instances[ 43 ] = VInstance( D3DXVECTOR3(  0.0f,  4.0f, -1.0f ), 2 );
	m_Instances[ 44 ] = VInstance( D3DXVECTOR3(  1.0f,  4.0f, -1.0f ), 2 );
	m_Instances[ 45 ] = VInstance( D3DXVECTOR3(  1.0f,  4.0f,  0.0f ), 2 );

	// Forth leaves layer
	m_Instances[ 46 ] = VInstance( D3DXVECTOR3(  0.0f,  5.0f,  0.0f ), 2 );
	m_Instances[ 47 ] = VInstance( D3DXVECTOR3(  1.0f,  5.0f,  0.0f ), 2 );
	m_Instances[ 48 ] = VInstance( D3DXVECTOR3( -1.0f,  5.0f,  0.0f ), 2 );
	m_Instances[ 49 ] = VInstance( D3DXVECTOR3(  0.0f,  5.0f,  1.0f ), 2 );
	m_Instances[ 50 ] = VInstance( D3DXVECTOR3(  0.0f,  5.0f, -1.0f ), 2 );

	// Fill instance buffer description
	D3D10_BUFFER_DESC inbDesc;
	inbDesc.Usage			= D3D10_USAGE_DYNAMIC;
	inbDesc.ByteWidth		= sizeof( VInstance ) * g_BlockTreeNumInstances;
	inbDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	inbDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
	inbDesc.MiscFlags		= 0;

	// Set instances to buffer memory
	D3D10_SUBRESOURCE_DATA iniData;
	iniData.pSysMem = m_Instances;

	// Create instance buffer
	HR( m_pDevice->CreateBuffer( &inbDesc, &iniData, &m_pInstanceBuffer ) );
}

void CBlockTree::Draw()
{
	unsigned strides[ 2 ] = { sizeof( Vertex ), sizeof( VInstance ) };
	unsigned offsets[ 2 ] = { 0, 0 };
	ID3D10Buffer* buffers[ 2 ] = { m_pVertexBuffer, m_pInstanceBuffer };
	m_pDevice->IASetVertexBuffers( 0, 2, buffers, strides, offsets );
	m_pDevice->DrawInstanced( g_VerticesPerCube, g_BlockTreeNumInstances, 0, 0 ); 
}

void CBlockTree::MapBuffer()
{
	VInstance* pInstance = nullptr;
	m_pInstanceBuffer->Map( D3D10_MAP_WRITE_NO_OVERWRITE, 0, ( void** ) &pInstance );

	for( unsigned i = 0; i < g_BlockTreeNumInstances; ++i )
	{
		pInstance[ i ].pos = m_Instances[ i ].pos + m_vPosition;
	}

	m_pInstanceBuffer->Unmap();
}
