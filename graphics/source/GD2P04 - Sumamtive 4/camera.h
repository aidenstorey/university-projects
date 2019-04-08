#ifndef GUARD_AIDEN_STOREY_CAMERA_H_20140612
#define GUARD_AIDEN_STOREY_CAMERA_H_20140612

// Local Includes
#include "d3dUtil.h"

// Prototypes
class CCamera
{
	// Member Functions
public:
	// Constructors
	CCamera();
	~CCamera();
	
	// Accessors
	D3DXMATRIX GetViewMatrix() const;
	void SetViewMatrix( const D3DXMATRIX _matView );

	D3DXVECTOR3 GetEyePosition() const;
	void SetEyePos( const D3DXVECTOR3 _vEyePos );

	D3DXVECTOR3 GetLookAt() const;
	void SetLookAt( const D3DXVECTOR3 _vLookAt );

	D3DXVECTOR3 GetUp() const;
	void SetUp( const D3DXVECTOR3 _vUp );

	// Other
	void Initialise( const D3DXVECTOR3 _vEyePos, const D3DXVECTOR3 _vLookAt, const D3DXVECTOR3 _vUp );
	void Update( float _fDT );

protected:
private:

	// Member Variables
public:
protected:
private:
	D3DXMATRIX	m_matView;

	D3DXVECTOR3	m_vEyePos;
	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 m_vUp;
};

#endif // GUARD_AIDEN_STOREY_CAMERA_H_20140612