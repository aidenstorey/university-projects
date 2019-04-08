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
#include "scene.h"

// Implementation
CScene::CScene( const int _kiSceneID ) :
	m_iSceneID( _kiSceneID ),
	m_bIsInitialised( false ),
	m_bPaused( false )
{
	// Initialise viewport
	unsigned int iScreenWidth = static_cast< unsigned int >( CEngine::GetInstance().GetClientWidth() );
	unsigned int iScreenHeight = static_cast< unsigned int >( CEngine::GetInstance().GetClientHeight() );
	this->m_UIViewport.Initialise( 0, 0, iScreenWidth, iScreenHeight, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );
	
	// Crete camera
	unsigned int iCam1ID = 0;
	this->SetCamera( m_pUICamera = dynamic_cast< CCamera* >( m_UIViewport.CreateCamera( 10, CAMERAMODE_ORTHOGONAL, 1, 200, 1, iCam1ID ) ) );
	this->m_pUICamera->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, -10.0f ) );
}

CScene::~CScene()
{}

bool CScene::IsInitialised()
{
	return ( this->m_bIsInitialised );
}

int CScene::GetSceneID()
{
	return ( this->m_iSceneID );
}

CCamera& CScene::GetCamera()
{
	return ( *this->m_pActiveCamera );
}

void CScene::SetCamera( CCamera* _pCamera )
{
	this->m_pActiveCamera = _pCamera;
}

CCamera& CScene::GetUICamera()
{
	return ( *this->m_pUICamera );
}

bool CScene::ProcessScene( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity( &matIdentity );

	bReturn &= this->Process( _kfTimeDelta, nullptr, matIdentity );

	return ( bReturn );
}

bool CScene::OnActivate()
{
	bool bReturn{ true };
	return ( bReturn );
}

bool CScene::OnDeactivate()
{
	bool bReturn{ true };
	return ( bReturn );
}

void CScene::AttachChildScene( CScene* _pScene )
{
	this->m_ChildSceneList.push_back( _pScene );
}

bool CScene::AttachUIElement( CDrawable* _pElement )
{
	bool bReturn{ false };
	
	if( _pElement != nullptr )
	{
		bReturn = true;
		
		try
		{
			this->m_UIElementList.push_back( _pElement );
			_pElement->m_pParent = this;
		}
		catch( ... )
		{
			bReturn = false;
		}
	}

	return ( bReturn );
}

void CScene::DetachUIElement( CDrawable* _pElement )
{
	std::vector< CDrawable* >::iterator iterDelete = std::find( this->m_UIElementList.begin(), this->m_UIElementList.end(), _pElement );
	if( iterDelete != this->m_UIElementList.end() )
	{
		_pElement->m_pParent = nullptr;
		this->m_UIElementList.erase( iterDelete );
	}
}

CViewport* CScene::AddViewport()
{
	CViewport* pReturn{ nullptr };

	try
	{
		this->m_ViewportList.push_back( CViewport() );
		pReturn = &this->m_ViewportList.back();
	}
	catch ( ... )
	{}

	return ( pReturn );
}

bool CScene::PostUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	for( auto& aScene : m_ChildSceneList )
	{
		bReturn &= aScene->Update( _kfTimeDelta );
	}

	return ( bReturn );
}

bool CScene::Update( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	bReturn &= this->PreUpdate( _kfTimeDelta );

	for( auto& aChild : this->m_ChildList )
	{
		bReturn &= aChild->Update( _kfTimeDelta );
	}

	for( auto& aElement : this->m_UIElementList )
	{
		bReturn &= aElement->Update( _kfTimeDelta );
	}

	bReturn &= this->OnUpdate( _kfTimeDelta );
	bReturn &= this->PostUpdate( _kfTimeDelta );

	return ( bReturn );
}

void CScene::Draw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{
	D3DXMATRIX matOffset = this->m_matWorld * _krOffset;
	for( auto& aViewport : m_ViewportList )
	{
		CEngine::GetInstance().GetDevice().RSSetViewports( 1, aViewport.GetD3DViewport() );
		CEngine::GetInstance().SetRenderingCamera( aViewport.GetActiveCamera() );
		
		CDrawable::Draw( CEngine::GetInstance().GetRenderingCamera(), matOffset );
	}

	CEngine::GetInstance().GetDevice().RSSetViewports( 1, this->m_UIViewport.GetD3DViewport() );
	CEngine::GetInstance().SetRenderingCamera( this->m_UIViewport.GetActiveCamera() );

	for( auto& aElement : this->m_UIElementList )
	{
		aElement->Draw( CEngine::GetInstance().GetRenderingCamera(), matOffset );
	}

	for( auto& aScene : m_ChildSceneList )
	{
		aScene->Draw( _pRenderCamera, matOffset );
	}
}