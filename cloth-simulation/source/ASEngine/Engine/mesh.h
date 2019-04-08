//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_MESH_H_201411021441
#define GUARD_AIDEN_STOREY_MESH_H_201411021441

// Library Includes
#include <D3DX10.h>
#include <string>
#include <vector>

// Local Includes
#include "object_3d.h"
#include "texture.h"
#include "vertex.h"

// Prototypes
class CMesh : public CObject3D
{
	friend class CObject3DManager;

	// Member Functions
public:
	// Accessors
	bool IsLoadSuccessful();
	void SetTexture( std::wstring _kstrTexture );

protected:
	// Constructors
	CMesh( const std::wstring _kstrMeshFile, bool _bCompiled = false,
		   const float& _kfPositionX = 0.0f, const float& _kfPositionY = 0.0f, const float& _kfPositionZ = 0.0f,
		   const float& _kfRotationX = 0.0f, const float& _kfRotationY = 0.0f, const float& _kfRotationZ = 0.0f,
		   const float _kfScaleX = 1.0f, const float& _kfScaleY = 1.0f, const float _kfScaleZ = 1.0f );
	~CMesh();

	// Other
	bool LoadOBJ( const std::wstring _kstrMeshFile );
	bool LoadCompiledOBJ( const std::wstring _kstrMeshFile );

	void OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset ) override;

private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*				m_pDevice;
	ID3D10Buffer*				m_pVertexBuffer{ nullptr };
	std::vector< VTextured3D >	m_VertexList;
	CTexture*					m_pTexture;

	unsigned					m_uVertexCount;

	bool						m_bLoadSuccessful;
};

#endif // GUARD_AIDEN_STOREY_MESH_H_201411021441