//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: collider.h
// Description	: Declaration for CCollider
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

#ifndef GUARD_AIDEN_STOREY_COLLIDER_H_201411032029
#define GUARD_AIDEN_STOREY_COLLIDER_H_201411032029

// Library Includes
#include <vector>

// Local Includes
#include "drawable.h"
#include "math_functions.h"
#include "mesh.h"

// Constants
enum class EColliderType
{
	COLLIDER_SPHERE,
	COLLIDER_CAPSULE,
	COLLIDER_PYRAMID,

	COLLIDER_NONE,
};

// Prototypes
class CCollider : public CDrawable
{
public:
	// Constructors
	CCollider( EColliderType _eType );
	~CCollider();

	// Accessors
	bool GetActive() const;
	void SetActive( const bool& _kbActive );

	// Other
	D3DXVECTOR3 CheckCollision( const D3DXVECTOR3& _vPosition );

protected:
	// Other
	D3DXVECTOR3 CheckCollisionSphere( const D3DXVECTOR3& _vPosition );
	D3DXVECTOR3 CheckCollisionCapsule( const D3DXVECTOR3& _vPosition );
	D3DXVECTOR3 CheckCollisionPyramid( const D3DXVECTOR3& _vPosition );

	void SetupPlanes();
private:

	// Member Variables
public:
protected:
private:
	CMesh*			m_pMesh;

	EColliderType	m_eType;
	bool			m_bActive;

	D3DXVECTOR3		m_vPosition{ 0.0f, 0.07f, 0.1f };
	float			m_fRadius{ 1.0f };

	float			m_fCapsuleLength{ 2.0f };
	D3DXVECTOR3		m_vCapsuleA{ m_vPosition + D3DXVECTOR3( 0.0f, + m_fCapsuleLength * 0.5f, 0.0f ) };
	D3DXVECTOR3		m_vCapsuleB{ m_vPosition + D3DXVECTOR3( 0.0f, - m_fCapsuleLength * 0.5f, 0.0f ) };
	D3DXVECTOR3		m_vCapsuleU{ m_vCapsuleB - m_vCapsuleA };

	std::vector< Math::TPlane >	m_PlaneList;
};

#endif // GUARD_AIDEN_STOREY_COLLIDER_H_201411032029