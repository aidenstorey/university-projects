//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "engine.h"
#include "../scene_id_list.h"
#include "viewport.h"

// This Include
#include "scene_sample.h"

// Implementation
CSceneSample::CSceneSample( int _kiSceneID ) :
	CScene( _kiSceneID )
{
	this->m_bIsInitialised = true;

	CViewport* pMainViewport = this->AddViewport();
	if( pMainViewport == nullptr )
	{
		this->m_bIsInitialised = false;
	}
	else
	{
		// Initialise viewport
		unsigned int iScreenWidth = static_cast< unsigned int >( CEngine::GetInstance().GetClientWidth() );
		unsigned int iScreenHeight = static_cast< unsigned int >( CEngine::GetInstance().GetClientHeight() );
		pMainViewport->Initialise( 0, 0, iScreenWidth, iScreenHeight, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );

		// Crete camera
		unsigned int iCam1ID = 0;
		this->SetCamera( m_pMainCamera = dynamic_cast< CCamera* >( pMainViewport->CreateCamera( 10, CAMERAMODE_ORTHOGONAL, 1, 200, 10, iCam1ID ) ) );
		this->m_pMainCamera->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, -10.0f ) );
	}
}

CSceneSample::~CSceneSample()
{}

bool CSceneSample::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };
	return ( bReturn );
}