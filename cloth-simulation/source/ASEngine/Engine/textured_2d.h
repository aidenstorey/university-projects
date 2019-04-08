//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_TEXTURED_2D_H_201409231707
#define GUARD_AIDEN_STOREY_TEXTURED_2D_H_201409231707

// Library Includes
#include <D3DX10.h>
#include <string>
#include <vector>

// Local includes
#include "object_2d.h"
#include "texture.h"
#include "shader_textured_2d.h"
#include "vertex.h"

// Prototypes
class CTextured2D : public CObject2D
{
	friend class CObject2DManager;

	// Member Functions
public:
	// Accessors
	CTexture* GetTexture() const;
	void SetTexture( CTexture* _pTexture );

	const std::vector< VTextured2D >* GetVertices() const;

protected:
	// Constructors
	CTextured2D( const std::wstring& _kstrName = L"", std::wstring _krTextureName = L"DefaultTexture.png", const float _kfPositionX = 0.0f,
				 const float _kfPositionY = 0.0f, const float _kfRotation = 0.0f, const float _kfScaleX = 1.0f, const float _kfScaleY = 1.0f );
	~CTextured2D();

	// Other
	void InitialiseObject();
	virtual void OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset ) override;	

private:

	// Member Variables
public:
protected:
	CTexture*					m_pTexture;
	std::vector< VTextured2D >	m_VertexList;

private:
	ID3D10Device*				m_pDevice;
	CShaderTextured2D*			m_pShader;
	
	ID3D10Buffer*				m_pVertexBuffer{ nullptr };
};

#endif // GUARD_AIDEN_STOREY_TEXTURED_2D_H_201409231707