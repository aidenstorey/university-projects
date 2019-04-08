// Local Includes
#include "Vertex.h"

// This Include 
#include "single_block.h"

// Implementation
CSingleBlock::CSingleBlock() :
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ), m_pInstanceBuffer( nullptr ),
	m_dwNumVertices( 0 )
{}

CSingleBlock::~CSingleBlock()
{
	ReleaseCOM( m_pInstanceBuffer );
}

void CSingleBlock::SetPosition( const D3DXVECTOR3 _vPosition )
{
	m_Instance.pos = _vPosition;
	MapBuffer();
}

void CSingleBlock::Initialise( ID3D10Device* _pDevice, ID3D10Buffer* _pVertexBuffer, DWORD _dwNumVertices )
{
	// Local copy
	m_pDevice = _pDevice;
	m_pVertexBuffer = _pVertexBuffer;
	m_dwNumVertices = _dwNumVertices;

	// Create Instances
	m_Instance = VInstance( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 3 );

	// Fill instance buffer description
	D3D10_BUFFER_DESC inbDesc;
	inbDesc.Usage			= D3D10_USAGE_DYNAMIC;
	inbDesc.ByteWidth		= sizeof( VInstance );
	inbDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	inbDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
	inbDesc.MiscFlags		= 0;

	// Set instances to buffer memory
	D3D10_SUBRESOURCE_DATA iniData;
	iniData.pSysMem = &m_Instance;

	// Create instance buffer
	HR( m_pDevice->CreateBuffer( &inbDesc, &iniData, &m_pInstanceBuffer ) );
}

void CSingleBlock::Draw()
{
	unsigned strides[ 2 ] = { sizeof( Vertex ), sizeof( VInstance ) };
	unsigned offsets[ 2 ] = { 0, 0 };
	ID3D10Buffer* buffers[ 2 ] = { m_pVertexBuffer, m_pInstanceBuffer };
	m_pDevice->IASetVertexBuffers( 0, 2, buffers, strides, offsets );
	m_pDevice->DrawInstanced( m_dwNumVertices, 1, 0, 0 );
}

void CSingleBlock::MapBuffer()
{
	VInstance* pInstance = nullptr;
	m_pInstanceBuffer->Map( D3D10_MAP_WRITE_NO_OVERWRITE, 0, ( void** )&pInstance );

	pInstance[ 0 ] = m_Instance;

	m_pInstanceBuffer->Unmap();
}