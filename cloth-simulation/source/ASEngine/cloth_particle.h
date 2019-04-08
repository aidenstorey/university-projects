//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: cloth_particle.h
// Description	: Declaration for tagClothParticle
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

#ifndef GUARD_AIDEN_STOREY_CLOTH_PARTICLE_H_201410091636
#define GUARD_AIDEN_STOREY_CLOTH_PARTICLE_H_201410091636

// Library Includes
#include <array>
#include <D3DX10.h>
#include <vector>

// Constants
enum EStructuralType
{
	STRUCTURE_LEFT = 0,
	STRUCTURE_UP,
	STRUCTURE_RIGHT,
	STRUCTURE_DOWN,

	STRUCTURE_MAX = 4,
	STRUCTURE_NONE,
};

// Prototypes
typedef struct tagClothLink TClothLink;
typedef struct tagGridPosition TGridPosition;

typedef struct tagClothParticle
{
	tagClothParticle( D3DXVECTOR3 _position, float _mass, bool _static ) :
		CurrentPosition( _position ), PreviousPosition( _position ), Mass( _mass ), Static( _static ), Heat( 0.0f )
	{
		for( auto& aLink : this->Structral )
		{
			aLink = nullptr;
		}
	}

	tagClothParticle( float _x, float _y, float _z, float _mass, bool _static ) :
		CurrentPosition( _x, _y, _z ), PreviousPosition( _x, _y, _z ), Mass( _mass ), Static( _static ), Heat( 0.0f )
	{
		for( auto& aLink : this->Structral )
		{
			aLink = nullptr;
		}
	}

	void AddLink( TClothLink* _pLink, EStructuralType _eType );
	void RemoveLink( TClothLink* _pLink );
	void CalculateNormal();

	D3DXVECTOR3						CurrentPosition;
	D3DXVECTOR3						PreviousPosition;
	D3DXVECTOR3						Normal;

	float							Mass;
	float							Heat;
	bool							Static;

	std::array< TClothLink*, 4 >	Structral;
	std::vector< TClothLink* >		Links;

	TGridPosition*					GridPosition;

} TClothParticle;

#endif // GUARD_AIDEN_STOREY_CLOTH_PARTICLE_H_201410091636