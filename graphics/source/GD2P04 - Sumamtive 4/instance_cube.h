#ifndef GUARD_AIDEN_STOREY_INSTANCE_CUBE_H_20140622
#define GUARD_AIDEN_STOREY_INSTANCE_CUBE_H_20140622

// Local Includes
#include "d3dUtil.h"

// Constants
const int g_VerticesPerCube = 36;

// Prototype
class CInstanceCube
{
	// Member functions
public: 
	// Constructors
	CInstanceCube();
	~CInstanceCube();

	// Accessors
	ID3D10Buffer* GetVertexBuffer();

	// Other
	void Initialise();
protected:
private:

	// Member variables
public:
protected:
private:
	ID3D10Buffer*	m_pVertexBuffer;
};

#endif // GUARD_AIDEN_STOREY_INSTANCE_CUBE_H_20140622