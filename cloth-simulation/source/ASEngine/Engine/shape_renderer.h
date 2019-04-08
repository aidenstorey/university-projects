//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SHAPE_RENDERER_H_201409212201
#define GUARD_AIDEN_STOREY_SHAPE_RENDERER_H_201409212201

// Library Includes
#include <D3DX10.h>

// Local Includes
#include "camera.h"

// Prototyes
class CShapeRenderer
{
	// Member Functions
public:
	// Constructors
	CShapeRenderer();
	~CShapeRenderer();

	// Other
	void Initialise();

	void DrawRectangle2D( CCamera* _pRenderCamera, const D3DXMATRIX& _kmatWorld,
						  const D3DXVECTOR2& _kvSize, const D3DXCOLOR _kColor );

	void DrawRectangle2D( CCamera* _pRenderCamera, const D3DXVECTOR2& _kvPosition, const float& _kfRotation,
						  const D3DXVECTOR2& _kvSize, const D3DXCOLOR _kColor );

	void DrawCircle2D( CCamera* _pRenderCamera, const D3DXMATRIX& _kmatWorld,
					   const float& _kfRadius, const D3DXCOLOR _kColor );

	void DrawCircle2D( CCamera* _pRenderCamera, const D3DXVECTOR2& _kvPosition,
					   const float& _kfRadius, const D3DXCOLOR _kColor );

	void DrawLines2D( CCamera* _pRenderCamera, const D3DXMATRIX& _kmatWorld, const D3DXVECTOR2* _pVertices,
					  const unsigned& _kuNumVertices, const D3DXCOLOR _kColor );

protected:
	void MapBuffer( const D3DXVECTOR2* _pVertices, const unsigned& _kuNumVertices, const D3DXCOLOR _kColor );

private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*	m_pDevice{ nullptr };
	ID3D10Buffer*	m_pVertexBuffer{ nullptr };

	const unsigned	m_MaxVertices;
	D3DXMATRIX		m_IdentityMatrix;
};

#endif // GUARD_AIDEN_STOREY_SHAPE_RENDERER_H_201409212201