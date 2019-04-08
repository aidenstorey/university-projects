//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: scene_physics.cpp
// Description	: Implementation for CScenePhysics
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// Library Includes
#include <D3DX10.h>
#include <CommCtrl.h>

// Local Includes
#include "engine.h"
#include "math_functions.h"
#include "resource.h"
#include "scene_id_list.h"
#include "viewport.h"

// This Include
#include "scene_physics.h"

// Implementation
BOOL CALLBACK DlgProc( HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam )
{
	// Initialise cloth defintion variables
	static int iWidthPos = 4;
	static int iLengthPos = 4;
	static int iRodPos = 3;

#ifdef _DEBUG
	iWidthPos = 0;
	iLengthPos = 0;
#endif

	const int MaxWidth = 3;
	const int MaxLength = 3;
	const int MaxRodCount = 3;

	const int ClothWidth[ MaxWidth ] = { 21, 31, 41 };
	const int ClothLength[ MaxLength ] = { 10, 20, 30 };
	const int ClothRodCount[ MaxRodCount ] = { 2, 5, 10 };

	switch( _uiMsg )
	{
		case WM_INITDIALOG:
		{
			// Initialise width slider
			SendMessage( GetDlgItem( _hWnd, SLD_WIDTH ), TBM_SETRANGE, ( WPARAM )FALSE, ( LPARAM )MAKELONG( 0, MaxWidth - 1 ) );
			SendMessage( GetDlgItem( _hWnd, SLD_WIDTH ), TBM_SETPOS, ( WPARAM )TRUE, ( LPARAM )iWidthPos );

			// Initialise length slider
			SendMessage( GetDlgItem( _hWnd, SLD_LENGTH ), TBM_SETRANGE, ( WPARAM )FALSE, ( LPARAM )MAKELONG( 0, MaxLength - 1 ) );
			SendMessage( GetDlgItem( _hWnd, SLD_LENGTH ), TBM_SETPOS, ( WPARAM )TRUE, ( LPARAM )iLengthPos );

			// Initialise rod slider
			SendMessage( GetDlgItem( _hWnd, SLD_RODS ), TBM_SETRANGE, ( WPARAM )FALSE, ( LPARAM )MAKELONG( 0, MaxRodCount ) );
			SendMessage( GetDlgItem( _hWnd, SLD_RODS ), TBM_SETPOS, ( WPARAM )TRUE, ( LPARAM )iRodPos );

			// Initialise speed slider
			SendMessage( GetDlgItem( _hWnd, SLD_WINDSPEED ), TBM_SETRANGE, ( WPARAM )FALSE, ( LPARAM )MAKELONG( 0, 100 ) );
			SendMessage( GetDlgItem( _hWnd, SLD_WINDSPEED ), TBM_SETPOS, ( WPARAM )TRUE, ( LPARAM )0 );

			// Initialise speed slider
			SendMessage( GetDlgItem( _hWnd, SLD_WINDCONESIZE ), TBM_SETRANGE, ( WPARAM )FALSE, ( LPARAM )MAKELONG( 0, 100 ) );
			SendMessage( GetDlgItem( _hWnd, SLD_WINDCONESIZE ), TBM_SETPOS, ( WPARAM )TRUE, ( LPARAM )0 );

			// Initialse radio buttons
			CheckRadioButton( _hWnd, RAD_GRAB, RAD_BURN, RAD_GRAB );
			CheckRadioButton( _hWnd, RAD_NONE, RAD_PYRAMID, RAD_NONE );

			// Initialise check buttons
			CheckDlgButton( _hWnd, CHK_TEARING, FALSE );
			CheckDlgButton( _hWnd, CHK_SLIDING, FALSE );
			CheckDlgButton( _hWnd, CHK_WIND, FALSE );
			CheckDlgButton( _hWnd, CHK_FIXED, FALSE );
			CheckDlgButton( _hWnd, CHK_SELFINTERSCTION, FALSE );
		}
		break;

		case WM_COMMAND:
		{
			// Perform appropriate action for button press / radio button + check box change 
			switch( LOWORD( _wParam ) )
			{
				case BTN_RESET:
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->ResetSimulation();
				} break;

				case RAD_GRAB: 
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->GetSimulation()->SetInteractionType( EInteractionType::GRAB );
				} break;

				case RAD_CUT: 
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->GetSimulation()->SetInteractionType( EInteractionType::CUT );
				} break;

				case RAD_BURN: 
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->GetSimulation()->SetInteractionType( EInteractionType::BURN ); 
				} break;

				case RAD_NONE: 
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->SetActiveCollider( EColliderType::COLLIDER_NONE ); 
				} break;

				case RAD_SPHERE: 
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->SetActiveCollider( EColliderType::COLLIDER_SPHERE ); 
				} break;

				case RAD_CAPSULE: 
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->SetActiveCollider( EColliderType::COLLIDER_CAPSULE ); 
				} break;

				case RAD_PYRAMID: 
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->SetActiveCollider( EColliderType::COLLIDER_PYRAMID ); 
				} break;

				case CHK_TEARING:
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->GetSimulation()->SetTearingEnabled( SendDlgItemMessage( _hWnd, CHK_TEARING, BM_GETCHECK, NULL, NULL ) == BST_CHECKED );
				}break;

				case CHK_SLIDING:
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->GetSimulation()->SetSlidingEnabled( SendDlgItemMessage( _hWnd, CHK_SLIDING, BM_GETCHECK, NULL, NULL ) == BST_CHECKED );
				}break;

				case CHK_WIND:
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->GetSimulation()->SetWindEnabled( SendDlgItemMessage( _hWnd, CHK_WIND, BM_GETCHECK, NULL, NULL ) == BST_CHECKED );
				}break;

				case CHK_FIXED:
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->SetFixedWindPosition( SendDlgItemMessage( _hWnd, CHK_FIXED, BM_GETCHECK, NULL, NULL ) == BST_CHECKED );
				}break;

				case CHK_SELFINTERSCTION:
				{
					CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );
					pScene->GetSimulation()->SetSelfIntersectingEnabled( SendDlgItemMessage( _hWnd, CHK_SELFINTERSCTION, BM_GETCHECK, NULL, NULL ) == BST_CHECKED );
				}break;

				case IDCANCEL:
				{
					ShowWindow( _hWnd, SW_HIDE );
				} break;

				default: break;
			}
		}
		break;

		case WM_HSCROLL:
		{
			CScenePhysics* pScene = reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) );

			// Get current position of slider
			iWidthPos = SendMessage( GetDlgItem( _hWnd, SLD_WIDTH ), TBM_GETPOS, 0, 0 );
			iLengthPos = SendMessage( GetDlgItem( _hWnd, SLD_LENGTH ), TBM_GETPOS, 0, 0 );
			iRodPos = SendMessage( GetDlgItem( _hWnd, SLD_RODS ), TBM_GETPOS, 0, 0 );

			// Set the cloth variables
			pScene->SetClothVariables( ClothWidth[ iWidthPos ], ClothLength[ iLengthPos ], iRodPos == MaxRodCount ? ClothWidth[ iWidthPos ] : ClothRodCount[ iRodPos ] );

			// Set wind variables
			pScene->GetSimulation()->SetWindSpeed( static_cast< float >( SendMessage( GetDlgItem( _hWnd, SLD_WINDSPEED ), TBM_GETPOS, 0, 0 ) ) );
			pScene->GetSimulation()->SetWindConeSize( static_cast< float >( SendMessage( GetDlgItem( _hWnd, SLD_WINDCONESIZE ), TBM_GETPOS, 0, 0 ) ) );
			
		}
		break;
	}

	return ( 0 );
}

//**************************************************
//
// CScenePhysics: Initialises scene
//
// @author: Aiden Storey
//
// @param[in]	_kiSceneID			ID of scene
//
//**************************************************
CScenePhysics::CScenePhysics( int _kiSceneID ) :
	CScene( _kiSceneID ),
	m_ColliderSphere( EColliderType::COLLIDER_SPHERE ),
	m_ColliderCapsule( EColliderType::COLLIDER_CAPSULE ),
	m_ColliderPyramid( EColliderType::COLLIDER_PYRAMID )
{
	this->m_bIsInitialised = true;

	// Add view port to scene
	CViewport* pMainViewport = this->AddViewport();

	// If no viewport added set initialise to false
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
		this->SetCamera( m_pMainCamera = dynamic_cast< CCamera* >( pMainViewport->CreateCamera( 40, CAMERAMODE_PERSPECTIVE, 1, 200, 10, iCam1ID ) ) );
		this->m_pMainCamera->SetPosition( D3DXVECTOR3( 0.0f, 0.0f, -10.0f ) );
		this->m_pMainCamera->SetTarget( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

		// Create cloth dialog box
		this->m_hWndCloth = CreateDialog( CEngine::GetInstance().GetHInstance(), MAKEINTRESOURCE( DLG_CLOTH ), CEngine::GetInstance().GetHWnd(), DlgProc );
		ShowWindow( this->m_hWndCloth, SW_SHOW );

#ifdef _DEBUG
		this->m_iClothWidth = 21;
		this->m_iClothLength = 10;
		this->m_iClothRodCount = 21;
#endif

		// Create cloth
		this->ResetSimulation();

		// Set colliders to inactive
		this->m_eActiveCollider = EColliderType::COLLIDER_NONE;
		this->SetActiveCollider( this->m_eActiveCollider );

		// Attatch collider objects
		this->AttachChild( &this->m_ColliderSphere );
		this->AttachChild( &this->m_ColliderCapsule );
		this->AttachChild( &this->m_ColliderPyramid );

		// Attach plane to represent the floor
		CMesh* pMesh = CEngine::GetInstance().GetObject3DManager().CreateMesh( L"Assets/Objects/plane.obj", 0.0f, -3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100.0f, 1.0f, 100.0f );
		pMesh->SetTexture( L"floor.jpg" );
		this->AttachChild( pMesh );

		// Set up UI elements
		this->m_pHelp = CEngine::GetInstance().GetObject2DManager().CreateButtonAsRectangle( L"Help", L"help.png", 36.0f, 23.0f, 376.5f, 283.0f );
		this->m_pHelp->SetClickFunction( []{ reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) )->ToggleHelpVisible(); } );
		this->AttachUIElement( m_pHelp );

		this->m_pExit = CEngine::GetInstance().GetObject2DManager().CreateButtonAsRectangle( L"Exit", L"exit.png", 20.0f, 20.0f, 200.5f, 87.5f );
		this->m_pExit->SetClickFunction( []{ reinterpret_cast< CScenePhysics* >( CEngine::GetInstance().GetSceneManager().GetScene( SCENE_PHYSICS ) )->SetHelpVisible( false ); } );
		this->AttachUIElement( m_pExit );

		this->m_pInstructions = CEngine::GetInstance().GetObject2DManager().CreateObjectAsRectangle( L"Instructions", L"instructions.png", 400.0f, 175.0f );
		this->AttachUIElement( m_pInstructions );

		this->SetHelpVisible( this->m_bInstructionsVisible );
	}
}

//**************************************************
//
// ~CScenePhysics: Cleans up scene
//
// @author: Aiden Storey
//
//**************************************************
CScenePhysics::~CScenePhysics()
{
	delete ( this->m_pSimulation );
}

//**************************************************
//
// GetSimulation: Returns current cloth simulation
//
// @author: Aiden Storey
//
// @returns		current cloth simulation
//
//**************************************************
CClothSimulation* CScenePhysics::GetSimulation()
{
	return ( this->m_pSimulation );
}

//**************************************************
//
// SetActiveCollider: Sets active collider 
//
// @author: Aiden Storey
//
// @param[in]	_eType			type of collider to activate
//
//**************************************************
void CScenePhysics::SetActiveCollider( const EColliderType& _eType )
{
	// Set active collider type to be passed value
	this->m_eActiveCollider = _eType;

	// Set colliders active state absed on passed value
	this->m_ColliderSphere.SetActive( _eType == EColliderType::COLLIDER_SPHERE );
	this->m_ColliderCapsule.SetActive( _eType == EColliderType::COLLIDER_CAPSULE );
	this->m_ColliderPyramid.SetActive( _eType == EColliderType::COLLIDER_PYRAMID );

	// Pass active collider to cloth simulation
	switch( this->m_eActiveCollider )
	{
		case EColliderType::COLLIDER_SPHERE: { this->m_pSimulation->SetActiveCollider( &this->m_ColliderSphere ); } break;
		case EColliderType::COLLIDER_CAPSULE: { this->m_pSimulation->SetActiveCollider( &this->m_ColliderCapsule ); } break;
		case EColliderType::COLLIDER_PYRAMID: { this->m_pSimulation->SetActiveCollider( &this->m_ColliderPyramid ); } break;

		default: { this->m_pSimulation->SetActiveCollider( nullptr ); } break;
	}
}

//**************************************************
//
// SetClothVariables: Sets variables for cloth
//
// @author: Aiden Storey
//
// @param[in]	_kiWidth			width for cloth
// @param[in]	_kiLength			length for cloth
// @param[in]	_kiRodCount			rod count for cloth
//
//**************************************************
void CScenePhysics::SetClothVariables( const int& _kiWidth, const int& _kiLength, const int& _kiRodCount )
{
	this->m_iClothWidth = _kiWidth;
	this->m_iClothLength = _kiLength;
	this->m_iClothRodCount = _kiRodCount;
}

//**************************************************
//
// SetFixedWindPosition: Sets variables for cloth
//
// @author: Aiden Storey
//
// @param[in]	_kbFixed			wind position fixed
//
//**************************************************
void CScenePhysics::SetFixedWindPosition( const bool& _kbFixed )
{
	this->m_bFixedWindPosition = _kbFixed;
}

//**************************************************
//
// OnUpdate: Updates the scene and all its elements
//
// @author: Aiden Storey
//
// @param[in]	_kfTimeDelta		Time since last update
//
// @returns		true if update successful false if not or exit requried
//
//**************************************************
bool CScenePhysics::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };

	// If R key is pressed
	if( CEngine::GetInstance().GetInputManager().IsPressed( 'R' ) )
	{
		// Reset the simulation and show cloth setting dialog
		this->ResetSimulation();
		ShowWindow( this->m_hWndCloth, SW_SHOW );
	}

	// If T key is pressed
	if( CEngine::GetInstance().GetInputManager().IsPressed( 'T' ) )
	{
		// Release cloth from pins
		this->m_pSimulation->ReleaseCloth();
	}

	// Move camera based on input
	this->HandleCameraMovement( _kfTimeDelta );

	// If escape key pressed
	if( CEngine::GetInstance().GetInputManager().IsPressed( VK_ESCAPE ) )
	{
		// Return false to signal program close
		bReturn = false;
	}

	return ( bReturn );
}

//**************************************************
//
// HandleCameraMovement: Moves camera based on user input
//
// @author: Aiden Storey
//
// @param[in]	_kfTimeDelta		Time since last update
//
//**************************************************
void CScenePhysics::HandleCameraMovement( const float _kfTimeDelta )
{
	// Initialise rotation + height value
	float fRotationY{ 0.0f };
	float fHeight{ 0.0f };

	// If W or S move up or down
	if( CEngine::GetInstance().GetInputManager().IsDown( 'W' ) )
	{
		fHeight += 5.0f * _kfTimeDelta;
	}

	if( CEngine::GetInstance().GetInputManager().IsDown( 'S' ) )
	{
		fHeight -= 5.0f * _kfTimeDelta;
	}

	// If A or D rotate left or right
	if( CEngine::GetInstance().GetInputManager().IsDown( 'D' ) )
	{
		fRotationY -= 2.0f * _kfTimeDelta;
	}

	if( CEngine::GetInstance().GetInputManager().IsDown( 'A' ) )
	{
		fRotationY += 2.0f * _kfTimeDelta;
	}

	// Calculate rotation matrix
	D3DXMATRIX matRotY;
	D3DXMatrixRotationY( &matRotY, fRotationY );
	
	// Calculate the rotated calera position
	D3DXVECTOR4 vRotated;
	D3DXVec3Transform( &vRotated, &this->m_pMainCamera->GetPosition(), &matRotY );
	
	// Set the camera position to be rotated + the height
	this->m_pMainCamera->SetPosition( D3DXVECTOR3( vRotated.x, vRotated.y + fHeight, vRotated.z ) );

	// If wind position isn't fixed
	if( !this->m_bFixedWindPosition )
	{
		// Give the cloth simulation the wind direction and origin
		this->m_pSimulation->SetWindDirection( this->m_pMainCamera->GetTarget() - this->m_pMainCamera->GetPosition() );
		this->m_pSimulation->SetWindOrigin( this->m_pMainCamera->GetPosition() );
	}
}

//**************************************************
//
// ResetSimulation: Creates a new simulation based on current member variables
//
// @author: Aiden Storey
//
//**************************************************
void CScenePhysics::ResetSimulation()
{
	// If there is a current simulation
	if( this->m_pSimulation != nullptr )
	{
		// Detach and delete it
		this->DetachChild( this->m_pSimulation );
		delete ( this->m_pSimulation );
		this->m_pSimulation = nullptr;
	}

	// Create new cloth with current definition
	m_pSimulation = new CClothSimulation( this->m_iClothLength, this->m_iClothWidth, this->m_iClothRodCount );
	this->AttachChild( m_pSimulation );

	// Sets the active collider of the simulation
	this->SetActiveCollider( this->m_eActiveCollider );

	// Ensure wind continues to update
	this->m_bFixedWindPosition = false;
	
	// Reset dialog box defaults
	CheckRadioButton( this->m_hWndCloth, RAD_GRAB, RAD_BURN, RAD_GRAB );
	CheckDlgButton( this->m_hWndCloth, CHK_TEARING, FALSE );
	CheckDlgButton( this->m_hWndCloth, CHK_SLIDING, FALSE );
	CheckDlgButton( this->m_hWndCloth, CHK_WIND, FALSE );
	CheckDlgButton( this->m_hWndCloth, CHK_FIXED, FALSE );
	CheckDlgButton( this->m_hWndCloth, CHK_SELFINTERSCTION, FALSE );
	SendMessage( GetDlgItem( this->m_hWndCloth, SLD_WINDSPEED ), TBM_SETPOS, ( WPARAM )TRUE, ( LPARAM )0 );
	SendMessage( GetDlgItem( this->m_hWndCloth, SLD_WINDCONESIZE ), TBM_SETPOS, ( WPARAM )TRUE, ( LPARAM )0 );
}

//**************************************************
//
// ToggleHelpVisible: Sets help UI to be opposite of current state
//
// @author: Aiden Storey
//
//**************************************************
void CScenePhysics::ToggleHelpVisible()
{
	this->SetHelpVisible( !this->m_bInstructionsVisible );
}

//**************************************************
//
// SetHelpVisible: Sets the help UI to be visible based on parameter
//
// @author: Aiden Storey
//
// @param[in]	_kbVisible			UI visible
//
//**************************************************
void CScenePhysics::SetHelpVisible( const bool& _kbVisible )
{
	this->m_bInstructionsVisible = _kbVisible;

	this->m_pExit->SetVisible( this->m_bInstructionsVisible );
	this->m_pInstructions->SetVisible( this->m_bInstructionsVisible );
}