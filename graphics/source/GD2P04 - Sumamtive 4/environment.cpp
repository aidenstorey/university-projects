// Library Includes
#include <time.h>

// Local Includes
#include "block_terrain.h"
#include "block_tree.h"
#include "camera.h"
#include "controlled_light.h"
#include "d3dApp.h"
#include "flat_surface.h"
#include "instance_cube.h"
#include "keyboard_input.h"
#include "ndc_quad.h"
#include "particle_system.h"
#include "services.h"
#include "single_block.h"
#include "writable_texture.h"

// Constant Variables
const int g_EnvironmentTextureArrayNum	= 15;
const int g_EnvironmentWaterTexArrayNum	= 12;
const int g_EnvironmentNumTrees			= 5;

// Prototypes
class CEnvApp : public D3DApp
{
	// Member Functions
public:
	// Constructors
	CEnvApp( HINSTANCE _hInstance );
	~CEnvApp();

	// Other
	void initApp();
	void onResize();
	void updateScene( float _fDT, float _fGT );
	void drawScene();

protected:
private:
	void BuildFX();
	void BuildVertexLayouts();

	void PopulateCubeTextureArray();
	void PopulateWaterTextureArray();

	void CreateStencilStates();

	void DrawShadowMap();

	// Member Variables
public:
protected:
private:
	ID3D10Effect*							m_FX;
	ID3D10EffectTechnique*					m_Tech;
	ID3D10EffectTechnique*					m_StandardTech;
	ID3D10InputLayout*						m_IVertexLayout;
	ID3D10InputLayout*						m_SVertexLayout;
	ID3D10InputLayout*						m_QVertexLayout;

	// Shadow map creation
	ID3D10Effect*							m_CShadowFX;
	ID3D10EffectTechnique*					m_CShadowTech;
	ID3D10EffectMatrixVariable*				m_fxShadowLightWVPVar;
	ID3D10EffectShaderResourceVariable*		m_fxShadowDiffuseVar;

	// Shadow map drawign
	ID3D10Effect*							m_DShadowFX;
	ID3D10EffectTechnique*					m_DShadowTech;
	ID3D10EffectShaderResourceVariable*		m_fxDrawShadowMapVar;

	ID3D10EffectShaderResourceVariable*		m_fxShadowMapVar;
	
	ID3D10EffectShaderResourceVariable*		m_fxTextureArrayVar;
	ID3D10ShaderResourceView*				m_fxTextureArrayRV;
	ID3D10ShaderResourceView*				m_fxWaterTextureArrayRV;
	ID3D10EffectMatrixVariable*				m_fxLightWVPVar;

	ID3D10ShaderResourceView*				m_Default;

	ID3D10BlendState*						m_ReflectionBS;
	ID3D10DepthStencilState*				m_SurfaceDSS;
	ID3D10DepthStencilState*				m_ReflectionDSS;
	ID3D10RasterizerState*					m_CullRS;

	ID3D10EffectMatrixVariable*				m_fxWVPVar;
	ID3D10EffectMatrixVariable*				m_fxWorldVar;
	ID3D10EffectVariable*					m_fxEyePosVar;
	ID3D10EffectVariable*					m_fxLightVar;
	ID3D10EffectVariable*					m_fxFogVar;
	ID3D10EffectMatrixVariable*				m_fxTexMtxVar;

	ID3D10BlendState*						m_TransparentBS;

	D3DXMATRIX								m_EnvWorld;
	D3DXMATRIX								m_Proj;
	D3DXMATRIX								m_WVP;

	// Scene objects
	CCamera									m_Camera;
	CKeyboardInput							m_Keyboard;
	CControlledLight						m_Light;
	CParticles								m_Particles;
	CFlatSurface							m_Water;

	CInstanceCube							m_InstanceCube;
	CBlockTerrain							m_BTerrain;
	CBlockTree								m_BTrees[ g_EnvironmentNumTrees ];
	TPoint									m_TreePositions[ g_EnvironmentNumTrees ];
	CSingleBlock							m_ReflectionObject;

	CWritableDepthTexture					m_ShadowMap;
	CNDCQuad								m_NDCQuad;

	bool									m_bCube;
};

// Implementation
int WINAPI WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _cmdLine, int _iShowCmd )
{
	srand( static_cast< UINT >( time( 0 ) ) );

	CEnvApp application( _hInstance );
	application.initApp();

	return application.run();
}

CEnvApp::CEnvApp( HINSTANCE _hInstance ) : 
	D3DApp( _hInstance ), m_FX( 0 ), m_Tech( 0 ), m_IVertexLayout( 0 ), m_SVertexLayout( 0 ),
	m_fxWVPVar( 0 ), m_fxWorldVar( 0 ), m_fxEyePosVar( 0 ), m_fxLightVar( 0 ),
	m_fxTextureArrayVar( 0 ), m_fxTexMtxVar( 0 ), m_bCube( false ), m_CShadowFX( nullptr ),
	m_DShadowFX( nullptr ), m_CShadowTech( nullptr ), m_DShadowTech( nullptr ), m_QVertexLayout( 0 ),
	m_fxShadowMapVar( nullptr )
{
	D3DXMatrixIdentity( &m_EnvWorld );	
	D3DXMatrixIdentity( &m_Proj );
	D3DXMatrixIdentity( &m_WVP );
}

CEnvApp::~CEnvApp()
{
	if( md3dDevice )
	{
		md3dDevice->ClearState();
	}

	ReleaseCOM( m_FX );
	ReleaseCOM( m_IVertexLayout );
	ReleaseCOM( m_SVertexLayout );
	ReleaseCOM( m_QVertexLayout );

	ReleaseCOM( m_CShadowFX );
	ReleaseCOM( m_DShadowFX );
	
	ReleaseCOM( m_fxTextureArrayRV );
	ReleaseCOM( m_TransparentBS );

	ReleaseCOM( m_Default );
	ReleaseCOM( m_ReflectionDSS );
	ReleaseCOM( m_SurfaceDSS );
	ReleaseCOM( m_CullRS );
	ReleaseCOM( m_ReflectionBS );
}

void CEnvApp::initApp()
{
	// Base call
	D3DApp::initApp();

	// Initialise Clear 
	mClearColor = D3DXCOLOR( 0.0, 0.6f, 1.0f, 1.0f );

	// Sub calls
	BuildFX();
	BuildVertexLayouts();

	// Send devices to services
	CServices::ProvideService( SERVICETYPE_KEYBOARD, static_cast< void* >( &m_Keyboard ) );
	CServices::ProvideService( SERVICETYPE_D3DDEVICE, static_cast< void* >( md3dDevice ) );
		
	// Load a default spec map
	HR( D3DX10CreateShaderResourceViewFromFile( md3dDevice, L"Assets/Textures/Surface/default.dds", 0, 0, &m_Default, 0 ) );
	
	// Initialise Camera to look at terrain
	m_Camera.Initialise( D3DXVECTOR3( 100.0f, 50.0f, 100.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	
	// Initialise Light
	m_Light.Initialise();

	// Initialise instanced cube
	m_InstanceCube.Initialise();

	// Initialise Terrain
	m_BTerrain.Initialise( md3dDevice, &m_InstanceCube );

	// Initialise Particles
	m_Particles.Initialise();

	// Initialise water surface
	m_Water.Initialise( md3dDevice, D3DXVECTOR3( 35.5f, 10.0f, 40.5f ), 6 );

	// Initialise object for reflection
	m_ReflectionObject.Initialise( md3dDevice, m_InstanceCube.GetVertexBuffer(), g_VerticesPerCube );

	// Initialise quad
	m_NDCQuad.Initialise( md3dDevice );

	// Initialise Trees
	for( int i = 0; i < g_EnvironmentNumTrees; ++i )
	{
		m_BTrees[ i ].Initialise( &m_InstanceCube );
	}	

	// Initialise ShadowMap
	m_ShadowMap.Initialise( md3dDevice, 2048, 2048 );

	// Set Tree positions
	m_TreePositions[ 0 ] = TPoint( -50, 50 );
	m_TreePositions[ 1 ] = TPoint( 40, -25 );
	m_TreePositions[ 2 ] = TPoint( -30, -45 );
	m_TreePositions[ 3 ] = TPoint( 20, -40 );
	m_TreePositions[ 4 ] = TPoint( -15, 29 );

	for( int i = 0; i < g_EnvironmentNumTrees; ++i )
	{
		D3DXVECTOR3 vPosition;
		vPosition.x = static_cast< float >( m_TreePositions[ i ].y );
		vPosition.y = static_cast< float >( 0 );
		vPosition.z = static_cast< float >( m_TreePositions[ i ].x );
		m_BTrees[ i ].SetPosition( vPosition );
	}

	// Populate texture array
	PopulateCubeTextureArray();
	PopulateWaterTextureArray();

	// Creates transparent blend state
	D3D10_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend = D3D10_BLEND_SRC_COLOR;
	blendDesc.DestBlend = D3D10_BLEND_INV_SRC_COLOR;
	blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HR( md3dDevice->CreateBlendState( &blendDesc, &m_TransparentBS ) );	

	// Create stencil states
	CreateStencilStates();
}


void CEnvApp::CreateStencilStates()
{
	D3D10_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable					= true;
	dsDesc.DepthWriteMask				= D3D10_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc					= D3D10_COMPARISON_LESS;
	dsDesc.StencilEnable				= true;
	dsDesc.StencilReadMask				= 0xff;
	dsDesc.StencilWriteMask				= 0xff;

	// Front face operations
	dsDesc.FrontFace.StencilFailOp		= D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp	= D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp		= D3D10_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc		= D3D10_COMPARISON_ALWAYS;
	
	// Culling on so just setting these
	dsDesc.BackFace.StencilFailOp		= D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp	= D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp		= D3D10_STENCIL_OP_REPLACE;
	dsDesc.BackFace.StencilFunc		= D3D10_COMPARISON_ALWAYS;

	// Create surface state
	HR( md3dDevice->CreateDepthStencilState( &dsDesc, &m_SurfaceDSS ) );

	dsDesc.DepthEnable					= true;
	dsDesc.DepthWriteMask				= D3D10_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc					= D3D10_COMPARISON_ALWAYS;
	dsDesc.FrontFace.StencilPassOp		= D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc		= D3D10_COMPARISON_EQUAL;

	// Create reflection state
	HR( md3dDevice->CreateDepthStencilState( &dsDesc, &m_ReflectionDSS ) );

	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory( &rsDesc, sizeof( D3D10_RASTERIZER_DESC ) );
	rsDesc.FillMode					= D3D10_FILL_SOLID;
	rsDesc.CullMode					= D3D10_CULL_BACK;
	rsDesc.FrontCounterClockwise	= true;

	HR( md3dDevice->CreateRasterizerState( &rsDesc, &m_CullRS ) );

	D3D10_BLEND_DESC bDesc;
	ZeroMemory( &bDesc, sizeof( D3D10_BLEND_DESC ) );
	bDesc.AlphaToCoverageEnable			= false;
	bDesc.BlendEnable[ 0 ]				= true;
	bDesc.SrcBlend						= D3D10_BLEND_BLEND_FACTOR;
	bDesc.DestBlend						= D3D10_BLEND_INV_BLEND_FACTOR;
	bDesc.BlendOp						= D3D10_BLEND_OP_ADD;
	bDesc.SrcBlendAlpha					= D3D10_BLEND_ONE;
	bDesc.DestBlendAlpha				= D3D10_BLEND_ZERO;
	bDesc.BlendOpAlpha					= D3D10_BLEND_OP_ADD;
	bDesc.RenderTargetWriteMask[ 0 ]	= D3D10_COLOR_WRITE_ENABLE_ALL;

	HR( md3dDevice->CreateBlendState( &bDesc, &m_ReflectionBS ) );
}

void CEnvApp::onResize()
{
	D3DApp::onResize();

	float aspect = static_cast< float >( mClientWidth / mClientHeight );
	D3DXMatrixPerspectiveFovLH( &m_Proj, 0.25f * PI, aspect, 1.0f, 1000.0f );
}

void CEnvApp::updateScene( float _fDT, float _fGT )
{	
	D3DXVECTOR3 vTangent;
	D3DXVec3Normalize( &vTangent, &( D3DXVECTOR3( 0.5f, 0.5f, 0.5f ) - D3DXVECTOR3( 0.5f, 0.5f, -0.5f ) ) );
	
	m_Light.Update( _fDT );
	mClearColor = m_Light.GetSkyColor();

	if( m_Light.IsStartedRaining() )
	{
		m_Particles.Reset();
	}

	if( !m_BTerrain.IsSmoothed() )
	{
		// Output controls to the screen
		mFrameStats = L"Press 'D' to apply Diamond Square algorithm to terrain\nPress 'S' to smooth terrain\n(For best result run Diamond Square to completion)";

		// Check keyboard inputs
		if( m_Keyboard.KeyPressed( 0x44 ) ) // D
		{
			m_BTerrain.RunDiamondSquare();
		}		
		else if( m_Keyboard.KeyPressed( 0x53 ) ) // s
		{
			m_BTerrain.RunSmoothing();
		}

		if( m_BTerrain.IsSmoothed() )
		{
			float fCenterH = static_cast< float >( m_BTerrain.GetHeightAtPoint( TPoint( 0, 0 ) ) );

			m_Camera.SetEyePos( D3DXVECTOR3( 0.0f, fCenterH + 2.0f, 0.0f ) );
			m_Camera.SetLookAt( D3DXVECTOR3( 0.0f, fCenterH + 2.0f, 1.0f ) );

			m_Light.SetFocus( D3DXVECTOR3( 0.0f, fCenterH, 0.0f ) );
			m_Particles.Reset();

			m_Water.SetHeight( m_BTerrain.GetPoolHeight() + 1.2f );

			for( int i = 0; i < g_EnvironmentNumTrees; ++i )
			{
				D3DXVECTOR3 vPosition;
				vPosition.x = static_cast< float >( m_TreePositions[ i ].y );
				vPosition.y = static_cast< float >( m_BTerrain.GetHeightAtPoint( m_TreePositions[ i ] ) + 1 );
				vPosition.z = static_cast< float >( m_TreePositions[ i ].x );
				m_BTrees[ i ].SetPosition( vPosition );
			}
		}
	}
	else
	{		
		D3DApp::updateScene( _fDT, _fGT );
		
		if( m_Keyboard.KeyPressed( 0x52 ) ) // R
		{
			m_Camera.SetEyePos( D3DXVECTOR3( 100.0f, 50.0f, 100.0f ) );
			m_Camera.SetLookAt( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
			m_BTerrain.Reset();
		}
		else if( m_Keyboard.KeyPressed( 0x45 ) ) // E
		{
			m_bCube = !m_bCube;
		}

		m_Camera.Update( _fDT );		

		if( m_Light.IsRaining() )
		{
			m_Particles.Update( _fDT, _fGT );
		}

		m_Water.Update( _fDT );

		if( m_bCube )
		{
			D3DXVECTOR3 pos;
			D3DXVec3Normalize( &pos, &( m_Camera.GetLookAt() - m_Camera.GetEyePosition() ) );
			pos *= 5.0f;
			m_ReflectionObject.SetPosition( pos + m_Camera.GetEyePosition() );
		}
	}

	m_Keyboard.UpdatePrevious();
}

void CEnvApp::drawScene()
{
	D3DApp::drawScene();

	// Restore default states for when using fonts
	md3dDevice->OMSetDepthStencilState( 0, 0 );
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dDevice->OMSetBlendState( 0, blendFactors, 0xffffffff );

	md3dDevice->IASetInputLayout( m_IVertexLayout );
	md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Draw to the shadow map
	m_ShadowMap.Begin();
	DrawShadowMap();
	m_ShadowMap.End();

	// Reset target/view
	md3dDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width	= mClientWidth;
	vp.Height	= mClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth	= 1.0f;
	md3dDevice->RSSetViewports( 1, &vp );

	// Draw quad with shadow map
	md3dDevice->IASetInputLayout( m_QVertexLayout );
	m_fxDrawShadowMapVar->SetResource( m_ShadowMap.GetDepthMap() );
	ID3D10EffectPass* pass = m_DShadowTech->GetPassByIndex( 0 );
	pass->Apply( 0 );
	//m_NDCQuad.Draw();

	md3dDevice->IASetInputLayout( m_IVertexLayout );

	// Set constants
	m_fxEyePosVar->SetRawValue( &m_Camera.GetEyePosition(), 0, sizeof( D3DXVECTOR3 ) );
	m_fxLightVar->SetRawValue( &m_Light.GetLight(), 0, sizeof( Light ) );

	m_fxShadowMapVar->SetResource( m_ShadowMap.GetDepthMap() );

	m_WVP = m_EnvWorld * m_Camera.GetViewMatrix() * m_Proj;
	m_fxWVPVar->SetMatrix( reinterpret_cast< float* >( &m_WVP ) );
	m_fxLightWVPVar->SetMatrix( ( float* ) &( m_EnvWorld * m_Light.GetViewMatrix() * m_Light.GetVolume() ) );
	m_fxWorldVar->SetMatrix( reinterpret_cast< float* >( &m_EnvWorld ) );
	m_fxTextureArrayVar->SetResourceArray( &m_fxTextureArrayRV, 0, 1 );
	m_fxFogVar->SetRawValue( &mClearColor, 0, sizeof( D3DXVECTOR3 ) );

	// Not transforming texture co-ords
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity( &texMtx );
	m_fxTexMtxVar->SetMatrix( reinterpret_cast< float* >( &texMtx ) );
	
	D3D10_TECHNIQUE_DESC techDesc;
	m_Tech->GetDesc( &techDesc );

	for( UINT i = 0; i < techDesc.Passes; ++i )
	{	
		// Set up description for terrain
		m_Tech->GetPassByIndex( i )->Apply( 0 );
		m_BTerrain.Draw();

		if( m_BTerrain.IsSmoothed() )
		{
			for( int i = 0; i < g_EnvironmentNumTrees; ++i )
			{
				m_BTrees[ i ].Draw();
			}

			if( m_bCube )
			{
				m_ReflectionObject.Draw();
			}
		}

		m_fxShadowMapVar->SetResource( 0 );
		m_Tech->GetPassByIndex( i )->Apply( 0 );		
	}

	if( m_BTerrain.IsSmoothed() )
	{
		md3dDevice->OMSetBlendState( m_TransparentBS, blendFactors, 0xffffffff );
		m_fxTextureArrayVar->SetResource( m_fxWaterTextureArrayRV );
		md3dDevice->IASetInputLayout( m_SVertexLayout );

		m_StandardTech->GetDesc( &techDesc );
		for( UINT i = 0; i < techDesc.Passes; ++i )
		{			
			m_StandardTech->GetPassByIndex( i )->Apply( 0 );

 			md3dDevice->OMSetDepthStencilState( m_SurfaceDSS, 1 );
			m_Water.Draw();
			md3dDevice->OMSetDepthStencilState( 0, 0 );
		}	

		if( m_bCube )
		{	
			D3DXPLANE plReflection( 0.0f, 1.0f, 0.0f, m_Water.GetHeight() * -1.0f );
			D3DXMATRIX matReflection, matWorld;
			D3DXMatrixReflect( &matReflection, &plReflection );	

			D3DXVECTOR3 vOldLightDir = m_Light.GetDirection();
			D3DXVECTOR3 vLightDir;
			D3DXVec3TransformNormal( &vLightDir, &vOldLightDir, &matReflection );
			m_Light.SetDirection( vLightDir );


			m_Tech->GetDesc( &techDesc );
			for( UINT i = 0; i < techDesc.Passes; ++i )
			{
				m_WVP		= matReflection * m_Camera.GetViewMatrix() * m_Proj;
				m_fxWVPVar->SetMatrix( ( float* ) m_WVP );
				m_fxWorldVar->SetMatrix( ( float* ) &matReflection );
				m_fxTextureArrayVar->SetResource( m_fxTextureArrayRV );
				md3dDevice->IASetInputLayout( m_IVertexLayout );
				m_Tech->GetPassByIndex( i )->Apply( 0 );

				md3dDevice->RSSetState( m_CullRS );
				float blendF[] = { 0.65f, 0.65f, 0.65f, 0.0f };
				md3dDevice->OMSetBlendState( m_ReflectionBS, blendF, 0xffffffff );
				md3dDevice->OMSetDepthStencilState( m_ReflectionDSS, 1 );	

				m_ReflectionObject.Draw();

				md3dDevice->OMSetDepthStencilState( 0, 0 );
				md3dDevice->RSSetState( 0 );
			}

			md3dDevice->OMSetBlendState( 0, blendFactors, 0xffffffff );
			m_Light.SetDirection( vOldLightDir );
		}
	}	

	// Draw particles
	if( m_BTerrain.IsSmoothed() && m_Light.IsRaining() )
	{
		m_Particles.SetEyePosition( m_Camera.GetEyePosition() );
		m_Particles.SetEmitPosition( m_Camera.GetEyePosition() );
		m_Particles.Draw( m_Camera.GetViewMatrix(), m_Proj );
	}

	m_Light.Draw( m_Camera.GetViewMatrix(), m_Proj, m_Camera.GetEyePosition() );

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, WHITE);

	mSwapChain->Present( 0, 0 );
}

void CEnvApp::DrawShadowMap()
{
	D3D10_TECHNIQUE_DESC techDesc;
	m_CShadowTech->GetDesc( &techDesc );

	m_fxShadowDiffuseVar->SetResource( m_fxTextureArrayRV );

	for( UINT i = 0; i < techDesc.Passes; ++i )
	{
		m_fxShadowLightWVPVar->SetMatrix( ( float* ) &( m_EnvWorld * m_Light.GetViewMatrix() * m_Light.GetVolume() ) );
		m_CShadowTech->GetPassByIndex( i )->Apply( 0 );
		m_BTerrain.Draw();
		
		if( m_BTerrain.IsSmoothed() )
		{
			for( int i = 0; i < g_EnvironmentNumTrees; ++i )
			{
				m_BTrees[ i ].Draw();
			}

			if( m_bCube )
			{
				m_ReflectionObject.Draw();
			}
		}
	}	
}

void CEnvApp::BuildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile( L"Assets/Shaders/tex.fx", 0, 0, "fx_4_0", shaderFlags, 0,
		md3dDevice, 0, 0, &m_FX, &compilationErrors, 0 );

	if( FAILED( hr ) )
	{
		if( compilationErrors )
		{
			MessageBoxA( 0, ( char* )compilationErrors->GetBufferPointer(), 0, 0 );
			ReleaseCOM( compilationErrors );
		}
		DXTrace( __FILE__, ( DWORD ) __LINE__, hr, L"D3DX10CreateEffectFromFile", true );
	}

	m_Tech = m_FX->GetTechniqueByName( "InstancedTech" );
	m_StandardTech = m_FX->GetTechniqueByName( "StandardTech" );
	
	m_fxWVPVar			= m_FX->GetVariableByName( "gWVP" )->AsMatrix();
	m_fxWorldVar		= m_FX->GetVariableByName( "gWorld")->AsMatrix();
	m_fxEyePosVar		= m_FX->GetVariableByName( "gEyePosW" );
	m_fxLightVar		= m_FX->GetVariableByName( "gLight" );
	m_fxTextureArrayVar	= m_FX->GetVariableByName( "gTextureArray" )->AsShaderResource();
	m_fxTexMtxVar		= m_FX->GetVariableByName( "gTexMtx" )->AsMatrix();
	m_fxFogVar			= m_FX->GetVariableByName( "gFogColor" );
	m_fxShadowMapVar	= m_FX->GetVariableByName( "gShadowMap" )->AsShaderResource();
	m_fxLightWVPVar		= m_FX->GetVariableByName( "gLightWVP" )->AsMatrix();


	
	// Create shadow map data
	
	hr = D3DX10CreateEffectFromFile( L"Assets/Shaders/shadow_mapping.fx", 0, 0, "fx_4_0", shaderFlags, 0,
		md3dDevice, 0, 0, &m_CShadowFX, &compilationErrors, 0 );

	if( FAILED( hr ) )
	{
		if( compilationErrors )
		{
			MessageBoxA( 0, ( char* )compilationErrors->GetBufferPointer(), 0, 0 );
			ReleaseCOM( compilationErrors );
		}
		DXTrace( __FILE__, ( DWORD ) __LINE__, hr, L"D3DX10CreateEffectFromFile", true );
	}

	m_CShadowTech = m_CShadowFX->GetTechniqueByName( "CreateShadowMapTech" );	
	m_fxShadowLightWVPVar	= m_CShadowFX->GetVariableByName( "gLightWVP" )->AsMatrix();
	m_fxShadowDiffuseVar	= m_CShadowFX->GetVariableByName( "gDiffuseMap" )->AsShaderResource();



	// Draw shadow map data

	hr = D3DX10CreateEffectFromFile( L"Assets/Shaders/draw_shadow_mapping.fx", 0, 0, "fx_4_0", shaderFlags, 0,
		md3dDevice, 0, 0, &m_DShadowFX, &compilationErrors, 0 );

	if( FAILED( hr ) )
	{
		if( compilationErrors )
		{
			MessageBoxA( 0, ( char* )compilationErrors->GetBufferPointer(), 0, 0 );
			ReleaseCOM( compilationErrors );
		}
		DXTrace( __FILE__, ( DWORD ) __LINE__, hr, L"D3DX10CreateEffectFromFile", true );
	}

	m_DShadowTech			= m_DShadowFX->GetTechniqueByName( "DrawShadowMapTech" );	
	m_fxDrawShadowMapVar	= m_DShadowFX->GetVariableByName( "gShadowMap")->AsShaderResource();	
}

void CEnvApp::BuildVertexLayouts()
{
	D3D10_INPUT_ELEMENT_DESC ivDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXID", 0, DXGI_FORMAT_R32_UINT, 1, 12, D3D10_INPUT_PER_INSTANCE_DATA, 1 },
	};

	D3D10_PASS_DESC passDesc;
	m_Tech->GetPassByIndex( 0 )->GetDesc( &passDesc );
	HR( md3dDevice->CreateInputLayout( ivDesc, 6, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_IVertexLayout ) );

	D3D10_INPUT_ELEMENT_DESC svDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXID", 0, DXGI_FORMAT_R32_UINT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_StandardTech->GetPassByIndex( 0 )->GetDesc( &passDesc );
	HR( md3dDevice->CreateInputLayout( svDesc, 5, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_SVertexLayout ) );

	D3D10_INPUT_ELEMENT_DESC qvDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_DShadowTech->GetPassByIndex( 0 )->GetDesc( &passDesc );
	HR( md3dDevice->CreateInputLayout( qvDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_QVertexLayout ) );
}

void CEnvApp::PopulateCubeTextureArray()
{
	// Textures to be used
	std::wstring fileNames[ g_EnvironmentTextureArrayNum ] =
	{
		L"Assets/Textures/Cube/grass_block_COLOR.dds",		
		L"Assets/Textures/Cube/grass_block_SPEC.dds",
		L"Assets/Textures/Cube/grass_block_NRM.dds",

		L"Assets/Textures/Cube/wood_block_COLOR.dds",
		L"Assets/Textures/Cube/wood_block_SPEC.dds",
		L"Assets/Textures/Cube/wood_block_NRM.dds",

		L"Assets/Textures/Cube/leaf_block_COLOR.dds",
		L"Assets/Textures/Cube/leaf_block_SPEC.dds",
		L"Assets/Textures/Cube/leaf_block_NRM.dds",

		L"Assets/Textures/Cube/crafting_table_COLOR.dds",
		L"Assets/Textures/Cube/crafting_table_SPEC.dds",
		L"Assets/Textures/Cube/crafting_table_NRM.dds",

		L"Assets/Textures/Cube/dirt_block_COLOR.dds",		
		L"Assets/Textures/Cube/dirt_block_SPEC.dds",
		L"Assets/Textures/Cube/dirt_block_NRM.dds",
	};

	// Load textures from file
	ID3D10Texture2D* textures[ g_EnvironmentTextureArrayNum ];
	for( int i = 0; i < g_EnvironmentTextureArrayNum; ++i )
	{
		D3DX10_IMAGE_LOAD_INFO loadInfo;

		loadInfo.Width			= D3DX10_FROM_FILE;
		loadInfo.Height			= D3DX10_FROM_FILE;
		loadInfo.Depth			= D3DX10_FROM_FILE;
		loadInfo.FirstMipLevel	= 0;
		loadInfo.MipLevels		= D3DX10_FROM_FILE;
		loadInfo.Usage			= D3D10_USAGE_STAGING;
		loadInfo.BindFlags		= 0;
		loadInfo.CpuAccessFlags	= D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
		loadInfo.MiscFlags		= 0;
		loadInfo.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter			= D3DX10_FILTER_NONE;
		loadInfo.MipFilter		= D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo		= 0;

		HR( D3DX10CreateTextureFromFile( md3dDevice, fileNames[ i ].c_str(), &loadInfo, 
			0, ( ID3D10Resource** )&textures[ i ], 0 ) );
	}

	// Get texture description
	D3D10_TEXTURE2D_DESC textElementDesc;
	textures[ 0 ]->GetDesc( &textElementDesc );

	// Fill texture array description
	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width				= textElementDesc.Width;
	texArrayDesc.Height				= textElementDesc.Height;
	texArrayDesc.MipLevels			= textElementDesc.MipLevels;
	texArrayDesc.ArraySize			= g_EnvironmentTextureArrayNum;
	texArrayDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count	= 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage				= D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags			= D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags		= 0;
	texArrayDesc.MiscFlags			= 0;

	// Create texture array
	ID3D10Texture2D* textArray = nullptr;
	HR( md3dDevice->CreateTexture2D( &texArrayDesc, NULL, &textArray) );

	// Put textures into array
	for( int i = 0; i < g_EnvironmentTextureArrayNum; ++i )
	{
		for( unsigned j = 0; j < textElementDesc.MipLevels; ++j )
		{
			D3D10_MAPPED_TEXTURE2D mappedText2D;
			textures[ i ]->Map( j, D3D10_MAP_READ, 0, &mappedText2D );

			md3dDevice->UpdateSubresource( textArray, D3D10CalcSubresource( j, i, textElementDesc.MipLevels ),
				0, mappedText2D.pData, mappedText2D.RowPitch, 0 );
			
			textures[ i ]->Unmap( j );
		}
	}

	// Fill shader resource descrition
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension	= D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = g_EnvironmentTextureArrayNum;

	// Create Texture array resource view
	HR( md3dDevice->CreateShaderResourceView( textArray, &viewDesc, &m_fxTextureArrayRV ) );

	ReleaseCOM( textArray );
	for( int i = 0; i < g_EnvironmentTextureArrayNum; ++i )
	{
		ReleaseCOM( textures[ i ] );
	}
}

void CEnvApp::PopulateWaterTextureArray()
{
	// Textures to be used
	std::wstring fileNames[ g_EnvironmentWaterTexArrayNum ] =
	{
		L"Assets/Textures/Surface/water1_COLOR.dds",
		L"Assets/Textures/Surface/water1_SPEC.dds",	
		L"Assets/Textures/Surface/water1_NRM.dds",	
		L"Assets/Textures/Surface/water2_COLOR.dds",
		L"Assets/Textures/Surface/water2_SPEC.dds",	
		L"Assets/Textures/Surface/water2_NRM.dds",		
		L"Assets/Textures/Surface/water3_COLOR.dds",
		L"Assets/Textures/Surface/water3_SPEC.dds",	
		L"Assets/Textures/Surface/water3_NRM.dds",	
		L"Assets/Textures/Surface/water4_COLOR.dds",
		L"Assets/Textures/Surface/water4_SPEC.dds",	
		L"Assets/Textures/Surface/water4_NRM.dds",				
	};

	// Load textures from file
	ID3D10Texture2D* textures[ g_EnvironmentTextureArrayNum ];
	for( int i = 0; i < g_EnvironmentWaterTexArrayNum; ++i )
	{
		D3DX10_IMAGE_LOAD_INFO loadInfo;

		loadInfo.Width			= D3DX10_FROM_FILE;
		loadInfo.Height			= D3DX10_FROM_FILE;
		loadInfo.Depth			= D3DX10_FROM_FILE;
		loadInfo.FirstMipLevel	= 0;
		loadInfo.MipLevels		= D3DX10_FROM_FILE;
		loadInfo.Usage			= D3D10_USAGE_STAGING;
		loadInfo.BindFlags		= 0;
		loadInfo.CpuAccessFlags	= D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
		loadInfo.MiscFlags		= 0;
		loadInfo.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter			= D3DX10_FILTER_NONE;
		loadInfo.MipFilter		= D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo		= 0;

		HR( D3DX10CreateTextureFromFile( md3dDevice, fileNames[ i ].c_str(), &loadInfo, 
			0, ( ID3D10Resource** )&textures[ i ], 0 ) );
	}

	// Get texture description
	D3D10_TEXTURE2D_DESC textElementDesc;
	textures[ 0 ]->GetDesc( &textElementDesc );

	// Fill texture array description
	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width				= textElementDesc.Width;
	texArrayDesc.Height				= textElementDesc.Height;
	texArrayDesc.MipLevels			= textElementDesc.MipLevels;
	texArrayDesc.ArraySize			= g_EnvironmentTextureArrayNum;
	texArrayDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count	= 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage				= D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags			= D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags		= 0;
	texArrayDesc.MiscFlags			= 0;

	// Create texture array
	ID3D10Texture2D* textArray = nullptr;
	HR( md3dDevice->CreateTexture2D( &texArrayDesc, NULL, &textArray) );

	// Put textures into array
	for( int i = 0; i < g_EnvironmentWaterTexArrayNum; ++i )
	{
		for( unsigned j = 0; j < textElementDesc.MipLevels; ++j )
		{
			D3D10_MAPPED_TEXTURE2D mappedText2D;
			textures[ i ]->Map( j, D3D10_MAP_READ, 0, &mappedText2D );

			md3dDevice->UpdateSubresource( textArray, D3D10CalcSubresource( j, i, textElementDesc.MipLevels ),
				0, mappedText2D.pData, mappedText2D.RowPitch, 0 );
			
			textures[ i ]->Unmap( j );
		}
	}

	// Fill shader resource descrition
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension	= D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = g_EnvironmentWaterTexArrayNum;

	// Create Texture array resource view
	HR( md3dDevice->CreateShaderResourceView( textArray, &viewDesc, &m_fxWaterTextureArrayRV ) );

	ReleaseCOM( textArray );
	for( int i = 0; i < g_EnvironmentWaterTexArrayNum; ++i )
	{
		ReleaseCOM( textures[ i ] );
	}
}