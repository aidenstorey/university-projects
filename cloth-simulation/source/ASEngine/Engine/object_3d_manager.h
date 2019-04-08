//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_OBJECT_3D_MANAGER_H_201411021616
#define GUARD_AIDEN_STOREY_OBJECT_3D_MANAGER_H_201411021616

// Library Includes
#include <string>
#include <vector>

// Local Includes
#include "object_3d.h"
#include "mesh.h"

// Prototypes
class CObject3DManager
{
	// Member Functions
public:
	// Constructors
	CObject3DManager();
	~CObject3DManager();

	// Other 
	CMesh* CreateMesh( const std::wstring _kstrMeshFile, const float& _kfPositionX = 0.0f, const float& _kfPositionY = 0.0f, const float& _kfPositionZ = 0.0f,
					   const float& _kfRotationX = 0.0f, const float& _kfRotationY = 0.0f, const float& _kfRotationZ = 0.0f,
					   const float _kfScaleX = 1.0f, const float& _kfScaleY = 1.0f, const float _kfScaleZ = 1.0f );
	
protected:
private:

	// Member Variables
public:
protected:
private:
	std::vector< CObject3D* > m_ObjectList;
};

#endif // GUARD_AIDEN_STOREY_OBJECT_3D_MANAGER_H_201411021616