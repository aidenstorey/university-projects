// Library Includes
#include <queue>

// Local Includes
#include "Vertex.h"

// This Include
#include "block_terrain.h"

// Implementation
CBlockTerrain::CBlockTerrain() :
	m_pDevice( nullptr ), m_pVertexBuffer( nullptr ), 
	m_pInstanceBuffer( nullptr ), m_dwNumInstances( 0 ),
	m_bSmoothed( false ), m_iCurrentDS( 0 )
{	
	memset( m_iDSHeight, 0, sizeof( m_iDSHeight[ 0 ][ 0 ] ) * g_BlockTerrainSize * g_BlockTerrainSize );	
}

CBlockTerrain::~CBlockTerrain()
{
	ReleaseCOM( m_pInstanceBuffer );
}

int	CBlockTerrain::GetHeightAtPoint( TPoint _tPoint )
{
	int iY = _tPoint.y + ( ( g_BlockTerrainSize - 1 ) / 2 ) - ( _tPoint.y < 0 ? 1 : 0 );
	int iX = _tPoint.x + ( ( g_BlockTerrainSize - 1 ) / 2 ) - ( _tPoint.x < 0 ? 1 : 0 );

	int iReturn = m_iDSHeight[ iY ][ iX ];

 	return ( iReturn );
}

float CBlockTerrain::GetPoolHeight() const
{
	return ( m_fPoolHeight );
}

void CBlockTerrain::Initialise( ID3D10Device* _pDevice, CInstanceCube* _pInstance  )
{
	// Local copy 
	m_pDevice = _pDevice;	

	// Initialise Variables
	m_bSmoothed = false;
	m_iCurrentDS = 0;
	m_vecNextDS.push_back( TDSUnit( TPoint( 0, 0 ), TPoint( g_BlockTerrainSize - 1, g_BlockTerrainSize - 1 ) ) );

	// Set the buffer to be instanced
	m_pVertexBuffer = _pInstance->GetVertexBuffer();

	// Create instances
	InitialiseInstanceBuffer();	
}

void CBlockTerrain::Draw()
{
	unsigned strides[ 2 ] = { sizeof( Vertex ), sizeof( VInstance ) };
	unsigned offsets[ 2 ] = { 0, 0 };
	ID3D10Buffer* buffers[ 2 ] = { m_pVertexBuffer, m_pInstanceBuffer };
	m_pDevice->IASetVertexBuffers( 0, 2, buffers, strides, offsets );
	m_pDevice->DrawInstanced( g_VerticesPerCube, g_BlockTerrainNumInstances, 0, 0 ); 
}

void CBlockTerrain::InitialiseInstanceBuffer()
{
	// Calculate number of instances
	m_dwNumInstances = g_BlockTerrainSize * g_BlockTerrainSize;

	// Value to access arrays correctly
	int iAccess = g_BlockTerrainSize - 1;
	
	// Seed Diamond Square
	m_iDSHeight[ 0 ][ 0 ] = 5 ;
	m_iDSHeight[ iAccess ][ 0 ] = 5;
	m_iDSHeight[ 0 ][ iAccess ] = 5;
	m_iDSHeight[ iAccess ][ iAccess ] = 5;
	
	// Populate instance storage
	unsigned iIndex = 0;
	for( int i = 0; i < g_BlockTerrainSize; ++i )
	{
		for( int j = 0; j < g_BlockTerrainSize; ++j )
		{
			m_Instances[ iIndex++ ] = VInstance( D3DXVECTOR3( static_cast< float >( i - g_BlockTerrainSize / 2 ), 
				static_cast< float >( m_iDSHeight[ i ][ j ] ), static_cast< float >( j - g_BlockTerrainSize / 2 ) ), 0 );
		}
	}

	// Fill instance buffer description
	D3D10_BUFFER_DESC inbDesc;
	inbDesc.Usage			= D3D10_USAGE_DYNAMIC;
	inbDesc.ByteWidth		= sizeof( VInstance ) * g_BlockTerrainNumInstances;
	inbDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	inbDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
	inbDesc.MiscFlags		= 0;

	// Set instances to buffer memory
	D3D10_SUBRESOURCE_DATA iniData;
	iniData.pSysMem = m_Instances;

	// Create instance buffer
	HR( m_pDevice->CreateBuffer( &inbDesc, &iniData, &m_pInstanceBuffer ) );
}

void CBlockTerrain::MapInstanceBuffer()
{
	VInstance* pInstance = nullptr;
	m_pInstanceBuffer->Map( D3D10_MAP_WRITE_NO_OVERWRITE, 0, ( void** ) &pInstance );

	for( unsigned i = 0; i < m_dwNumInstances; ++i )
	{
		pInstance[ i ].pos.y = static_cast< float >( m_iDSHeight[ i / g_BlockTerrainSize ][ i % g_BlockTerrainSize ] );
		pInstance[ i ].texID = m_Instances[ i ].texID;
	}

	m_pInstanceBuffer->Unmap();
}

bool CBlockTerrain::IsSmoothed()
{
	return ( m_bSmoothed );
}

void CBlockTerrain::RunDiamondSquare()
{
	// If it is possible to perform algorithm
	if( m_iCurrentDS < g_BlockTerrainMaxDS && m_vecNextDS.size() > 0 )
	{
		// Increment the count of divisions
		++m_iCurrentDS;

		// Container for new units for next DS
		std::vector< TDSUnit > vecNewPoints;

		// Loops through current units for DS
		for( unsigned i = 0; i < m_vecNextDS.size(); ++i )
		{
			// Calls algorithm on current unit and pass storage 
			// so new subdivided elements can be added
			DiamondSquare( m_vecNextDS[ i ], vecNewPoints );
		}

		// Clear list and assign new one
		m_vecNextDS.clear();
		m_vecNextDS.assign( vecNewPoints.begin(), vecNewPoints.end() );

		// Re-maps instances with new heights
		MapInstanceBuffer();
	}
	else
	{
		// Call smoothing function
		RunSmoothing();
	}
}

void CBlockTerrain::RunSmoothing()
{
	// Set smoothing to true
	m_bSmoothed = true;
	
	// Run smoothing and re-map buffer
	SmoothTerrain( m_iCurrentDS == 7 ? 5 : 1 );
	MapInstanceBuffer();

	// Clear buffer
	m_vecNextDS.clear();
}

void CBlockTerrain::Reset()
{
	// Set default values for terrain
	m_bSmoothed = false;
	m_iCurrentDS = 0;
	
	// Clear any current units
	m_vecNextDS.clear();

	// Push back first unit to be used
	m_vecNextDS.push_back( TDSUnit( TPoint( 0, 0 ), TPoint( g_BlockTerrainSize - 1, g_BlockTerrainSize - 1 ) ) );

	// Set all values back to 0
	memset( m_iDSHeight, 0, sizeof( m_iDSHeight[ 0 ][ 0 ] ) * g_BlockTerrainSize * g_BlockTerrainSize );	
	
	// Value to access arrays correctly
	int iAccess = g_BlockTerrainSize - 1;

	// Reset texture id of pool blocks
	for( int i = 0; i < g_BlockTerrainPoolNumBlocks; ++i )
	{
		int iY = static_cast< int >( ( g_BlockTerrainSize / 2.0f ) + g_BlockTerrainPoolPos.y + g_BlockTerrainPoolOffset[ i ].y );
		int iX = static_cast< int >( ( g_BlockTerrainSize / 2.0f ) + g_BlockTerrainPoolPos.x + g_BlockTerrainPoolOffset[ i ].x ); 
		m_Instances[ iY * g_BlockTerrainSize + iX ].texID = 0; 
	}
	
	// Seed Diamond Square
	m_iDSHeight[ 0 ][ 0 ] = 5 ;
	m_iDSHeight[ iAccess ][ 0 ] = 5;
	m_iDSHeight[ 0 ][ iAccess ] = 5;
	m_iDSHeight[ iAccess ][ iAccess ] = 5;

	// Re-map buffer
	MapInstanceBuffer();
}

void CBlockTerrain::DiamondSquare( TPoint _FL, TPoint _NR, int _iCount )
{
	if( _iCount > 0 )
	{
		// Calculate center point
		TPoint C = ( _FL + ( _NR - _FL ) / 2 );

		int FLH = m_iDSHeight[ _FL.x ][ _FL.y ];
		int NLH = m_iDSHeight[ _FL.x ][ _NR.y ];
		int FRH = m_iDSHeight[ _NR.x ][ _FL.y ];
		int NRH = m_iDSHeight[ _NR.x ][ _NR.y ];

		// Calculates center point height
		m_iDSHeight[ C.x ][ C.y ] = ( FLH + NLH + FRH + NRH ) / 4 + rand() % g_BlockTerrainRandomMax;

		// Calculates diamond point heights
		m_iDSHeight[ C.x ][ _FL.y ] = ( FLH + FRH ) / 2 + rand() % g_BlockTerrainRandomMax;
		m_iDSHeight[ _FL.x ][ C.y ] = ( FLH + NLH ) / 2 + rand() % g_BlockTerrainRandomMax;
		m_iDSHeight[ C.x ][ _NR.y ] = ( NLH + NRH ) / 2 + rand() % g_BlockTerrainRandomMax;
		m_iDSHeight[ _NR.x ][ C.y ] = ( NRH + FRH ) / 2 + rand() % g_BlockTerrainRandomMax;

		// Call function on smaller area
		DiamondSquare( _FL, C, _iCount - 1 );
		DiamondSquare( C, _NR, _iCount - 1 );
		DiamondSquare( TPoint( C.x, _FL.y ), TPoint( _NR.x, C.y ), _iCount - 1 );
		DiamondSquare( TPoint( _FL.x, C.y ), TPoint( C.x, _NR.y ), _iCount - 1 );
	}
}

void CBlockTerrain::DiamondSquare( TDSUnit _tUnit, std::vector< TDSUnit >& _rNext  )
{
	// Calculate points for accessing
	TPoint FL = _tUnit.fl;
	TPoint NR = _tUnit.nr;
	TPoint C = ( FL + ( NR - FL ) / 2 );

	// Get the height of corners
	int FLH = m_iDSHeight[ FL.x ][ FL.y ];
	int NLH = m_iDSHeight[ FL.x ][ NR.y ];
	int FRH = m_iDSHeight[ NR.x ][ FL.y ];
	int NRH = m_iDSHeight[ NR.x ][ NR.y ];

	// Calculates center point height
	m_iDSHeight[ C.x ][ C.y ] = ( FLH + NLH + FRH + NRH ) / 4 + rand() % g_BlockTerrainRandomMax;

	// Calculates diamond point heights
	m_iDSHeight[ C.x ][ FL.y ] = ( FLH + FRH ) / 2 + rand() % g_BlockTerrainRandomMax;
	m_iDSHeight[ FL.x ][ C.y ] = ( FLH + NLH ) / 2 + rand() % g_BlockTerrainRandomMax;
	m_iDSHeight[ C.x ][ NR.y ] = ( NLH + NRH ) / 2 + rand() % g_BlockTerrainRandomMax;
	m_iDSHeight[ NR.x ][ C.y ] = ( NRH + FRH ) / 2 + rand() % g_BlockTerrainRandomMax;

	// If another split possible
	if( m_iCurrentDS < g_BlockTerrainMaxDS )
	{
		// Push back new units into vector
		_rNext.push_back( TDSUnit( FL, C ) );
		_rNext.push_back( TDSUnit( C, NR ) );
		_rNext.push_back( TDSUnit( TPoint( C.x, FL.y ), TPoint( NR.x, C.y ) ) );
		_rNext.push_back( TDSUnit( TPoint( FL.x, C.y ), TPoint( C.x, NR.y ) ) );
	}
}

void CBlockTerrain::SmoothTerrain( int _iPasses )
{
	for( int p = 0; p < _iPasses; ++p )
	{
		for( unsigned i = 0; i < g_BlockTerrainSize; ++i )
		{
			for( unsigned j = 0; j < g_BlockTerrainSize; ++j )
			{
				m_iDSHeight[ i ][ j ] = SmoothPoint( i, j );
			}
		}
	}
	
	CreatePond();
}

int CBlockTerrain::SmoothPoint( int _iDepth, int _iWidth )
{
	int iReturn = 0;
	int iPoints = 0;

	for( int i = _iDepth - 1; i <= _iDepth + 1; ++i )
	{
		for( int j = _iWidth - 1; j <= _iWidth + 1; ++j )
		{
			if( ( 0 <= i && i < g_BlockTerrainSize ) && ( 0 <= j && j < g_BlockTerrainSize ) )
			{
				iReturn += m_iDSHeight[ i ][ j ];
				++iPoints;
			}
		}
	}

	if( iPoints > 0 )
	{
		iReturn /= iPoints;
	}

	return ( iReturn );
}

void CBlockTerrain::CreatePond()
{
	int iAverageHeight = 0;

	for( unsigned i = 0; i < g_BlockTerrainSize; ++i )
	{
		for( unsigned j = 0; j < g_BlockTerrainSize; ++j )
		{
			iAverageHeight += m_iDSHeight[ i ][ j ];
		}
	}

	iAverageHeight = iAverageHeight / ( g_BlockTerrainSize * g_BlockTerrainSize );	

	for( int i = 0; i < g_BlockTerrainPoolNumBlocks; ++i )
	{
		int iY = static_cast< int >( ( g_BlockTerrainSize / 2.0f ) + g_BlockTerrainPoolPos.y + g_BlockTerrainPoolOffset[ i ].y );
		int iX = static_cast< int >( ( g_BlockTerrainSize / 2.0f ) + g_BlockTerrainPoolPos.x + g_BlockTerrainPoolOffset[ i ].x ); 
		m_iDSHeight[ iY ][ iX ] = iAverageHeight;
		m_Instances[ iY * g_BlockTerrainSize + iX ].texID = 4; 
	}

	std::queue< TPoint > qTerrainEval;

	for( int i = 0; i < g_BlockTerrainSideNumBlocks; ++i )
	{
		int iY = static_cast< int >( ( g_BlockTerrainSize / 2.0f ) + g_BlockTerrainPoolPos.y + g_BlockTerrainSideOffset[ i ].y );
		int iX = static_cast< int >( ( g_BlockTerrainSize / 2.0f ) + g_BlockTerrainPoolPos.x + g_BlockTerrainSideOffset[ i ].x );
		m_iDSHeight[ iY ][ iX ] = iAverageHeight + 1; 
		qTerrainEval.push( TPoint( iX, iY ) );
	}
	
	while( qTerrainEval.size() > 0 )
	{
		TPoint EvalPoint = qTerrainEval.front();
		qTerrainEval.pop();

		for( int i = EvalPoint.y - 1; i <= EvalPoint.y + 1; ++i )
		{
			for( int j = EvalPoint.x - 1; j <= EvalPoint.x + 1; ++j )
			{
				if( i < g_BlockTerrainSize && i >= 0 && 
					j < g_BlockTerrainSize && j >= 0 && 
					( i == EvalPoint.y || j == EvalPoint.x ) )
				{
					if( abs( m_iDSHeight[ i ][ j ] - m_iDSHeight[ EvalPoint.y ][ EvalPoint.x ] ) > 1 )
					{
						int iHeight = m_iDSHeight[ EvalPoint.y ][ EvalPoint.x ];
						iHeight += ( m_iDSHeight[ i ][ j ] < m_iDSHeight[ EvalPoint.y ][ EvalPoint.x ] ) ? -1 : 1;
						m_iDSHeight[ i ][ j ] = iHeight;
						qTerrainEval.push( TPoint( j, i ) );
					}
				}
			}
		}
	}

	m_fPoolHeight = static_cast< float >( iAverageHeight );
	MapInstanceBuffer();
}