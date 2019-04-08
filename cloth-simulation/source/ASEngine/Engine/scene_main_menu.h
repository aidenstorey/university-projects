//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: scene_main_menu.h
// Description	: Prototype for CSceneMainMenu
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

#ifndef GUARD_AIDEN_STOREY_SCENE_MAIN_MENU_H_201409282205
#define GUARD_AIDEN_STOREY_SCENE_MAIN_MENU_H_201409282205

// Library Includes
#include <vector>

// Local includes
#include "button_rectangle.h"
#include "scene.h"
#include "text.h"

// Prototypes
class CSceneMainMenu : public CScene
{
	// Member Functions
public:
	// Constructors
	CSceneMainMenu( int _kiSceneID );
	~CSceneMainMenu();

protected:
	// Other
	bool OnUpdate( const float& _kfTimeDelta ) override;

private:

	// Member Variables
public:
protected:
private:
	CCamera*			m_pMainCamera{ nullptr };
	CButtonRectangle*	m_PlayButton;
	CButtonRectangle*	m_LevelEditorButton;
	CText*				m_pTitle;
	CText*				m_pInstructions;
};

#endif // GUARD_AIDEN_STOREY_SCENE_MAIN_MENU_H_201409282205