#ifndef GUARD_AIDEN_STOREY_BLOCK_TERRAIN_H_20140621
#define GUARD_AIDEN_STOREY_BLOCK_TERRAIN_H_20140621

// Library Includes
#include <vector>

// Local Includes
#include "2d_point.h"
#include "d3dUtil.h"
#include "diamsq_unit.h"
#include "instance_cube.h"
#include "Vertex.h"

// Constant Variables
const int g_BlockTerrainMaxDS			= 7;
const int g_BlockTerrainSize			= 129;
const int g_BlockTerrainNumInstances	= g_BlockTerrainSize * g_BlockTerrainSize;
const int g_BlockTerrainRandomMax		= 5;

const TPoint	g_BlockTerrainPoolPos		= TPoint( 40, 35 ); 
const int		g_BlockTerrainPoolNumBlocks	= 21;
const TPoint	g_BlockTerrainPoolOffset[ g_BlockTerrainPoolNumBlocks ]	= 
{
	TPoint( 0, -1 ),
	TPoint( 0, 0 ),
	TPoint( 0, 1 ),
	TPoint( -1, -1 ),
	TPoint( -1, 0 ),
	TPoint( -1, 1 ),
	TPoint( 1, -1 ),
	TPoint( 1, 0 ),
	TPoint( 1, 1 ),

	TPoint( 2, 1 ),
	TPoint( 2, 0 ),
	TPoint( 2, -1 ),
	TPoint( -2, 1 ),
	TPoint( -2, 0 ),
	TPoint( -2, -1 ),
	TPoint( 1, 2 ),
	TPoint( 0, 2 ),
	TPoint( -1, 2 ),
	TPoint( 1, -2 ),
	TPoint( 0, -2 ),
	TPoint( -1, -2 ),
};

const int		g_BlockTerrainSideNumBlocks = 16;
const TPoint	g_BlockTerrainSideOffset[ g_BlockTerrainSideNumBlocks ] = 
{
	TPoint( 3, 1 ),
	TPoint( 3, 0 ),
	TPoint( 3, -1 ),
	TPoint( -3, 1 ),
	TPoint( -3, 0 ),
	TPoint( -3, -1 ),
	TPoint( 1, 3 ),
	TPoint( 0, 3 ),
	TPoint( -1, 3 ),
	TPoint( 1, -3 ),
	TPoint( 0, -3 ),
	TPoint( -1, -3 ),
	TPoint( 2, 2 ),
	TPoint( 2, -2 ),
	TPoint( -2, 2 ),
	TPoint( -2, -2 ),
};

// Prototypes
class CBlockTerrain
{
	// Member Functions
public:
	// Constructors
	CBlockTerrain();
	~CBlockTerrain();

	// Accessors
	int	GetHeightAtPoint( TPoint _tPoint );
	float GetPoolHeight() const;

	// Other
	void Initialise( ID3D10Device* _pDevice, CInstanceCube* _pInstance );
	void Draw();

	bool IsSmoothed();
	void RunDiamondSquare();
	void RunSmoothing();

	void CreatePond();

	void Reset();

protected:
	void InitialiseVertexBuffer();
	void InitialiseInstanceBuffer();
		
	void MapInstanceBuffer();

	void DiamondSquare( TPoint _FL, TPoint _NR, int _iCount );
	void DiamondSquare( TDSUnit _tUnit, std::vector< TDSUnit >& _rNext );
	void SmoothTerrain( int _iPasses );
	int SmoothPoint( int _iDepth, int _iWidth );

private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*			m_pDevice;
	ID3D10Buffer*			m_pVertexBuffer;
	ID3D10Buffer*			m_pInstanceBuffer;

	DWORD					m_dwNumInstances;
		
	Vertex					m_Vertices[ g_VerticesPerCube ];
	VInstance				m_Instances[ g_BlockTerrainNumInstances ];

	bool					m_bSmoothed;
	int						m_iDSHeight[ g_BlockTerrainSize ][ g_BlockTerrainSize ];
	int						m_iCurrentDS;

	float					m_fPoolHeight;

	std::vector< TDSUnit >	m_vecNextDS;
};

#endif // GUARD_AIDEN_STOREY_BLOCK_TERRAIN_H_20140621