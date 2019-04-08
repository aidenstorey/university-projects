//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "engine.h"

// This Include
#include "scene_manager.h"

// Implementation
CSceneManager::CSceneManager() : 
	m_pCurrentScene( nullptr )
{}

CSceneManager::~CSceneManager()
{
	for ( auto& aScene : m_SceneList )
	{
		delete aScene;
	}
}

CScene* CSceneManager::GetScene( const int _kiSceneID )
{
	CScene* pReturn{ nullptr };

	for( auto& aScene : m_SceneList )
	{
		if ( aScene->GetSceneID() == _kiSceneID )
		{
			pReturn = aScene;
			break;
		}
	}

	return ( pReturn );
}

bool CSceneManager::SetCurrentScene( const int _kiSceneID )
{
	bool bReturn{ false };

	CScene* pCurrentScene = this->GetScene( _kiSceneID );
	if ( pCurrentScene != nullptr )
	{
		bReturn = true;

		if( this->m_pCurrentScene != nullptr )
		{
			bReturn &= this->m_pCurrentScene->OnDeactivate();
		}

		this->m_pCurrentScene = pCurrentScene;

		bReturn &= this->m_pCurrentScene->OnActivate();
	}

	return ( bReturn );
}

CScene* CSceneManager::GetCurrentScene()
{
	return ( this->m_pCurrentScene );
}


bool CSceneManager::Update( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	CEngine::GetInstance().BeginRender();

	if ( this->m_pCurrentScene != nullptr )
	{
		bReturn &= this->m_pCurrentScene->ProcessScene( _kfTimeDelta );
	}

	CEngine::GetInstance().EndRender();

	return ( bReturn );
}

bool CSceneManager::ConnectChildScene( const int _kiChildSceneID, const int _kiParentSceneID )
{
	bool bReturn{ true };

	CScene* pChild = this->GetScene( _kiChildSceneID );
	CScene* pParent = this->GetScene( _kiParentSceneID );

	if ( pChild == nullptr || pParent == nullptr )
	{
		bReturn = true;
	}
	else
	{
		pParent->AttachChildScene( pChild );
	}

	return ( bReturn );
}