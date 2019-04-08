#ifndef GUARD_AIDEN_STOREY_NDC_QUAD_H_20140627
#define GUARD_AIDEN_STOREY_NDC_QUAD_H_20140627

// Local Includes
#include "d3dUtil.h"

// Prototypes
class CNDCQuad
{
	// Member Functions
public:
	// Constructors
	CNDCQuad();
	~CNDCQuad();

	// Other
	void Initialise( ID3D10Device* _pDevice );
	void Draw();

protected:
private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*	m_pDevice;
	ID3D10Buffer*	m_pVertexBuffer;
};

#endif // GUARD_AIDEN_STOREY_NDC_QUAD_H_20140627