//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "safe_release.h"
#include "shader_default_2d.h"
#include "shader_default_3d.h"
#include "shader_id_list.h"
#include "shader_textured_2d.h"
#include "shader_textured_3d.h"

// This Include
#include "engine.h"

// Implementation
LRESULT CALLBACK WindowProc( HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam )
{
	static HWND hWndButton;

	switch ( _uiMsg )
	{
		case WM_CREATE:
		{
			return ( 0 );
		}
		break;

		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return ( 0 );
		}
		break;

		case WM_LBUTTONDOWN:
		{
			CEngine::GetInstance().GetInputManager().SetKeyState( VK_LBUTTON, true );
		}
		break;

		case WM_LBUTTONUP:
		{
			CEngine::GetInstance().GetInputManager().SetKeyState( VK_LBUTTON, false );
		}
		break;

		case WM_RBUTTONDOWN:
		{
			CEngine::GetInstance().GetInputManager().SetKeyState( VK_RBUTTON, true );
		}
		break;

		case WM_RBUTTONUP:
		{
			CEngine::GetInstance().GetInputManager().SetKeyState( VK_RBUTTON, false );
		}
		break;

		case WM_KEYDOWN:
		{
			CEngine::GetInstance().GetInputManager().SetKeyState( _wParam, true );
		}
		break;

		case WM_KEYUP:
		{
			CEngine::GetInstance().GetInputManager().SetKeyState( _wParam, false );
		}
		break;

		default: break;
	}

	
	return( DefWindowProc( _hWnd, _uiMsg, _wParam, _lParam ) );
}

CEngine::CEngine() :
	m_bRunning( false ),
	m_tFrameTime( 0 ), 
	m_tPreviousFrameEnd( 0 ),
	m_iMaxFPS( 0 )
{}

CEngine::~CEngine()
{
	ReleaseCOM( this->m_pRenderTargetView );
	ReleaseCOM( this->m_pDepthStencilView );
	ReleaseCOM( this->m_pSwapChain );
	ReleaseCOM( this->m_pDepthStencilBuffer );
	ReleaseCOM( this->m_pDevice );
}

HWND CEngine::GetHWnd()
{
	return ( this->m_hWnd );
}

HINSTANCE CEngine::GetHInstance()
{
	return ( this->m_hInstance );
}

ID3D10Device& CEngine::GetDevice()
{
	return ( *this->m_pDevice );
}

const int& CEngine::GetClientWidth() const
{
	return ( this->m_iClientWidth );
}

const int& CEngine::GetClientHeight() const
{
	return ( this->m_iClientHeight );
}

CTimer&	CEngine::GetTimer()
{
	return ( this->m_Timer );
}

void CEngine::SetRenderingCamera( CCamera* _pCamera )
{
	this->m_pRenderingCamera = _pCamera;
}

CCamera* CEngine::GetRenderingCamera()
{
	return ( this->m_pRenderingCamera );
}

void CEngine::SetMaxFPS( const DWORD _ktFPS )
{
	this->m_tFrameTime = 1000 / _ktFPS;
	this->m_iMaxFPS = _ktFPS;
}

CInputManager& CEngine::GetInputManager()
{
	return ( this->m_InputManager );
}

CObject2DManager& CEngine::GetObject2DManager()
{
	return ( this->m_Object2DManager );
}

CObject3DManager& CEngine::GetObject3DManager()
{
	return ( this->m_Object3DManager );
}

CSceneManager& CEngine::GetSceneManager()
{
	return ( this->m_SceneManager );
}

CShaderManager& CEngine::GetShaderManager()
{
	return ( this->m_ShaderManager );
}

CShapeRenderer& CEngine::GetShapeRenderer()
{
	return ( this->m_ShapeRenderer );
}

CTextManager& CEngine::GetTextManager()
{
	return ( this->m_TextManager );
}

CTextureManager& CEngine::GetTextureManager()
{
	return ( this->m_TextureManager );
}

bool CEngine::Initialise( HINSTANCE _hInstance, const wchar_t* _kcProgramTitle, const wchar_t* _kcWndClassName,
						  const std::wstring _kstrTexturePath, const std::wstring _kstrShaderPath )
{
	this->m_bRunning = true;

	// Store local copy of parameters
	this->m_hInstance = _hInstance;

	// Initialise window
	this->m_bRunning &= this->InitialiseWindow( _kcProgramTitle, _kcWndClassName );

	if( this->m_bRunning )
	{
		// Set file paths for assets
		this->m_TextureManager.SetTexturePath( _kstrTexturePath );
		this->m_ShaderManager.SetShaderPath( _kstrShaderPath );

		// Initialise Graphics
		this->m_bRunning &= this->InitialiseDirect3D();

		// Initialise managers
		this->m_bRunning &= this->m_InputManager.Initialise( m_hWnd );
	}

	this->m_Timer.Reset();

	return ( this->m_bRunning );
}

bool CEngine::InitialiseWindow( const wchar_t* _kcProgramTitle, const wchar_t* _kcWndClassName )
{
	bool bReturn{ true };

	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= this->m_hInstance;
	wc.hIcon			= LoadIcon( 0, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( 0, IDC_ARROW );
	wc.hbrBackground	= ( HBRUSH )GetStockObject( NULL_BRUSH );
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= _kcWndClassName;

	if ( !RegisterClass( &wc ) )
	{
		PostQuitMessage( 0 );
	}

	RECT r = { 0, 0, this->m_iClientWidth, this->m_iClientHeight };
	AdjustWindowRect( &r, WS_OVERLAPPEDWINDOW, false );
	int width = r.right - r.left;
	int height = r.bottom - r.top;


	this->m_hWnd = CreateWindow( _kcWndClassName, _kcProgramTitle, WS_OVERLAPPED | WS_SYSMENU,
								 CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0,
								 this->m_hInstance, this );

	if ( !this->m_hWnd )
	{
		PostQuitMessage( 0 );
	}

	ShowWindow( this->m_hWnd, SW_SHOW );
	UpdateWindow( this->m_hWnd );

	return ( bReturn );
}

bool CEngine::InitialiseDirect3D()
{
	bool bReturn{ true };

	// Fill swap chain description
	DXGI_SWAP_CHAIN_DESC scDesc; 
	scDesc.BufferDesc.Width						= this->m_iClientWidth;
	scDesc.BufferDesc.Height					= this->m_iClientHeight;
	scDesc.BufferDesc.RefreshRate.Numerator		= 60;
	scDesc.BufferDesc.RefreshRate.Denominator	= 1;
	scDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.SampleDesc.Count						= 1;
	scDesc.SampleDesc.Quality					= 0;
	scDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount							= 1;
	scDesc.OutputWindow							= this->m_hWnd;
	scDesc.Windowed								= true;
	scDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags								= 0;

	// Create the device
	D3D10CreateDeviceAndSwapChain( 0, this->m_DriverType, 0, 0, D3D10_SDK_VERSION, &scDesc, &this->m_pSwapChain, &this->m_pDevice );

	// Disable full screen toggle
	IDXGIFactory *pFactory = NULL;
	if( SUCCEEDED( m_pSwapChain->GetParent( __uuidof( IDXGIFactory ), ( void** )&pFactory ) ) )
	{
		pFactory->MakeWindowAssociation( m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES );
		ReleaseCOM( pFactory );
	}

	// Create transparent blend state
	D3D10_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.BlendEnable[ 0 ] = true;
	blendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[ 0 ] = D3D10_COLOR_WRITE_ENABLE_ALL;

	m_pDevice->CreateBlendState( &blendDesc, &m_pBlendState );

	// Create and set default shader
	bReturn &= this->m_ShaderManager.CreateShader < CShaderDefault2D >( SHADER_DEFAULT2D );
	bReturn &= this->m_ShaderManager.CreateShader < CShaderDefault3D >( SHADER_DEFAULT3D );
	bReturn &= this->m_ShaderManager.CreateShader < CShaderTextured2D >( SHADER_TEXTURED2D );
	bReturn &= this->m_ShaderManager.CreateShader < CShaderTextured3D >( SHADER_TEXTURED3D );

	// Create the default font
	this->m_TextManager.CreateD3DFont( L"DefaultFont" );

	// Initialise shape renderer
	this->m_ShapeRenderer.Initialise();

	this->OnResize();

	return ( bReturn );
}

bool CEngine::Update()
{
	bool bReturn{ true };

	if ( this->m_bRunning )
	{
		this->m_Timer.Tick();

		bReturn &= this->m_SceneManager.Update( this->m_Timer.GetDeltaTime() );

		if ( m_iMaxFPS != 0 )
		{
			DWORD tTimeDelta = timeGetTime() - m_tPreviousFrameEnd;

			if ( tTimeDelta < this->m_tFrameTime )
			{
				Sleep( m_tFrameTime - tTimeDelta );
			}

			this->m_tPreviousFrameEnd = timeGetTime();
		}

	}

	this->m_InputManager.Update();

	return ( bReturn );
}

void CEngine::BeginRender()
{
	this->m_pDevice->ClearRenderTargetView( this->m_pRenderTargetView, this->m_ClearColor );
	this->m_pDevice->ClearDepthStencilView( this->m_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0 );

	this->m_pDevice->OMSetDepthStencilState( 0, 0 );
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	this->m_pDevice->OMSetBlendState( 0, blendFactors, 0xffffffff );

	this->m_pDevice->OMSetRenderTargets( 1, &this->m_pRenderTargetView, this->m_pDepthStencilView );
	this->m_pDevice->RSSetViewports( 1, &this->m_Viewport );
}

void CEngine::EndRender()
{
	this->m_pSwapChain->Present( 0, 0 );
}

void CEngine::OnResize()
{
	if( m_pSwapChain != nullptr )
	{
		// Release old views to repopulate with new data
		ReleaseCOM( this->m_pRenderTargetView );
		ReleaseCOM( this->m_pDepthStencilView );
		ReleaseCOM( this->m_pDepthStencilBuffer );

		// Resize swap chain
		this->m_pSwapChain->ResizeBuffers( 0, 0, 0, DXGI_FORMAT_UNKNOWN, 0 );

		// Recreate render target view
		ID3D10Texture2D* pBackBuffer;
		this->m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), reinterpret_cast< void** >( &pBackBuffer ) );
		this->m_pDevice->CreateRenderTargetView( pBackBuffer, 0, &this->m_pRenderTargetView );

		// Clean up
		ReleaseCOM( pBackBuffer );

		// Create Depth/stencil buffer/view
		D3D10_TEXTURE2D_DESC dsDesc;
		dsDesc.Width = this->m_iClientWidth;
		dsDesc.Height = this->m_iClientHeight;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
		dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsDesc.SampleDesc.Count = 1;
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage = D3D10_USAGE_DEFAULT;
		dsDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;

		this->m_pDevice->CreateTexture2D( &dsDesc, 0, &this->m_pDepthStencilBuffer );
		this->m_pDevice->CreateDepthStencilView( this->m_pDepthStencilBuffer, 0, &this->m_pDepthStencilView );

		// Bind render target view and depth/stencil view to pipeline
		this->m_pDevice->OMSetRenderTargets( 1, &this->m_pRenderTargetView, this->m_pDepthStencilView );

		// Set viewport
		this->m_Viewport.TopLeftX = 0;
		this->m_Viewport.TopLeftY = 0;
		this->m_Viewport.Width = this->m_iClientWidth;
		this->m_Viewport.Height = this->m_iClientHeight;
		this->m_Viewport.MinDepth = 0.0f;
		this->m_Viewport.MaxDepth = 1.0f;

		this->m_pDevice->RSSetViewports( 1, &m_Viewport );
	}
}