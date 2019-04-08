// This Include
#include "keyboard_input.h"

// Implementation
CKeyboardInput::CKeyboardInput()
{
	for( int i = 0; i < g_kiKeyboardMaxKeys; ++i )
	{
		m_bPrevKeys[ i ] = false;
		m_bCurrKeys[ i ] = false;
	}
}

CKeyboardInput::~CKeyboardInput()
{}

bool CKeyboardInput::KeyPressed( const unsigned _uiMsg )
{
	return ( !m_bPrevKeys[ _uiMsg ] && m_bCurrKeys[ _uiMsg ] );
}

bool CKeyboardInput::KeyDown( const unsigned _uiMsg )
{
	return ( m_bCurrKeys[ _uiMsg ] );
}

bool CKeyboardInput::KeyReleased( const unsigned _uiMsg )
{
	return ( m_bPrevKeys[ _uiMsg ] && !m_bCurrKeys[ _uiMsg ] );
}

void CKeyboardInput::UpdateKey( const unsigned _uiMsg, const bool _bKeyPressed )
{
	m_bPrevKeys[ _uiMsg ] = m_bCurrKeys[ _uiMsg ];
	m_bCurrKeys[ _uiMsg ] = _bKeyPressed;
}

void CKeyboardInput::UpdatePrevious()
{
	for( int i = 0; i < g_kiKeyboardMaxKeys; ++i )
	{
		m_bPrevKeys[ i ] = m_bCurrKeys[ i ];
	}
}