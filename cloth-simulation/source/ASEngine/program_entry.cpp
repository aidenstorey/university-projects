//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: program_entry.cpp
// Description	: Implementation for CProgramEntry
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//


// Library Includes
#include <ctime>

// Local Includes
#include "engine.h"
#include "scene_id_list.h"
#include "scene_physics.h"

// This Include 
#include "program_entry.h"

// Implementation
int WINAPI WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _cmdLine, int _iShowCmd )
{
	srand( static_cast< unsigned int >( time( NULL ) ) );

	CProgramEntry Program;

	if ( Program.Initialise( _hInstance ) )
	{
		Program.Run();
	}

	return ( 0 );
}

//**************************************************
//
// CProgramEntry: Initialises program entry
//
// @author: Aiden Storey
//
//**************************************************
CProgramEntry::CProgramEntry()
{}

//**************************************************
//
// ~CProgramEntry: Cleans up program entry
//
// @author: Aiden Storey
//
//**************************************************
CProgramEntry::~CProgramEntry()
{}

//**************************************************
//
// Initialise: Initialses engine 
//
// @author: Aiden Storey
//
// @param[in]	_hInstance			instance for window
//
// @returns		if initialisation was successful
//
//**************************************************
bool CProgramEntry::Initialise( HINSTANCE _hInstance )
{
	bool bReturn{ true };

	bReturn &= CEngine::GetInstance().Initialise( _hInstance, L"GD2P02 - Summative 2", L"Engine", L"Assets/Textures/", L"Assets/Shaders/" );

	return ( bReturn );
}

//**************************************************
//
// Run: description
//
// @author: Aiden Storey

// @returns		completion code
//
//**************************************************
int CProgramEntry::Run()
{
	CEngine::GetInstance().GetSceneManager().CreateScene< CScenePhysics >( SCENE_PHYSICS );
	CEngine::GetInstance().GetSceneManager().SetCurrentScene( SCENE_PHYSICS );

	MSG msg{ 0 };
	for ( ;; )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				break;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if ( !CEngine::GetInstance().Update() )
			{
				PostQuitMessage( 0 );
			}
		}
	}

	return ( static_cast< int >( msg.wParam ) );
}