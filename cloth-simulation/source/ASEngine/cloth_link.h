//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: cloth_link.h
// Description	: Declaration for tagClothLink
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

#ifndef GUARD_AIDEN_STOREY_CLOTH_LINK_H_201410091642
#define GUARD_AIDEN_STOREY_CLOTH_LINK_H_201410091642

// Local Includes
#include "cloth_particle.h"

// Constants
enum class ELinkType
{
	STRUCTURAL,
	SHEER,
	BEND,
};

// Implementation
typedef struct tagClothLink
{
	tagClothLink( ELinkType _eType, TClothParticle* _pParticleA, TClothParticle* _pParticleB, float _fRestingDistance ) :
		Type( _eType ), ParticleA( _pParticleA ), ParticleB( _pParticleB ), RestingDistance( _fRestingDistance ), Destroyed( false )
	{}
	
	TClothParticle* GetOppositeEnd( TClothParticle* _pParticle )
	{
		return ( ParticleA == _pParticle ? ParticleB : ParticleA );
	}

	void Destroy()
	{
		if( Type == ELinkType::STRUCTURAL )
		{
			for( auto& aLinkA : this->ParticleA->Links )
			{
				TClothParticle* pOppositeA = aLinkA->GetOppositeEnd( this->ParticleA );
				for( auto& aLinkB : this->ParticleB->Links )
				{
					if( pOppositeA == aLinkB->GetOppositeEnd( this->ParticleB ) )
					{
						if( aLinkA->Type != ELinkType::STRUCTURAL )
						{
							aLinkA->Destroyed = true;
						}

						if( aLinkB->Type != ELinkType::STRUCTURAL )
						{
							aLinkB->Destroyed = true;
						}
					}
				}
			}
		}

		this->Destroyed = true;
	}

	TClothParticle* ParticleA;
	TClothParticle* ParticleB;

	ELinkType		Type;
	float			RestingDistance;
	bool			Destroyed;

} TClothLink;

#endif // GUARD_AIDEN_STOREY_CLOTH_LINK_H_201410091642