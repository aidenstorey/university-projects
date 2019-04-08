// This Include
#include "writable_texture.h"

// Implementation
CWritableDepthTexture::CWritableDepthTexture() : 
	m_pDevice( nullptr ), m_DepthMapRV( nullptr ), m_DepthMapDSV( nullptr ),
	m_dwWidth( 0 ), m_dwHeight( 0 )
{
	ZeroMemory( &m_Viewport, sizeof( D3D10_VIEWPORT ) );
}

CWritableDepthTexture::~CWritableDepthTexture()
{
	ReleaseCOM( m_DepthMapRV );
	ReleaseCOM( m_DepthMapDSV );
}

ID3D10ShaderResourceView* CWritableDepthTexture::GetDepthMap()
{
	return ( m_DepthMapRV );
}

void CWritableDepthTexture::Initialise( ID3D10Device* _pDevice, DWORD _dwWidth, DWORD _dwHeight )
{
	// Local Instance 
	m_pDevice		= _pDevice;
	m_dwWidth		= _dwWidth;
	m_dwHeight		= _dwHeight;

	// Build map
	BuildDepthMap();

	// Initialise viewport
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width	= m_dwWidth;
	m_Viewport.Height	= m_dwHeight;
	m_Viewport.MinDepth	= 0.0f;
	m_Viewport.MaxDepth	= 1.0f;
}

void CWritableDepthTexture::Begin()
{
	ID3D10RenderTargetView* targets[ 1 ] = { 0 };
	m_pDevice->OMSetRenderTargets( 1, targets, m_DepthMapDSV );
	m_pDevice->RSSetViewports( 1, &m_Viewport );
	m_pDevice->ClearDepthStencilView( m_DepthMapDSV, D3D10_CLEAR_DEPTH, 1.0f, 0 );
}

void CWritableDepthTexture::End()
{
	// Left blank so that when used code can be more understandable as there is an actual end
}

void CWritableDepthTexture::BuildDepthMap()
{
	ID3D10Texture2D* depthMap = 0;

	// Fill description
	D3D10_TEXTURE2D_DESC tDesc;
	tDesc.Width					= m_dwWidth;
	tDesc.Height				= m_dwHeight;
	tDesc.MipLevels				= 1;
	tDesc.ArraySize				= 1;
	tDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
	tDesc.SampleDesc.Count		= 1;
	tDesc.SampleDesc.Quality	= 0;
	tDesc.Usage					= D3D10_USAGE_DEFAULT;
	tDesc.BindFlags				= D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags		= 0;
	tDesc.MiscFlags				= 0;

	// Create texture
	HR( m_pDevice->CreateTexture2D( &tDesc, 0, &depthMap ) );

	// Fill description
	D3D10_DEPTH_STENCIL_VIEW_DESC dsDesc;
	dsDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	dsDesc.ViewDimension		= D3D10_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice	= 0;

	// Create depth stencil view
	HR( m_pDevice->CreateDepthStencilView( depthMap, &dsDesc, &m_DepthMapDSV ) );

	// Fill description
	D3D10_SHADER_RESOURCE_VIEW_DESC rvDesc;
	rvDesc.Format						= DXGI_FORMAT_R32_FLOAT;
	rvDesc.ViewDimension				= D3D10_SRV_DIMENSION_TEXTURE2D;
	rvDesc.Texture2D.MipLevels			= tDesc.MipLevels;
	rvDesc.Texture2D.MostDetailedMip	= 0;

	// Create shader resource view
	HR( m_pDevice->CreateShaderResourceView( depthMap, &rvDesc, &m_DepthMapRV ) );

	// Clean up 
	ReleaseCOM( depthMap );
}