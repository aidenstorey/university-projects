//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SCENE_H_201409120959
#define GUARD_AIDEN_STOREY_SCENE_H_201409120959

// Library Includes
#include <vector>

// Local Includes
#include "camera.h"
#include "drawable.h"
#include "viewport.h"

// Prototype
class CScene : protected CDrawable
{
	// Member Functions
public:
	// Constructors
	CScene( const int _kiSceneID );
	virtual ~CScene();

	// Accessors
	bool IsInitialised();

	int GetSceneID();

	CCamera& GetCamera();
	void SetCamera( CCamera* _pCamera );

	CCamera& GetUICamera();
	
	// Other
	bool ProcessScene( const float& _kfTimeDelta );

	virtual bool OnActivate();
	virtual bool OnDeactivate();

	void AttachChildScene( CScene* _pScene );

	bool AttachUIElement( CDrawable* _pElement );
	void DetachUIElement( CDrawable* _pElement );

	CViewport* AddViewport();

protected:
	// Other
	virtual bool PostUpdate( const float& _kfTimeDelta ) override;

private:
	virtual bool Update( const float& _kfTimeDelta ) override;
	virtual void Draw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset ) override;

	// Member Variables
public:
protected:
	bool						m_bIsInitialised;

	CCamera*					m_pActiveCamera{ nullptr };
	std::vector< CViewport >	m_ViewportList;

	CCamera*					m_pUICamera{ nullptr };
	CViewport					m_UIViewport;

private:
	int							m_iSceneID;
	bool						m_bPaused;

	std::vector< CScene* >		m_ChildSceneList;
	std::vector< CDrawable* >	m_UIElementList;
};

#endif // GUARD_AIDEN_STOREY_SCENE_H_201409120959