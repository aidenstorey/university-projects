//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_INPUT_MANAGER_H_201409182108
#define GUARD_AIDEN_STOREY_INPUT_MANAGER_H_201409182108

// Library Includes
#include <D3DX10.h>
#include <Windows.h>

// Constants
const int g_NumKeys = 256;

// Prototypes
class CInputManager
{
	// Member Functions
public:
	// Constructors
	CInputManager();
	~CInputManager();

	// Accessors
	void SetKeyState( const unsigned _kuKey, const bool _bPressed );

	bool IsDown( const char _cKey );
	bool IsPressed( const char _cKey );
	bool IsReleased( const char _cKey );

	D3DXVECTOR2 MouseScreenPosition2D();
	D3DXVECTOR3 MouseScreenPosition3D();

	D3DXVECTOR2 MouseClientPosition2D();
	D3DXVECTOR3 MouseClientPosition3D();

	D3DXVECTOR2 MouseWorldPosition2D();
	D3DXVECTOR3 MouseWorldPosition3D();

	D3DXVECTOR2 MouseUIPosition2D();
	D3DXVECTOR3 MouseUIPosition3D();

	D3DXVECTOR2 MouseClampedWorldPosition2D();
	D3DXVECTOR3 MouseClampedWorldPosition3D();

	// Other
	bool Initialise( HWND _hWnd );
	void Update();

protected:
private:

	// Member Variablies
public:
protected:
private:
	HWND		m_hWnd;
	bool		m_bPrevKeyState[ g_NumKeys ];
	bool		m_bCurrKeyState[ g_NumKeys ];
};

#endif // GUARD_AIDEN_STOREY_INPUT_MANAGER_H_201409182108