#ifndef GUARD_AIDEN_STOREY_WRITABLE_TEXTURE_H_20140627
#define GUARD_AIDEN_STOREY_WRITABLE_TEXTURE_H_20140627

// Local Includes
#include "d3dUtil.h"

// Prototypes
class CWritableDepthTexture
{
	// Member Functions
public:
	// Constructors
	CWritableDepthTexture();
	~CWritableDepthTexture();

	// Accessors	
	ID3D10ShaderResourceView*	GetDepthMap();

	// Other 
	void Initialise( ID3D10Device* _pDevice, DWORD _dwWidth, DWORD _dwHeight );

	void Begin();
	void End();

protected:
	void BuildDepthMap();
private:

	// Member Variables
public:
protected:
private:
	ID3D10Device*				m_pDevice;
	
	DWORD						m_dwWidth;
	DWORD						m_dwHeight;
	
	ID3D10ShaderResourceView*	m_DepthMapRV;
	ID3D10DepthStencilView*		m_DepthMapDSV;

	D3D10_VIEWPORT				m_Viewport;
};

#endif // GUARD_AIDEN_STOREY_WRITABLE_TEXTURE_H_20140627