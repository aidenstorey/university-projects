#ifndef GUARD_AIDEN_STOREY_SELF_INDERSECTION_GRID_H_2014112024
#define GUARD_AIDEN_STOREY_SELF_INDERSECTION_GRID_H_2014112024

// Library Includes
#include <D3DX10.h>
#include <iostream>
#include <fstream>
#include <vector>

// Local Includes
#include "cloth_particle.h"

// Constants
const int g_GridWidth = 60;
const int g_GridHeight = 40;
const int g_GridDepth = 60;

const float g_GridSpacing = 0.2f;

const float g_GridWorldWidth = g_GridWidth * g_GridSpacing;
const float g_GridWorldHeight = g_GridHeight * g_GridSpacing;
const float g_GridWorldDepth = g_GridDepth * g_GridSpacing;

const float g_GridWidthOffset = ( g_GridWorldWidth * 0.5f );
const float g_GridHeightOffset = ( g_GridWorldHeight * 0.5f );
const float g_GridDepthOffset = ( g_GridWorldDepth * 0.5f );

// Prototypes
typedef struct tagGridAccessor
{
	tagGridAccessor() {}

	tagGridAccessor( int _iX, int _iY, int _iZ ) :
		x( _iX ), y( _iY ), z( _iZ )
	{}

	int x;
	int y;
	int z;
} TGridAccessor;

typedef struct tagGridPosition
{
	tagGridPosition() {}

	void AddParticle( TClothParticle* _pParticle )
	{
		this->Particles.push_back( _pParticle );
	}

	void RemoveParticle( TClothParticle* _pParticle )
	{
		std::vector< TClothParticle* >::iterator iterDelete = this->Particles.begin();
		while( iterDelete != this->Particles.end() )
		{
			// If link is the same as passed
			if( ( *iterDelete ) == _pParticle )
			{
				// Remove from list
				iterDelete = Particles.erase( iterDelete );
				break;
			}
			iterDelete++;
		}
	}

	std::vector< TClothParticle* >	Particles;
	TGridAccessor					Accessor;

} TGridPosition;

typedef struct tagSelfIntersectionGrid
{
	tagSelfIntersectionGrid() 
	{
		for( unsigned iX = 0; iX < g_GridWidth; ++iX )
		{
			for( unsigned iY = 0; iY < g_GridHeight; ++iY )
			{
				for( unsigned iZ = 0; iZ < g_GridDepth; ++iZ )
				{
					this->m_Grid[ iX ][ iY ][ iZ ].Accessor = TGridAccessor( iX, iY, iZ );
				}
			}
		}
	}

	bool OnGrid( int iX, int iY, int iZ )
	{
		return ( iX >= 0 && iX < g_GridWidth &&
				 iY >= 0 && iY < g_GridHeight &&
				 iZ >= 0 && iZ < g_GridDepth );
	}

	bool OnGrid( const TGridAccessor& _krAccessor )
	{
		return ( _krAccessor.x >= 0 && _krAccessor.x < g_GridWidth &&
				 _krAccessor.y >= 0 && _krAccessor.y < g_GridHeight &&
				 _krAccessor.z >= 0 && _krAccessor.z < g_GridDepth );
	}

	TGridPosition* GetGridPosition( const D3DXVECTOR3& _krWorldPosition )
	{
		TGridPosition* pReturn{ nullptr };

		D3DXVECTOR3 vAccessor( _krWorldPosition * g_GridSpacing );

		int iWidth = static_cast< int >( ( _krWorldPosition.x + g_GridWidthOffset ) / g_GridSpacing );
		int iHeight = static_cast< int >( ( _krWorldPosition.y + g_GridHeightOffset ) / g_GridSpacing );
		int iDepth = static_cast< int >( ( _krWorldPosition.z + g_GridDepthOffset ) / g_GridSpacing );

		if( this->OnGrid( iWidth, iHeight, iDepth ) )
		{
			pReturn = &this->m_Grid[ iWidth ][ iHeight ][ iDepth ];
		}

		return ( pReturn );		
	}

	TGridPosition* GetGridPosition( const TGridAccessor& _krAccessor )
	{
		TGridPosition* pReturn{ nullptr };

		if( this->OnGrid( _krAccessor ) )
		{
			pReturn = &this->m_Grid[ _krAccessor.x ][ _krAccessor.y ][ _krAccessor.z ];
		}

		return ( pReturn );
	}

	TGridPosition* GetGridPosition( const int& _krX, const int& _krY, const int& _krZ )
	{
		TGridPosition* pReturn{ nullptr };

		if( this->OnGrid( _krX, _krY, _krZ ) )
		{
			pReturn = &this->m_Grid[ _krX ][ _krY ][ _krZ ];
		}

		return ( pReturn );
	}

	void DebugOutput( std::string _strFileName )
	{
		std::ofstream file;
		file.open( _strFileName );

		int iCount = 0;

		for( unsigned iX = 0; iX < g_GridWidth; ++iX )
		{
			for( unsigned iY = 0; iY < g_GridHeight; ++iY )
			{
				for( unsigned iZ = 0; iZ < g_GridDepth; ++iZ )
				{
					if( this->m_Grid[ iX ][ iY ][ iZ ].Particles.size() > 0 )
					{
						file << "{" << iX << ", " << iY << ", " << iZ << "}:" << this->m_Grid[ iX ][ iY ][ iZ ].Particles.size() << "\n";
						iCount += this->m_Grid[ iX ][ iY ][ iZ ].Particles.size();
					}
				}
			}
		}

		file << "Total Particles: " << iCount << "\n";

		file.close();
	}

	TGridPosition	m_Grid[ g_GridWidth ][ g_GridHeight ][ g_GridDepth ];
} TSelfIntersectionGrid;

#endif // GUARD_AIDEN_STOREY_SELF_INDERSECTION_GRID_H_2014112024