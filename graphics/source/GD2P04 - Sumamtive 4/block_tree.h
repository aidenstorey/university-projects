#ifndef GUARD_AIDEN_STOREY_BLOCK_TREE_H_20140622
#define GUARD_AIDEN_STOREY_BLOCK_TREE_H_20140622

// Local Includes
#include "d3dUtil.h"
#include "instance_cube.h"
#include "Vertex.h"

// Constant Variables
const int g_BlockTreeNumInstances = 51;

// Prototypes
class CBlockTree
{
	// Member Functions
public:
	// Constructors
	CBlockTree();
	~CBlockTree();

	// Accessors
	void SetPosition( D3DXVECTOR3 _vPosition );

	// Other
	void Initialise( CInstanceCube* _pInstance );
	void Draw();

protected:
	void MapBuffer();
private:
	
	// Member Variables
public:
protected:
private:
	ID3D10Device*	m_pDevice;
	ID3D10Buffer*	m_pVertexBuffer;
	ID3D10Buffer*	m_pInstanceBuffer;
	
	VInstance		m_Instances[ g_BlockTreeNumInstances ];

	D3DXVECTOR3		m_vPosition;
};


#endif // GUARD_AIDEN_STOREY_BLOCK_TREE_H_20140622