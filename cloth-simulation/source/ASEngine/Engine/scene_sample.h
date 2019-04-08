//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SCENE_SAMPLE_MAIN_MENU_H_201410091427
#define GUARD_AIDEN_STOREY_SCENE_SAMPLE_MAIN_MENU_H_201410091427

// Local includes
#include "scene.h"

// Prototypes
class CSceneSample : public CScene
{
	// Member Functions
public:
	// Constructors
	CSceneSample( int _kiSceneID );
	~CSceneSample();

protected:
	// Other
	bool OnUpdate( const float& _kfTimeDelta ) override;

private:

	// Member Variables
public:
protected:
private:
	CCamera*	m_pMainCamera{ nullptr };
};

#endif // GUARD_AIDEN_STOREY_SCENE_MAIN_MENU_H_201410091427