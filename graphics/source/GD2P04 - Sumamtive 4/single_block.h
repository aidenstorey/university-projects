#ifndef GUARD_AIDEN_STOREY_SINGLE_BLOCK_H
#define GUARD_AIDEN_STOREY_SINGLE_BLOCK_H

// Local Includes
#include "d3dUtil.h"

// Prototypes
class CSingleBlock
{
	// Member Functions
public:
	// Constructors
	CSingleBlock();
	~CSingleBlock();

	// Accessors
	void SetPosition( const D3DXVECTOR3 _vPosition );
	
	// Other
	void Initialise( ID3D10Device* _pDevice, ID3D10Buffer* _pVertexBuffer, DWORD _dwNumVertices  );
	void Draw();

protected:
	// Other
	void MapBuffer();

private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*	m_pDevice;
	ID3D10Buffer*	m_pVertexBuffer;
	ID3D10Buffer*	m_pInstanceBuffer;

	VInstance		m_Instance;
	
	DWORD			m_dwNumVertices;
};

#endif // GUARD_AIDEN_STOREY_SINGLE_BLOCK_H