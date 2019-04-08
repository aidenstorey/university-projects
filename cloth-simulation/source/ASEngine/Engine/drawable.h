//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_DRAWABLE_H_201409191014
#define GUARD_AIDEN_STOREY_DRAWABLE_H_201409191014

// Library Includes
#include <D3DX10.h>
#include <vector>

// Local Includes
#include "camera.h"

// Prototypes
class CDrawable
{
	friend class CScene;

	// Member Fucntions
public:
	// Constructors
	CDrawable();
	~CDrawable();

	// Accessors 
	bool IsVisible() const;
	void SetVisible( const bool _kbVisible );

	D3DXMATRIX GetWorldMatrix() const;
	void SetWorldMatrix( const D3DXMATRIX& _krWorld );

	// Other
	bool Process( const float& _kfTimeDelta, CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset );

	bool AttachChild( CDrawable* _pDrawable );
	void DetachChild( CDrawable* _pDrawable );

protected:
	// Other
	virtual bool OnUpdate( const float& _kfTimeDelta );
	virtual bool PreUpdate( const float& _kfTimeDelta );
	virtual bool PostUpdate( const float& _kfTimeDelta );

	virtual void OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset );
	virtual void PreDraw( CCamera* _pRednerCamera, const D3DXMATRIX& _krOffset );
	virtual void PostDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset );


private:
	virtual bool Update( const float& _kfTimeDelta );
	virtual void Draw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset );

	// Member Variables
public:
protected:
	bool						m_bIsVisible;
	D3DXMATRIX					m_matWorld;

private:
	CDrawable*					m_pParent;
	std::vector< CDrawable* >	m_ChildList;
};

#endif // GUARD_AIDEN_STOREY_DRAWABLE_H_201409191014