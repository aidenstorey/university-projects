//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_VIEWPORT_H_201408192208
#define GUARD_AIDEN_STOREY_VIEWPORT_H_201408192208

// Library Includes
#include <D3DX10.h>
#include <map>

// Forward Decleration
class CCamera;
enum ECameraMode;

// Prototypes
class CViewport
{
	// Member Functions
public:
	// Constructors
	CViewport();
	~CViewport();

	// Accessors
	CCamera* GetCamera( unsigned int _uiCameraID );

	bool SetActiveCamera( unsigned int _uiCameraID );
	CCamera* GetActiveCamera();

	D3D10_VIEWPORT* GetD3DViewport();
	void SetViewport( const D3D10_VIEWPORT&	_D3DViewport );

	unsigned int GetX() const;
	void SetX( const unsigned int _uiX );

	unsigned int GetY() const;
	void SetY( const unsigned int _uiY );

	unsigned int GetWidth() const;
	void SetWidth( const unsigned int _uiWidth );

	unsigned int GetHeight() const;
	void SetHeight( const unsigned int _uiHeight );

	D3DXCOLOR GetClearColor() const;
	void SetClearColor( const D3DXCOLOR _vColor );
	
	// Other
	void Initialise( const unsigned int _uiX, const unsigned int _uiY, const unsigned int _uiWidth, 
		const unsigned int _uiHeight, D3DXCOLOR _vClearColor );

	CCamera* CreateCamera( float _fFOV, ECameraMode _eMode, float _fNearClip, float _fFarClip, float _fScale, unsigned int& _ruiCameraID );
protected:
private:

	// Member Variables
public:
protected:
private:
	D3D10_VIEWPORT						m_D3DViewport;
	D3DXCOLOR							m_vClearColor;
	CCamera*							m_pActiveCamera;
	std::map< unsigned int, CCamera* >	m_mapCameras;

};

#endif // GUARD_AIDEN_STOREY_VIEWPORT_H_201408192208