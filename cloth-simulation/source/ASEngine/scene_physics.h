//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: scene_physics.h
// Description	: Declaration for CScenePhysics
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

#ifndef GUARD_AIDEN_STOREY_SCENE_PHYSICS_H_201410101436
#define GUARD_AIDEN_STOREY_SCENE_PHYSICS_H_201410101436

// Local includes
#include "cloth_simulation.h"
#include "collider.h"
#include "scene.h"

// Prototypes
class CScenePhysics : public CScene
{
	// Member Functions
public:
	// Constructors
	CScenePhysics( int _kiSceneID );
	~CScenePhysics();

	// Accessors
	CClothSimulation* GetSimulation();

	void SetActiveCollider( const EColliderType& _eType );
	void SetClothVariables( const int& _kiWidth, const int& _kiLength, const int& _kiRodCount );
	void SetFixedWindPosition( const bool& _kbFixed );
	
	// Other
	void ResetSimulation();

	void ToggleHelpVisible();
	void SetHelpVisible( const bool& _kbVisible );

protected:
	// Other
	bool OnUpdate( const float& _kfTimeDelta ) override;

	void HandleCameraMovement( const float _kfTimeDelta );
	void CheckCollisions();

private:
	// Other 
	void CreateUI();

	// Member Variables
public:
protected:
private:
	CCamera*				m_pMainCamera{ nullptr };

	HWND					m_hWndCloth;
	CClothSimulation*		m_pSimulation;

	int						m_iClothWidth{ 41 };
	int						m_iClothLength{ 30 };
	int						m_iClothRodCount{ 41 };

	EColliderType			m_eActiveCollider;

	CCollider				m_ColliderSphere;
	CCollider				m_ColliderCapsule;
	CCollider				m_ColliderPyramid;

	bool					m_bFixedWindPosition{ false };

	bool					m_bInstructionsVisible{ false };
	CButton*				m_pHelp{ nullptr };
	CButton*				m_pExit{ nullptr };
	CTextured2D*			m_pInstructions{ nullptr };
};

#endif // GUARD_AIDEN_STOREY_SCENE_PHYSICS_H_201410101436