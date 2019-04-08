//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SCENE_MANAGER_H_20109191003
#define GUARD_AIDEN_STOREY_SCENE_MANAGER_H_20109191003

// Library Includes
#include <vector>

// Local Includes
#include "scene.h"

// Prototypes
class CSceneManager
{
	// Member Functions
public:
	// Constructors
	CSceneManager();
	~CSceneManager();
	
	// Accessors
	CScene* GetScene( const int _kiSceneID );

	bool SetCurrentScene( const int _kiSceneID );
	CScene* GetCurrentScene();
	
	// Other
	bool Update( const float& _kfTimeDelta );

	template< typename T > bool CreateScene( const int _kiSceneID );
	template< typename T > bool CreateScene( const int _kiSceneID, const int _kiParentSceneID );

	bool ConnectChildScene( const int _kiChildSceneID, const int _kiParentSceneID );
	
protected:
	template< typename T > bool CreateScene( const int _kiSceneID, const int _kiParentSceneID, const bool _kbParented );	

private:

	// Member Variables
public:
protected:
private:
	std::vector< CScene* >	m_SceneList; 
	CScene*					m_pCurrentScene{ nullptr };
};

// Templated function definitions
#include "scene_manager.hpp"

#endif // GUARD_AIDEN_STOREY_SCENE_MANAGER_H_20109191003