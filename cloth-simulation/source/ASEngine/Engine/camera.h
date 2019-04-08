//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_INTERFACE_CAMERA_H_20140724
#define GUARD_AIDEN_STOREY_INTERFACE_CAMERA_H_20140724

// Library Includes
#include <D3DX10.h>

// Types
enum ECameraMode
{
	INVALID_CAMERAMODE,
	CAMERAMODE_ORTHOGONAL,
	CAMERAMODE_PERSPECTIVE,
	MAX_CAMERAMODE,
};

// Prototypes
class CViewport;

class CCamera
{
	// Member Functions
public:
	// Constructors
	CCamera();
	virtual ~CCamera();

	// Accessors
	D3DXMATRIX GetViewMatrix() const;
	void SetViewMatrix( const D3DXMATRIX _matView );

	D3DXMATRIX GetProjMatrix() const;
	void SetProjMatrix( const D3DXMATRIX _matProj );

	D3DXMATRIX GetViewProjMatrix() const;

	D3DXVECTOR3 GetPosition() const;
	void SetPosition( const D3DXVECTOR3 _vPosition );

	D3DXVECTOR3 GetTarget() const;
	void SetTarget( const D3DXVECTOR3 _vTarget );

	D3DXVECTOR3 GetUp() const;
	void SetUp( const D3DXVECTOR3 _vUp );

	float GetNearPlane() const;
	void SetNearPlane( const float _fNear );

	float GetFarPlane() const;
	void SetFarPlane( const float _fFar );

	void SetClipPlanes( const float _fNear, const float _fFar );

	float GetScale() const;
	void SetScale( const float _fScale );
	
	CViewport* GetViewport() const;

	ECameraMode GetCameraMode() const;
	void SetCameraMode( const ECameraMode _eMode );
	
	// Other 
	virtual void Initialise( CViewport* _pViewport, float _fFOV, ECameraMode _eMode, float _fNearClip, float _fFarClip, float _fScale, unsigned int _uiID );

	void ResetViewMatrix();
	void ResetProjMatrix();
protected:
private:

	// Member Variables
public:
protected:
private:
	unsigned int	m_uiID;
	ECameraMode		m_eMode;
	CViewport*		m_pViewport;

	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;

	D3DXVECTOR3		m_vPosition;
	D3DXVECTOR3		m_vTarget;
	D3DXVECTOR3		m_vUp;

	float			m_fNearPlane;
	float			m_fFarPlane;

	float			m_fFOV;

	float			m_fScale;
};

#endif // GUARD_AIDEN_STOREY_INTERFACE_CAMERA_H_20140724