//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: cloth_particle.cpp
// Description	: Implementation for tagClothParticle
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// Local Includes
#include "cloth_link.h"

// This Include
#include "cloth_particle.h"

// Implementation
//**************************************************
//
// AddLink: Adds link to appropriate particle lists
//
// @author: Aiden Storey
//
// @param[in]	_pLink			link to add
// @param[in]	_eType			type of link
//
//**************************************************
void tagClothParticle::AddLink( TClothLink* _pLink, EStructuralType _eType )
{
	// If type indicate it is structural
	if( _eType != STRUCTURE_NONE )
	{
		// Set appropriate structural link
		this->Structral[ _eType ] = _pLink;
	}

	// Push link onto list
	this->Links.push_back( _pLink );
}

//**************************************************
//
// RemoveLink: Removes passed link from lists
//
// @author: Aiden Storey
//
// @param[in]	_pLink			link to remove
//
//**************************************************
void tagClothParticle::RemoveLink( TClothLink* _pLink )
{
	// Iterate over all links
	std::vector< TClothLink* >::iterator iterDelete = this->Links.begin();
	while( iterDelete != this->Links.end() )
	{
		// If link is the same as passed
		if( ( *iterDelete ) == _pLink )
		{
			// Remove from list
			iterDelete = Links.erase( iterDelete );
			break;
		}
		iterDelete++;
	}

	// If link is structural
	if( _pLink->Type == ELinkType::STRUCTURAL )
	{
		// Iterate over structural links
		for( auto& aLink : this->Structral )
		{
			// Set to null if found
			if( aLink == _pLink )
			{
				aLink = nullptr;
			}
		}
	}
}

//**************************************************
//
// CalculateNormal: Calculates the normal of particle
//
// @author: Aiden Storey
//
//**************************************************
void tagClothParticle::CalculateNormal()
{
	// Initialise normal and count
	D3DXVECTOR3 vNormal{ 0.0f, 0.0f, 0.0f };
	int iNormalsCalculated = 0;

	// Iterate over all links
	for( unsigned iLinkA = STRUCTURE_LEFT; iLinkA < STRUCTURE_MAX; ++iLinkA )
	{
		// Store next link
		int iLinkB = ( iLinkA + 1 ) % STRUCTURE_MAX;

		// If the current and next link are both valid
		if( this->Structral[ iLinkA ] != nullptr && this->Structral[ iLinkB ] != nullptr )
		{
			// Get the vector to each opposite particle
			D3DXVECTOR3 vA = this->Structral[ iLinkA ]->GetOppositeEnd( this )->CurrentPosition - this->CurrentPosition;
			D3DXVECTOR3 vB = this->Structral[ iLinkB ]->GetOppositeEnd( this )->CurrentPosition - this->CurrentPosition;

			// Calculate cross product between vectors
			D3DXVECTOR3 vCross;
			D3DXVec3Cross( &vCross, &vA, &vB );

			// Add to normal and increment count
			vNormal += vCross;
			iNormalsCalculated++;
		}
	}

	// If normals have been calculated
	if( iNormalsCalculated != 0 )
	{
		// Divide by normals calculated for average
		vNormal /= static_cast< float >( iNormalsCalculated );
	}

	// Set normal to calculated value
	this->Normal = vNormal;
}