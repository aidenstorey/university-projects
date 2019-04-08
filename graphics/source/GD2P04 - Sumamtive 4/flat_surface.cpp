// Local Includes
#include "Vertex.h"

// This Include
#include "flat_surface.h"

// Implementation
CFlatSurface::CFlatSurface() :
	m_dwNumVertices( 0 ), m_dwNumIndices( 0 ), m_dwWidth( 0 ), m_dwDepth( 0 ),
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ), m_pIndexBuffer( nullptr ),
	m_fTimePassed( 0.0f ), m_fHeight( 0.0f ), m_uiTexID( 0 )
{}

CFlatSurface::~CFlatSurface()
{
	ReleaseCOM( m_pVertexBuffer );
	ReleaseCOM( m_pIndexBuffer );
}

float CFlatSurface::GetHeight() const
{
	return( m_fHeight );
}

void CFlatSurface::SetHeight( const float _fHeight )
{
	m_fHeight = _fHeight;
}

void CFlatSurface::Initialise( ID3D10Device* _pDevice, D3DXVECTOR3 _vCenter, DWORD _dwSize )
{
	// Local copy
	m_pDevice = _pDevice;
	m_fHeight = _vCenter.y;

	m_dwWidth = _dwSize;
	m_dwDepth = _dwSize;

	m_dwNumVertices = m_dwWidth * m_dwDepth;
	m_dwNumIndices	= ( m_dwWidth - 1 ) * ( m_dwDepth - 1 ) * g_kiIndicesPerQuad;

	VertexID* pVertices = new VertexID[ m_dwNumVertices ];

	float fStartX = _vCenter.x - static_cast< float >( _dwSize / 2.0f );
	float fStartZ = _vCenter.z - static_cast< float >( _dwSize / 2.0f );

	for( unsigned i = 0; i < m_dwWidth; ++i )
	{
		for( unsigned j = 0; j < m_dwDepth; ++j )
		{
			pVertices[ i * m_dwWidth + j ].pos = D3DXVECTOR3( fStartX + i, _vCenter.y, fStartZ + j );
			pVertices[ i * m_dwWidth + j ].normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
			pVertices[ i * m_dwWidth + j ].texC = D3DXVECTOR2( static_cast< float >( i ), static_cast< float >( j ) );
			pVertices[ i * m_dwWidth + j ].tangent = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		}
	}

	// Fill vertex buffer description
	D3D10_BUFFER_DESC vBufDesc;
	vBufDesc.Usage			= D3D10_USAGE_DYNAMIC;
	vBufDesc.ByteWidth		= sizeof( VertexID ) * m_dwNumVertices;
	vBufDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vBufDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vBufDesc.MiscFlags		= 0;

	// Set vertices for buffer
	D3D10_SUBRESOURCE_DATA vInitData;
	vInitData.pSysMem = pVertices;

	// Create vertex buffer
	HR( m_pDevice->CreateBuffer( &vBufDesc, &vInitData, &m_pVertexBuffer ) );

	// Create array to store indices
	DWORD* pIndices = new DWORD[ m_dwNumIndices ];
	
	// Create look up for offsets of quad
	DWORD dwQuadOffset[] = { 0, 1, m_dwWidth + 1, m_dwWidth, 0, m_dwWidth + 1 };

	// Creates indices
	int index = 0;
	for( unsigned i = 0; i < m_dwWidth - 1; ++i )
	{
		for( unsigned j = 0; j < m_dwDepth - 1; ++j )
		{
			for( int k = 0; k < g_kiIndicesPerQuad; ++k )
			{
				pIndices[ index++ ] = ( i * m_dwWidth ) + j + dwQuadOffset[ k ];
				
			}
		}
	}

	// Fill index buffer description
	D3D10_BUFFER_DESC iBufDesc;
	iBufDesc.Usage			= D3D10_USAGE_IMMUTABLE;
	iBufDesc.ByteWidth		= sizeof( DWORD ) * m_dwNumIndices;
	iBufDesc.BindFlags		= D3D10_BIND_INDEX_BUFFER;
	iBufDesc.CPUAccessFlags = 0;
	iBufDesc.MiscFlags		= 0;

	// Set indices for buffer
	D3D10_SUBRESOURCE_DATA iInitData;
	iInitData.pSysMem = pIndices;
	
	// Create index buffer
	HR( m_pDevice->CreateBuffer( &iBufDesc, &iInitData, &m_pIndexBuffer ) );

	// Clean up 
	delete [] pVertices;
	delete [] pIndices;
	MapBuffer();
}

void CFlatSurface::Update( float _fDT )
{
	m_fTimePassed += _fDT;

	if( m_fTimePassed > 1.0f / 10.0f )
	{
		m_fTimePassed = 0.0f;
		m_uiTexID = ( m_uiTexID + 1 ) % 4;
		MapBuffer();
	}
}

void CFlatSurface::Draw()
{
	UINT stride = sizeof( VertexID );
	UINT offset = 0;
	m_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	m_pDevice->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	m_pDevice->DrawIndexed( m_dwNumIndices, 0, 0 );
}

void CFlatSurface::MapBuffer()
{
	VertexID* pVertex = nullptr;
	m_pVertexBuffer->Map( D3D10_MAP_WRITE_NO_OVERWRITE, 0, ( void** ) &pVertex );

	for( unsigned i = 0; i < m_dwNumVertices; ++i )
	{
		pVertex[ i ].pos.y = m_fHeight;
		pVertex[ i ].texID = m_uiTexID;
	}

	m_pVertexBuffer->Unmap();
}