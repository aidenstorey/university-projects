//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: collider.cpp
// Description	: Implementation for CCollider
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// Library Includes
#include <string>

// Local includes
#include "engine.h"
#include "math_functions.h"

// This Include
#include "collider.h"

// Implementation
//**************************************************
//
// CCollider: Initialises collider
//
// @author: Aiden Storey
//
// @param[in]	_eType			type of collider
//
//**************************************************
CCollider::CCollider( EColliderType _eType ) :
	m_eType( _eType )
{
	// Create file path from passed type
	std::wstring strMeshFile( L"Assets/Objects/" );
	switch( this->m_eType )
	{
		case EColliderType::COLLIDER_SPHERE: { strMeshFile += L"sphere.obj"; } break;
		case EColliderType::COLLIDER_CAPSULE: { strMeshFile += L"capsule.obj"; } break;
		case EColliderType::COLLIDER_PYRAMID: { strMeshFile += L"pyramid.obj"; this->SetupPlanes(); } break;

		default: break;
	}
	
	// Create and attach mesh
	this->m_pMesh = CEngine::GetInstance().GetObject3DManager().CreateMesh( strMeshFile, this->m_vPosition.x, this->m_vPosition.y, this->m_vPosition.z, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f );
	this->AttachChild( this->m_pMesh );

	// Initialise active status
	this->SetActive( true );
}

//**************************************************
//
// ~CCollider: Cleans up collider
//
// @author: Aiden Storey
//
//**************************************************
CCollider::~CCollider()
{}

//**************************************************
//
// GetActive: Returns if collider is active
//
// @author: Aiden Storey
//
// @returns		true if active otherwise false
//
//**************************************************
bool CCollider::GetActive() const
{
	return ( this->m_bActive );
}

//**************************************************
//
// SetActive: Sets if collider is active
//
// @author: Aiden Storey
//
// @param[in]	_kbActive			state for collider
//
//**************************************************
void CCollider::SetActive( const bool& _kbActive )
{
	this->m_bActive = _kbActive;
	this->m_pMesh->SetVisible( _kbActive );
}

//**************************************************
//
// CheckCollision: Checks point collision against current type of collider
//
// @author: Aiden Storey
//
// @param[in]	_vPosition			point to check for collision
//
// @returns		new position based on collision with collider
//
//**************************************************
D3DXVECTOR3 CCollider::CheckCollision( const D3DXVECTOR3& _vPosition )
{
	// Initialse return
	D3DXVECTOR3 vReturn{ _vPosition };

	// Set return based on collision type
	switch( this->m_eType )
	{
		case EColliderType::COLLIDER_SPHERE: { vReturn = this->CheckCollisionSphere( _vPosition ); } break;
		case EColliderType::COLLIDER_CAPSULE: { vReturn = this->CheckCollisionCapsule( _vPosition ); } break;
		case EColliderType::COLLIDER_PYRAMID: { vReturn = this->CheckCollisionPyramid( _vPosition ); } break;

		default: break;
	}

	return ( vReturn );
}

//**************************************************
//
// CheckCollisionSphere: Checks point collision against sphere collider
//
// @author: Aiden Storey
//
// @param[in]	_vPosition			point to check for collision
//
// @returns		new position based on collision with collider
//
//**************************************************
D3DXVECTOR3 CCollider::CheckCollisionSphere( const D3DXVECTOR3& _vPosition )
{
	// Initalise return
	D3DXVECTOR3 vReturn{ _vPosition };

	// If the passed position is in the sphere
	if( Math::DistanceBetween( this->m_vPosition, _vPosition ) < this->m_fRadius )
	{
		// Calculate the direction from center to position and normalize
		D3DXVECTOR3 vDirection( _vPosition - this->m_vPosition );
		D3DXVec3Normalize( &vDirection, &vDirection );

		// Set return value to be position pushed to edge of the sphere
		vReturn = this->m_vPosition + vDirection * this->m_fRadius;
	}

	return ( vReturn );
}

//**************************************************
//
// CheckCollisionCapsule: Checks point collision against capsule collider
//
// @author: Aiden Storey
//
// @param[in]	_vPosition			point to check for collision
//
// @returns		new position based on collision with collider
//
//**************************************************
D3DXVECTOR3 CCollider::CheckCollisionCapsule( const D3DXVECTOR3& _vPosition )
{
	// Initialse return
	D3DXVECTOR3 vReturn = _vPosition;

	// Calculate vector from point a of capsule to position
	D3DXVECTOR3 vAP( _vPosition - this->m_vCapsuleA );
	
	// Calculate dot product of capsule vector and vector to position
	float fDot = D3DXVec3Dot( &this->m_vCapsuleU, &vAP );
	
	// Calculate length of capsule vector and its square
	float fLength =	D3DXVec3Length( &this->m_vCapsuleU );
	float fLengthSq = fLength * fLength;
	
	// Calculate point on line
	D3DXVECTOR3 vW( vAP - ( this->m_vCapsuleU * ( fDot / fLengthSq ) ) );	
	D3DXVECTOR3 vPointOnLine( _vPosition - vW );

	// Calculte lengths of vectors
	float fLengthA = D3DXVec3Length( &this->m_vCapsuleA );
	float fLengthB = D3DXVec3Length( &this->m_vCapsuleB );
	float fLengthP = D3DXVec3Length( &vPointOnLine );

	// If point on line isn't within the ends
	if( fLengthP >= min( fLengthA, fLengthB ) )
	{
		// Calculate distance to each end
		float fDistA = Math::DistanceBetween( vPointOnLine, this->m_vCapsuleA );
		float fDistB = Math::DistanceBetween( vPointOnLine, this->m_vCapsuleB );
	
		// Set Set point on line to be closest end
		vPointOnLine = ( fDistA < fDistB ? this->m_vCapsuleA : this->m_vCapsuleB );
	}

	// If the passed position is in the capsule
	if( Math::DistanceBetween( vPointOnLine, _vPosition ) < this->m_fRadius )
	{
		// Calculate the direction from center to position and normalize
		D3DXVECTOR3 vDirection( _vPosition - vPointOnLine );
		D3DXVec3Normalize( &vDirection, &vDirection );

		// Set return value to be position pushed to edge of the capsule
		vReturn = vPointOnLine + vDirection * this->m_fRadius;
	}

	return ( vReturn );
}

//**************************************************
//
// CheckCollisionPyramid: Checks point collision agains pyramic collider
//
// @author: Aiden Storey
//
// @param[in]	_vPosition			point to check for collision
//
// @returns		new position based on collision with collider
//
//**************************************************
D3DXVECTOR3 CCollider::CheckCollisionPyramid( const D3DXVECTOR3& _vPosition )
{
	D3DXVECTOR3 vReturn = _vPosition;
	
	//if( _vPosition.y >= this->m_vPosition.y)
	{
		bool bInside{ true };

		float fClosest = D3D10_FLOAT32_MAX;
		D3DXVECTOR3 vClosest;

		for( auto& aPlane : this->m_PlaneList )
		{
			bInside &= Math::CalculateSideOfPlane( aPlane, _vPosition ) == Math::BEHIND_PLANE;

			D3DXVECTOR3 vPointOnPlane;
			float fDistance = std::abs( Math::DistancePointToPlane( aPlane, _vPosition, &vPointOnPlane ) );
		
			if( fDistance < fClosest )
			{
				fClosest = fDistance;
				vClosest = vPointOnPlane;
			}			
		}

		if( bInside )
		{
			if( vClosest.y >= this->m_vPosition.y )
			{
				vReturn = vClosest;
			}
		}
	}

	return ( vReturn );
}

//**************************************************
//
// SetupPlanes: Creates planes for pyramid collision
//
// @author: Aiden Storey
//
//**************************************************
void CCollider::SetupPlanes()
{
	const int iPointCount = 5;
	const D3DXVECTOR3 vPoints[ iPointCount ] =
	{
		{ this->m_vPosition.x + 1.0f, this->m_vPosition.y + 0.0f, this->m_vPosition.z + 1.0f },
		{ this->m_vPosition.x + 0.0f, this->m_vPosition.y + 2.0f, this->m_vPosition.z + 0.0f },
		{ this->m_vPosition.x - 1.0f, this->m_vPosition.y + 0.0f, this->m_vPosition.z + 1.0f },
		{ this->m_vPosition.x - 1.0f, this->m_vPosition.y + 0.0f, this->m_vPosition.z - 1.0f },
		{ this->m_vPosition.x + 1.0f, this->m_vPosition.y + 0.0f, this->m_vPosition.z - 1.0f },
	};

	const int iPlaneCount = 4;
	const int iPointPerPlane = 3;
	const int iAccessors[ iPlaneCount ][ iPointPerPlane ] =
	{
		{ 0, 1, 2 },
		{ 3, 1, 4 },
		{ 2, 1, 3 },
		{ 1, 0, 4 },
		//{ 4, 0, 3 },
	};

	for( int i = 0; i < iPlaneCount; ++i )
	{
		D3DXVECTOR3 vA( vPoints[ iAccessors[ i ][ 1 ] ] - vPoints[ iAccessors[ i ][ 0 ] ] );
		D3DXVECTOR3 vB( vPoints[ iAccessors[ i ][ 2 ] ] - vPoints[ iAccessors[ i ][ 0 ] ] );

		D3DXVECTOR3 vNormal;
		D3DXVec3Cross( &vNormal, &vA, &vB );

		this->m_PlaneList.push_back( { vPoints[ iAccessors[ i ][ 0 ] ], vNormal } );
	}
}