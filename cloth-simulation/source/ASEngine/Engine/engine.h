//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_ENGINE_H_201409131344
#define GUARD_AIDEN_STOREY_ENGINE_H_201409131344

// Library Includes
#include <D3DX10.h>
#include <string>
#include <Windows.h>

// Local Includes
#include "camera.h"
#include "input_manager.h"
#include "object_2d_manager.h"
#include "object_3d_manager.h"
#include "scene_manager.h"
#include "../scene_id_list.h"
#include "shader_manager.h"
#include "shape_renderer.h"
#include "timer.h"
#include "text_manager.h"
#include "texture_manager.h"


// Prototypes
class CEngine
{
	// Member Functions
public:
	// Constructors
	~CEngine();

	// Accessors
	HWND GetHWnd();
	HINSTANCE GetHInstance();

	ID3D10Device& GetDevice();
	
	const int& GetClientWidth() const;
	const int& GetClientHeight() const;

	CTimer&	GetTimer();

	void SetRenderingCamera( CCamera* _pCamera );
	CCamera* GetRenderingCamera();

	void SetMaxFPS( const DWORD _ktFPS );

	CInputManager& GetInputManager();
	CObject2DManager& GetObject2DManager();
	CObject3DManager& GetObject3DManager();
	CSceneManager& GetSceneManager();
	CShaderManager& GetShaderManager();
	CShapeRenderer& GetShapeRenderer();
	CTextManager& GetTextManager();
	CTextureManager& GetTextureManager();
	

	// Other
	bool Initialise( HINSTANCE _hInstance, const wchar_t* _kcProgramTitle, const wchar_t* _kcWndClassName, 
					 const std::wstring _kstrTexturePath = L"", const std::wstring _kstrShaderPath = L"" );
	bool Update();

	void BeginRender();
	void EndRender();

	void OnResize();

	// Singleton
	static CEngine& GetInstance()
	{
		static CEngine Instance;
		return( Instance );
	}

protected:
	// Constructors
	CEngine();
	CEngine( CEngine const& );
	const CEngine& operator= ( const CEngine& );

	// Other
	bool InitialiseWindow( const wchar_t* _kcProgramTitle, const wchar_t* _kcWndClassName  );
	bool InitialiseDirect3D();

private:

	// Member Variables
public:
protected:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;

	ID3D10Device*				m_pDevice{ nullptr };
	ID3D10BlendState*			m_pBlendState{ nullptr };

	int							m_iClientWidth{ 800 };
	int							m_iClientHeight{ 600 };

	D3D10_VIEWPORT				m_Viewport;

	IDXGISwapChain*				m_pSwapChain{ nullptr };
	ID3D10Texture2D*			m_pDepthStencilBuffer{ nullptr };
	ID3D10RenderTargetView*		m_pRenderTargetView{ nullptr };
	ID3D10DepthStencilView*		m_pDepthStencilView{ nullptr };

	D3D10_DRIVER_TYPE			m_DriverType{ D3D10_DRIVER_TYPE_HARDWARE };
	D3DXCOLOR					m_ClearColor{ 0.854902f, 0.854902f, 0.854902f, 1.0f };

	bool						m_bRunning;
	bool						m_bInitialised;

	DWORD						m_tPreviousFrameEnd;
	DWORD						m_tFrameTime;
	int							m_iMaxFPS;

	CTimer						m_Timer;

	CCamera*					m_pRenderingCamera{ nullptr };

	CInputManager				m_InputManager;
	CObject2DManager			m_Object2DManager;
	CObject3DManager			m_Object3DManager;
	CSceneManager				m_SceneManager;
	CShaderManager				m_ShaderManager;
	CShapeRenderer				m_ShapeRenderer;
	CTextManager				m_TextManager;
	CTextureManager				m_TextureManager;


private:
};

#endif // GUARD_AIDEN_STOREY_ENGINE_H_201409131344