#ifndef GUARD_AIDEN_STOREY_FLAT_SURFACE_H_20140626
#define GUARD_AIDEN_STOREY_FLAT_SURFACE_H_20140626

// Local Include
#include "d3dUtil.h"

// Prototypes
class CFlatSurface
{
	// Member Functions
public:
	// Constructors
	CFlatSurface();
	~CFlatSurface();

	// Accessors
	float GetHeight() const;
	void SetHeight( const float _fHeight );

	// Other
	void Initialise( ID3D10Device* _pDevice, D3DXVECTOR3 _vCenter, DWORD _dwSize );
	void Update( float _fDT );
	void Draw();

protected:
	// Other
	void MapBuffer();

private:

	// Member Variables
public:
protected:
private:
	DWORD			m_dwNumVertices;
	DWORD			m_dwNumIndices;
		
	DWORD			m_dwWidth;
	DWORD			m_dwDepth;

	ID3D10Device*	m_pDevice;
	ID3D10Buffer*	m_pVertexBuffer;
	ID3D10Buffer*	m_pIndexBuffer;

	float			m_fTimePassed;
	float			m_fHeight;
	UINT			m_uiTexID;
};

#endif // GUARD_AIDEN_STOREY_FLAT_SURFACE_H_20140626