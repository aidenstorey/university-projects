//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: cloth_simulation.cpp
// Description	: Implementation for CClothSimulation
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

// Fix windows include
#define NOMINMAX

// Library Includes
#include <algorithm>

// Local Includes
#include "engine.h"
#include "math_functions.h"
#include "safe_release.h"
#include "shader_id_list.h"

// This Include
#include "cloth_simulation.h"

// Implementation
//**************************************************
//
// CClothSimulation: Initialises cloth simulation
//
// @author: Aiden Storey
//
// @param[in]	_uRowCount			number of rows
// @param[in]	_uColCount			number of columns
// @param[in]	_uPinCount			number of rods
//
//**************************************************
CClothSimulation::CClothSimulation( const unsigned& _uRowCount, const unsigned& _uColCount, const unsigned& _uPinCount ) :
	m_RowCount( _uRowCount ),
	m_ColCount( _uColCount ),
	m_PinCount( _uPinCount )
{	
	// Store local copy of device and required from the engine 
	this->m_pDevice = &CEngine::GetInstance().GetDevice();
	this->m_pShader = reinterpret_cast< CShaderDefault3D* >( CEngine::GetInstance().GetShaderManager().GetShader( SHADER_DEFAULT3D ) );	 
	
	// Create link definitions
	this->m_LinkDefinitionList.push_back( { ELinkType::SHEER, 1, -1, this->m_SpacingDiagonal, false } );
	this->m_LinkDefinitionList.push_back( { ELinkType::STRUCTURAL, 1, 0, this->m_Spacing, true } );
	this->m_LinkDefinitionList.push_back( { ELinkType::BEND, 2, 0, this->m_SpacingDouble, false } );
	this->m_LinkDefinitionList.push_back( { ELinkType::SHEER, 1, 1, this->m_SpacingDiagonal, false } );
	this->m_LinkDefinitionList.push_back( { ELinkType::STRUCTURAL, 0, 1, this->m_Spacing, false } );
	this->m_LinkDefinitionList.push_back( { ELinkType::BEND, 0, 2, this->m_SpacingDouble, false } );

	// Create cloth and buffer
	this->SetupCloth();
	this->SetupLinkBuffer();

	// Initialise interaction type
	this->SetInteractionType( EInteractionType::GRAB );

	// Set iterations based on running mode
#ifdef _DEBUG
	this->m_uSatisfyIterationCount = 2;
#else
	this->m_uSatisfyIterationCount = 10;
#endif 
}

//**************************************************
//
// ~CClothSimulation: Cleans up cloth simulation
//
// @author: Aiden Storey
//
//**************************************************
CClothSimulation::~CClothSimulation()
{
	// Delete all particles
	for( auto& aParticle : this->m_ParticleList )
	{
		delete aParticle;
	}

	// Delete all active links
	for( auto& aLink : this->m_LinkList )
	{
		delete aLink;
	}

	ReleaseCOM( m_pLinkBuffer );
}

//**************************************************
//
// IsOnMesh: Returns if the passed variables are on the mesh
//
// @author: Aiden Storey
//
// @param[in]	_iRow			row of mesh
// @param[in]	_iCol			column of mesh
//
// @returns		true if the passed variables are on the mesh otherwise false
//
//**************************************************
bool CClothSimulation::IsOnMesh( int _iRow, int _iCol )
{
	return ( _iRow >= 0 && static_cast< unsigned >( _iRow ) < this->m_RowCount && _iCol >= 0 && static_cast< unsigned >( _iCol ) < this->m_ColCount );
}

//**************************************************
//
// SetInteractionType: Sets the interaction type for mouse
//
// @author: Aiden Storey
//
// @param[in]	_eType			type of interaction
//
//**************************************************
void CClothSimulation::SetInteractionType( const EInteractionType& _eType )
{
	// Set interaction function based on passed type
	switch( _eType )
	{
		case EInteractionType::BURN: { this->m_pInteractionFunction = &CClothSimulation::InteractionBurn; } break;
		case EInteractionType::CUT: { this->m_pInteractionFunction = &CClothSimulation::InteractionCut; } break;
		case EInteractionType::GRAB: { this->m_pInteractionFunction = &CClothSimulation::InteractionGrab; } break;
		default: break;
	};

	// Ensure nothing is selected
	this->m_pSelectedParticle = nullptr;
}

//**************************************************
//
// SetTearingEnabled: Sets if tearing is possible
//
// @author: Aiden Storey
//
// @param[in]	_kbTearingEnabled			state of tearing
//
//**************************************************
void CClothSimulation::SetTearingEnabled( const bool& _kbTearingEnabled )
{
	this->m_bTearingEnabled = _kbTearingEnabled;
}

//**************************************************
//
// SetSlidingEnabled: Sets if sliding is possible
//
// @author: Aiden Storey
//
// @param[in]	_kbSlidingEnabled			state of sliding
//
//**************************************************
void CClothSimulation::SetSlidingEnabled( const bool& _kbSlidingEnabled )
{
	this->m_bSlideEnabled = _kbSlidingEnabled;
}

//**************************************************
//
// SetWindEnabled: Sets if wind is applied
//
// @author: Aiden Storey
//
// @param[in]	_kbWindEnabled			state of wind 
//
//**************************************************
void CClothSimulation::SetWindEnabled( const bool& _kbWindEnabled )
{
	this->m_bWindEnabled = _kbWindEnabled;
}

//**************************************************
//
// SetSelfIntersectingEnabled: Sets if self colliding is enabled
//
// @author: Aiden Storey
//
// @param[in]	_kbEnabled			state of wind 
//
//**************************************************
void CClothSimulation::SetSelfIntersectingEnabled( const bool& _kbEnabled )
{
	this->m_bSelfIntersectionEnabled = _kbEnabled;
}

//**************************************************
//
// SetActiveCollider: Sets the active collider to check collisions
//
// @author: Aiden Storey
//
// @param[in]	_pCollider			collider to collide with
//
//**************************************************
void CClothSimulation::SetActiveCollider( CCollider* _pCollider )
{
	this->m_pActiveCollider = _pCollider;
}

//**************************************************
//
// SetWindOrigin: Sets the origin point of wind
//
// @author: Aiden Storey
//
// @param[in]	_krOrigin			origin for wind vector
//
//**************************************************
void CClothSimulation::SetWindOrigin( const D3DXVECTOR3& _krOrigin )
{
	this->m_vWindOrigin = _krOrigin;
}

//**************************************************
//
// SetWindDirection: Sets the direction wind is applied
//
// @author: Aiden Storey
//
// @param[in]	_kvDirection			direction for wind vector
//
//**************************************************
void CClothSimulation::SetWindDirection( const D3DXVECTOR3& _kvDirection )
{
	this->m_vWindDirection = _kvDirection;
}

//**************************************************
//
// SetWindSpeed: Sets speed for wind
//
// @author: Aiden Storey
//
// @param[in]	_kfSpeed			percent of max speed
//
//**************************************************
void CClothSimulation::SetWindSpeed( const float& _kfSpeed )
{
	this->m_fWindSpeed = ( this->m_MaxWindSpeed / 100.0f )  * _kfSpeed;
}

//**************************************************
//
// SetWindConeSize: Sets the angle for wind cone
//
// @author: Aiden Storey
//
// @param[in]	_kfSize			percent of size 
//
//**************************************************
void CClothSimulation::SetWindConeSize( const float& _kfSize )
{
	this->m_fWindConeSize = ( this->m_MaxWindConeSize / 100.0f )  * _kfSize;
}

//**************************************************
//
// SetupCloth: Creates particles and links for cloth
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::SetupCloth()
{	
	// Calculate offsets
	float fRowOffset = ( this->m_RowCount * this->m_Spacing ) * 0.5f;
	float fColOffset = ( this->m_ColCount * this->m_Spacing ) * 0.5f;

	// Calculate how often pins occur
	unsigned uPinFrequency = this->m_ColCount / this->m_PinCount;

	// Fill the default cloth particles
	for( unsigned row = 0; row < this->m_RowCount; ++row )
	{
		for( unsigned col = 0; col < this->m_ColCount; ++col )
		{
			bool bStatic = row == 0 && col % uPinFrequency == 0;

			TClothParticle* pParticle = new TClothParticle( ( this->m_Spacing * col ) - fColOffset, -( this->m_Spacing * row ) + fRowOffset, 0.0f, this->m_ClothMass, bStatic );
			this->m_ParticleList.push_back( pParticle );

			TGridPosition* pGridPosition = this->m_SelfIntersectionGrid.GetGridPosition( pParticle->CurrentPosition );
			if( pGridPosition != nullptr )
			{
				pGridPosition->AddParticle( pParticle );
			}
			pParticle->GridPosition = pGridPosition;

			if( bStatic )
			{
				this->m_StaticParticles.push_back( pParticle );
			}
		}
	}
		
	// Enable cloth to move in three dimensions
	this->m_ParticleList[ rand() % ( this->m_ParticleList.size() - this->m_ColCount ) + this->m_ColCount ]->CurrentPosition.z = static_cast< float >( rand() ) / ( RAND_MAX * 10.0f );
	
	// Fill the default cloth link 
	for( unsigned iRow = 0; iRow < this->m_RowCount; ++iRow )
	{
		for( unsigned iCol = 0; iCol < this->m_ColCount; ++iCol )
		{
			this->AddParticleLinks( iRow, iCol );
		}
	}

	// Shuffle links for update
	std::random_shuffle( this->m_LinkList.begin(), this->m_LinkList.end() );
	
	// Set number of links to draw
	this->m_LinkDrawCount = this->m_LinkList.size() * 2;
}

//**************************************************
//
// SetupLinkBuffer: Creates vertex buffer for links
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::SetupLinkBuffer()
{
	// Fill link vertex buffer description
	D3D10_BUFFER_DESC lvbDesc;
	lvbDesc.Usage = D3D10_USAGE_DYNAMIC;
	lvbDesc.ByteWidth = sizeof( VDefault3D ) * m_LinkDrawCount;
	lvbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	lvbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	lvbDesc.MiscFlags = 0;

	// Create link vertex buffer
	this->m_pDevice->CreateBuffer( &lvbDesc, 0, &this->m_pLinkBuffer ); 
}

//**************************************************
//
// AddParticleLinks: Creates appropriate link for particles
//
// @author: Aiden Storey
//
// @param[in]	_iRow			row of particle
// @param[in]	_iCol			column of particle
//
//**************************************************
void CClothSimulation::AddParticleLinks( const int& _iRow, const int& _iCol )
{
	// Get the current particle 
	TClothParticle* pParticleA = this->m_ParticleList[ ( _iRow * m_ColCount ) + _iCol ];

	// Iterate over the link definitions
	for( auto& aDef : this->m_LinkDefinitionList )
	{
		// Get the row and column of the new particle
		int iRow = _iRow + aDef.OffsetX;
		int iCol = _iCol + aDef.OffsetY;

		// Ensure that the particle is valid
		if( this->IsOnMesh( iRow, iCol ) )
		{
			// Get the particle
			TClothParticle* pParticleB = this->m_ParticleList[ ( iRow * m_ColCount ) + iCol ];
			
			// Create a new link between the particles
			TClothLink* pLink = new TClothLink( aDef.Type, pParticleA, pParticleB, aDef.Spacing );
			
			// Initialise the type structural type of the link
			EStructuralType eTypeA{ STRUCTURE_NONE };
			EStructuralType eTypeB{ STRUCTURE_NONE };

			// If the link is a structure link
			if( aDef.Type == ELinkType::STRUCTURAL )
			{
				// Put link in structural list
				this->m_StructuralLinkList.push_back( pLink );

				// If the link is horizontal
				if( aDef.Horizontal == true )
				{
					// Set structural types to be left/right
					eTypeA = STRUCTURE_LEFT;
					eTypeB = STRUCTURE_RIGHT;
				}
				else
				{
					// Set structural types to be up/down
					eTypeA = STRUCTURE_UP;
					eTypeB = STRUCTURE_DOWN;
				}
			}

			// Add link to particles
			pParticleA->AddLink( pLink, eTypeA );
			pParticleB->AddLink( pLink, eTypeB );

			// Put link in complete list
			this->m_LinkList.push_back( pLink );
		}
	}
}

//**************************************************
//
// MapLinkBuffer: Maps links to buffer for drawing
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::MapLinkBuffer()
{
	// Map vertex buffer
	VDefault3D* pVertex = nullptr;
	this->m_pLinkBuffer->Map( D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast< void** >( &pVertex ) );

	// Count number of vertices
	unsigned uVertexNum = 0;
	
	// Iterate over list of active links
	for( unsigned i = 0; i < this->m_LinkList.size(); ++i )
	{
		// Update position and color for first point
		pVertex[ uVertexNum ].pos = this->m_LinkList[ i ]->ParticleA->CurrentPosition;
		pVertex[ uVertexNum ].color = this->m_ClothColorBase + this->m_ClothColorStep *  this->m_LinkList[ i ]->ParticleA->Heat;
		uVertexNum++;

		// Update position and color for second point
		pVertex[ uVertexNum ].pos = this->m_LinkList[ i ]->ParticleB->CurrentPosition;
		pVertex[ uVertexNum ].color = this->m_ClothColorBase + this->m_ClothColorStep *  this->m_LinkList[ i ]->ParticleB->Heat;;
		uVertexNum++;
	}

	// Set number of links to be drawn
	this->m_LinkDrawCount = uVertexNum;

	// Unmap vertex buffer
	this->m_pLinkBuffer->Unmap();
}

//**************************************************
//
// ReleaseCloth: Sets the static particles to non-static
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::ReleaseCloth()
{
	for( auto& aParticle : this->m_StaticParticles )
	{
		aParticle->Static = false;
	}
	this->m_StaticParticles.clear();
}

//**************************************************
//
// ClosestToMouse: Finds the closest particle to the current mouse position
//
// @author: Aiden Storey
//
// @param[in]	_kbStatic			if selecting static particles
//
// @returns		closest point to mouse, if none found nullptr
//
//**************************************************
TClothParticle* CClothSimulation::ClosestToMouse( const bool& _kbStatic )
{
	// Initialise closest
	TClothParticle* tClosest{ nullptr };

	// Get the clamped world position of mouse
	D3DXVECTOR2 vMousePosition = CEngine::GetInstance().GetInputManager().MouseClampedWorldPosition2D();

	// Create the WVP
	D3DXMATRIX matWVP = this->m_matWorld * CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetCamera().GetViewProjMatrix();

	// Initailise smallest distance 
	float fSmallestDistance = this->m_GrabDistance;

	// Iterate over particles
	for( auto& aParticle : this->m_ParticleList )
	{
		// If the particle is of the same static type as parameter
		if( aParticle->Static == _kbStatic )
		{
			// Transform the coordinate to screen space
			D3DXVECTOR3 vOut;
			D3DXVec3TransformCoord( &vOut, &aParticle->CurrentPosition, &matWVP );

			// Calculate the distance between point in screen space and mouse
			float fDistanceBetween = Math::DistanceBetween( D3DXVECTOR2( vOut.x, vOut.y ), vMousePosition );

			// If the distance is smaller than the current
			if( fDistanceBetween < fSmallestDistance )
			{
				// Update the current to be this particle
				tClosest = aParticle;
				fSmallestDistance = fDistanceBetween;
			}
		}
	}

	return( tClosest );
}

//**************************************************
//
// InteractionGrab: Moves cloth via mouse interaciton
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::InteractionGrab()
{
	// If the left mouse button is pressed
	if( CEngine::GetInstance().GetInputManager().IsPressed( VK_LBUTTON ) )
	{
		// Get closest non-static particle
		this->m_pSelectedParticle = this->ClosestToMouse( false );
	}
	// If the left mouse button is down and there is a selected particle
	else if( CEngine::GetInstance().GetInputManager().IsDown( VK_LBUTTON ) && this->m_pSelectedParticle != nullptr )
	{
		// Get the clamped world position of mouse
		D3DXVECTOR2 vMousePosition = CEngine::GetInstance().GetInputManager().MouseClampedWorldPosition2D();

		// Create the WVP and its determinant
		D3DXMATRIX matWVP = this->m_matWorld * CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetCamera().GetViewProjMatrix();
		float fWVPDet = D3DXMatrixDeterminant( &matWVP );

		// Create the inverse WVP
		D3DXMATRIX matInvWVP;
		D3DXMatrixInverse( &matInvWVP, &fWVPDet, &matWVP );

		// Transform selected particles position to screen space
		D3DXVECTOR3 vPosToScreen;
		D3DXVec3TransformCoord( &vPosToScreen, &this->m_pSelectedParticle->CurrentPosition, &matWVP );		

		// Transform mouse position with selcted particles z to world space
		D3DXVECTOR3 vScreenToPos;
		D3DXVec3TransformCoord( &vScreenToPos, &D3DXVECTOR3( vMousePosition.x, vMousePosition.y, vPosToScreen.z ), &matInvWVP );

		// If moving isn't causing a collision
		if( !this->m_bSelfIntersectionEnabled || ( this->m_bSelfIntersectionEnabled && 
			!this->IsSelfCollision( this->m_pSelectedParticle, vScreenToPos - this->m_pSelectedParticle->CurrentPosition ) ) )
		{
			// Set the selected particles current position to mouse position in world
			this->m_pSelectedParticle->CurrentPosition = vScreenToPos;
		}

		// Sets currently selected particle to static
		if( CEngine::GetInstance().GetInputManager().IsPressed( VK_RBUTTON ) )
		{
			this->m_pSelectedParticle->Static = true;
			this->m_pSelectedParticle = nullptr;
		}
	}
	// Ensure no particle is selected 
	else
	{
		this->m_pSelectedParticle = nullptr;
	}
}

//**************************************************
//
// InteractionCut: Cuts cloth via mouse interaction
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::InteractionCut()
{
	// If the left mouse button is pressed
	if( CEngine::GetInstance().GetInputManager().IsPressed( VK_LBUTTON ) )
	{
		this->m_vTearMousePosition = CEngine::GetInstance().GetInputManager().MouseClampedWorldPosition2D();
	}
	// If the left mouse button is down
	else if( CEngine::GetInstance().GetInputManager().IsDown( VK_LBUTTON ) )
	{
		// Get the clamped world position of mouse
		D3DXVECTOR2 vMousePosition = CEngine::GetInstance().GetInputManager().MouseClampedWorldPosition2D();

		// Create the WVP
		D3DXMATRIX matWVP = this->m_matWorld * CEngine::GetInstance().GetSceneManager().GetCurrentScene()->GetCamera().GetViewProjMatrix();

		// Iterate over all of the links
		for( auto& aLink : this->m_LinkList )
		{
			// Calculate particle a's screen coord
			D3DXVECTOR3 vOutA;
			D3DXVec3TransformCoord( &vOutA, &aLink->ParticleA->CurrentPosition, &matWVP );

			// Calculate particle b's screen coord
			D3DXVECTOR3 vOutB;
			D3DXVec3TransformCoord( &vOutB, &aLink->ParticleB->CurrentPosition, &matWVP );

			// Create 2D vector for each coord
			D3DXVECTOR2 vScreenPosA( vOutA.x, vOutA.y );
			D3DXVECTOR2 vScreenPosB( vOutB.x, vOutB.y );

			// Check if current mouse line and link line intersect
			if( Math::IsLineSegmentIntersection( this->m_vTearMousePosition, vMousePosition, vScreenPosA, vScreenPosB ) )
			{
				// Destroy the link and signal cleanup
				aLink->Destroy();
				this->m_bCleanup = true;
			}
		}

		// Update the tear position to the current mouse position
		this->m_vTearMousePosition = vMousePosition;
	}
}

//**************************************************
//
// InteractionBurn: Applies heat to cloth via mouse interaction
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::InteractionBurn()
{
	// If the left mouse button is down
	if( CEngine::GetInstance().GetInputManager().IsDown( VK_LBUTTON ) )
	{
		// Get closest non-static particle
		TClothParticle* pParticle = this->ClosestToMouse( false );
		
		// If particle found
		if( pParticle != nullptr )
		{
			// Add heat to the particle and signal burnint
			pParticle->Heat += this->m_TouchHeat;
			this->m_bBurning = true;
		}
	}
}

//**************************************************
//
// BurnLinks: Spreads heat and destroys particles
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::BurnLinks()
{
	// Iterate over all of the links
	for( auto& aLink : this->m_LinkList )
	{
		// If passes the chance to transfer
		if( rand() % this->m_HeatTrasnferChance == 0 )
		{
			// Calculate heat given off by each particle
			float fTrasnferA = aLink->ParticleA->Heat * this->m_HeatTransferRate;
			float fTransferB = aLink->ParticleB->Heat * this->m_HeatTransferRate;

			// Apply heat to opposite ends particle
			aLink->ParticleA->Heat += fTransferB;
			aLink->ParticleB->Heat += fTrasnferA;

			// If the heat from both ends of the link exceed the heat to burn link
			if( aLink->ParticleA->Heat > this->m_BurnHeat && aLink->ParticleB->Heat > this->m_BurnHeat )
			{
				// Destroy the link and signal cleanup
				aLink->Destroy();
				this->m_bCleanup = true;
			}
		}
	}
}

//**************************************************
//
// CheckCollisions: Checks particle collision with collider
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::CheckCollisions()
{
	// Iterate over all particles
	for( auto& aParticle : this->m_ParticleList )
	{
		// If the particle isn't static
		if( !aParticle->Static )
		{
			// Set the current position to be return from active colliders collision detection
			aParticle->CurrentPosition = this->m_pActiveCollider->CheckCollision( aParticle->CurrentPosition );
		}
	}
}

//**************************************************
//
// CheckSelfCollision: Checks particle collision with the cloth
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::CheckSelfCollision()
{
	// Iterate over all of the particles
	for( auto& aParticle : this->m_ParticleList )
	{
		// If particle isn't static
		if( !aParticle->Static )
		{
			// Calculate change vector and undo change if it caused an intersection
			D3DXVECTOR3 vChange{ aParticle->CurrentPosition - aParticle->PreviousPosition };
			if( this->IsSelfCollision( aParticle, vChange ) )
			{
				aParticle->CurrentPosition -= vChange;
			}
		}
	}
}

//**************************************************
//
// IsSelfCollision: Checks if particle is intersecting with the rest of the cloth
//
// @author: Aiden Storey
//
// @param[in]	_pParticle			particle to check
// @param[in]	_krOffset			its movement offset
//
// @returns		true if intersecting with cloth
//
//**************************************************
bool CClothSimulation::IsSelfCollision( TClothParticle* _pParticle, const D3DXVECTOR3& _krOffset )
{
	// Get particles grid position and check if it's not null
	TGridPosition* pGridPosition = _pParticle->GridPosition;
	if( pGridPosition != nullptr )
	{
		// Calclulate two points of the triangle
		D3DXVECTOR3 vP0{ _pParticle->CurrentPosition };
		D3DXVECTOR3 vP1{ _pParticle->CurrentPosition + _krOffset };

		// Iterate over particles structural links
		for( auto& aLink : _pParticle->Structral )
		{
			// If the link isn't null
			if( aLink != nullptr )
			{
				// Get the third point of the triangle
				D3DXVECTOR3 vP2{ aLink->GetOppositeEnd( _pParticle )->CurrentPosition };

				// Calculate the edge vectors
				D3DXVECTOR3 vU( vP1 - vP0 );
				D3DXVECTOR3 vV( vP2 - vP0 );

				// Calculate the normal
				D3DXVECTOR3 vNormal;
				D3DXVec3Cross( &vNormal, &vU, &vV );

				// Iterate over the adjacent grid positions
				for( int iX = pGridPosition->Accessor.x - 1; iX <= pGridPosition->Accessor.x + 1; ++iX )
				{
					for( int iY = pGridPosition->Accessor.y - 1; iY <= pGridPosition->Accessor.y + 1; ++iY )
					{
						for( int iZ = pGridPosition->Accessor.z - 1; iZ <= pGridPosition->Accessor.z + 1; ++iZ )
						{
							// Get the adjacent grid position and check if its valid
							TGridPosition* pCheckPosition = this->m_SelfIntersectionGrid.GetGridPosition( iX, iY, iZ );
							if( pCheckPosition != nullptr )
							{
								// Iterate over the grid positions particles
								for( auto& aParticles : pCheckPosition->Particles )
								{
									// Iterate over the particles structural links
									for( auto& aCheckLink : aParticles->Structral )
									{
										// If the link isn't null and isn't connected to the initial link
										if( aCheckLink != nullptr && aCheckLink->GetOppositeEnd( aParticles ) != _pParticle )
										{
											// Create the line origin and destination
											D3DXVECTOR3 vO = aCheckLink->ParticleA->CurrentPosition;
											D3DXVECTOR3 vD = aCheckLink->ParticleB->CurrentPosition;

											// Calculate the side of the plane that the particles are on
											Math::ESideOfPlane eA = Math::CalculateSideOfPlane( Math::TPlane( vP0, vNormal ), vO );
											Math::ESideOfPlane eB = Math::CalculateSideOfPlane( Math::TPlane( vP0, vNormal ), vD );
											
											// If the link is intersecting the plane
											if( eA != Math::ON_PLANE && eB != Math::ON_PLANE && eA != eB )
											{	
												// Check if link is intersecting the triangle
												if( Math::IsLineTriangleIntersection( vO, vD, vP0, vP1, vP2 ) )
												{
													// Return true if it is
													return ( true );
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return ( false );
}

//**************************************************
//
// KeepStaticDistance: Ensures static particles don't cross
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::KeepStaticDistance()
{
	float fPreviousX = this->m_RodMinX;
	for( unsigned iRod = 0; iRod < this->m_StaticParticles.size(); ++iRod )
	{
		int iNumFromEnd = this->m_StaticParticles.size() - iRod - 1;

		if( m_StaticParticles[ iRod ]->CurrentPosition.x < fPreviousX + this->m_StaticDistance )
		{
			m_StaticParticles[ iRod ]->CurrentPosition.x = std::min( fPreviousX + this->m_StaticDistance, this->m_RodMaxX - iNumFromEnd * this->m_StaticDistance );
		}

		fPreviousX = m_StaticParticles[ iRod ]->CurrentPosition.x;
	}
}

//**************************************************
//
// VerletIntegration: Move particles based on force 
//
// @author: Aiden Storey
//
// @param[in]	_kfTimeDelta			time since last update
//
//**************************************************
void CClothSimulation::VerletIntegration( const float& _kfTimeDelta )
{
	// Normalise wind direction and times it by wind speed for wind
	D3DXVECTOR3 vWind;
	D3DXVec3Normalize( &vWind, &this->m_vWindDirection );
	vWind *= this->m_fWindSpeed;

	// Calcualte delta squared
	float fDelta = _kfTimeDelta * _kfTimeDelta;

	// Iterate over all particles
	for( auto& aParticle : this->m_ParticleList )
	{
		// If particle isn't static
		if( !aParticle->Static )
		{
			// Initialise force 
			D3DXVECTOR3 vForce( this->m_Gravity );

			// If wind is enabled
			if( this->m_bWindEnabled )
			{
				// If the particle is within the wind cone
				if( Math::AngleBetween( this->m_vWindDirection, ( aParticle->CurrentPosition - this->m_vWindOrigin ) ) < this->m_fWindConeSize )
				{
					// Calculate its normal
					aParticle->CalculateNormal();
					
					// Calculate scalar from angle between normal and wind vector
					float fScalar = this->m_WindScalar * ( ( Math::PI * 0.5f ) - Math::AngleBetween( aParticle->Normal, vWind ) );

					// Add wind to force
					vForce += vWind * std::abs( fScalar );
				}
			}

			// Store particles previous position
			D3DXVECTOR3 vPrev = aParticle->CurrentPosition;

			// Calculate the offset to apply to current particle
			D3DXVECTOR3 vOffset = ( ( aParticle->CurrentPosition - aParticle->PreviousPosition ) ) + ( ( vForce * aParticle->Mass * 0.5f ) * _kfTimeDelta );

			// If cloth isn't colliding with itself
			if( !this->m_bSelfIntersectionEnabled || ( this->m_bSelfIntersectionEnabled && !this->IsSelfCollision( aParticle, vOffset ) ) )
			{
				// If the particle will move below the floor
				if( aParticle->CurrentPosition.y + vOffset.y < this->m_FloorPosition )
				{
					// Apply friction force to offset
					vOffset *= this->m_FloorFriction;

					// Apply offset to position and set y to be floot position
					aParticle->CurrentPosition += vOffset;
					aParticle->CurrentPosition.y = this->m_FloorPosition;
				}
				else
				{
					// Apply offset to position
					aParticle->CurrentPosition += vOffset;
				}
				
				// Set the previous position
				aParticle->PreviousPosition = vPrev;

				// Get new grid position
				TGridPosition* pGridPosition = this->m_SelfIntersectionGrid.GetGridPosition( aParticle->CurrentPosition );
				if( pGridPosition != nullptr )
				{
					// Add particle to grid position
					pGridPosition->AddParticle( aParticle );
				}

				// If the particle is on the grid
				if( aParticle->GridPosition != nullptr )
				{
					// Remove it from its previous grid position
					aParticle->GridPosition->RemoveParticle( aParticle );
				}

				// Set the particles new grid position
				aParticle->GridPosition = pGridPosition;				
			}
		}
	}
}

//**************************************************
//
// SatisfyConstraints: Ensures Constraints are moved to resting position
//
// @author: Aiden Storey
//
// @param[in]	_kfTimeDelta			time since last update
//
//**************************************************
void CClothSimulation::SatisfyConstraints( const float& _kfTimeDelta )
{
	// Iterate over all active particles
	for( auto& aLink : this->m_LinkList )
	{
		// If link is valid
		if( !aLink->Destroyed )
		{
			// Get each particle
			TClothParticle* pA = aLink->ParticleA;
			TClothParticle* pB = aLink->ParticleB;

			// Calculate difference vector of particles
			D3DXVECTOR3 vDifference = pA->CurrentPosition - pB->CurrentPosition;

			// Calculate distance between the particles
			float fDistance = Math::Magnitude( vDifference );

			// Calcualte the difference
			float fDifference = ( aLink->RestingDistance - fDistance ) / fDistance;

			// If the link is structural and tearing or wind is enabled
			if( ( this->m_bTearingEnabled || this->m_bWindEnabled ) && aLink->Type == ELinkType::STRUCTURAL )
			{
				// If the distance has exceeded tear distance
				if( fDistance > this->m_TearDistance )
				{
					// Destroy link and signal cleanup
					aLink->Destroy();
					this->m_bCleanup = true;
					continue;
				}
			}

			// Calculate the change vector 
			D3DXVECTOR3 vChange = vDifference *_kfTimeDelta * fDifference * m_ClothStiffness;

			// Calculate the inverse mass of the particles
			float fInverseMassA = 1 / pA->Mass;
			float fInverseMassB = 1 / pB->Mass;

			// Calculate the mass scalar of the particles
			float fMassScalarA = fInverseMassA / ( fInverseMassA + fInverseMassB );
			float fMassScalarB = fInverseMassB / ( fInverseMassA + fInverseMassB );

			// If A isn't static and B is or is selected
			if( !pA->Static && ( pB->Static || pB == this->m_pSelectedParticle ) )
			{
				// If sliding enabled
				if( this->m_bSlideEnabled && pB->Static )
				{
					// Apply x change to static particle
					pB->CurrentPosition.x -= vChange.x;
					pB->CurrentPosition.x = std::min( std::max( this->m_RodMinX, pB->CurrentPosition.x ), this->m_RodMaxX );
				}

				// Apply the full change to A
				pA->CurrentPosition += vChange * fMassScalarA;
			}
			// If B isn't static and A is or is selected
			else if( ( pA->Static || pA == this->m_pSelectedParticle ) && !pB->Static )
			{
				// If sliding enabled
				if( this->m_bSlideEnabled && pA->Static )
				{
					// Apply x change to static particle
					pA->CurrentPosition.x += vChange.x;
					pA->CurrentPosition.x = std::min( std::max( this->m_RodMinX, pA->CurrentPosition.x ), this->m_RodMaxX );
				}

				// Apply the full change to B
				pB->CurrentPosition -= vChange * fMassScalarB;
			}
			// If neither ate static
			else if( !pA->Static && !pB->Static )
			{
				// Apply half of the change to each particle
				vChange *= 0.5f;
				pA->CurrentPosition += vChange * fMassScalarA;
				pB->CurrentPosition -= vChange * fMassScalarB;
			}
		}
	}
}

//**************************************************
//
// CleanupLinks: Removes destroyed links 
//
// @author: Aiden Storey
//
//**************************************************
void CClothSimulation::CleanupLinks()
{
	// Iterate over all active links
	std::vector< TClothLink* >::iterator iterDelete = this->m_LinkList.begin();
	while( iterDelete != this->m_LinkList.end() )
	{
		// If the link is destroyed
		if( ( *iterDelete )->Destroyed )
		{
			// Remove the link from list
			TClothLink* pLink = ( *iterDelete );
			iterDelete = m_LinkList.erase( iterDelete );

			// Detach link from each particle
			pLink->ParticleA->RemoveLink( pLink );
			pLink->ParticleB->RemoveLink( pLink );

			// Delete if it isn't a structural link
			if( pLink->Type != ELinkType::STRUCTURAL )
			{
				delete ( pLink );
			}
		}
		// Increment iterator
		else
		{
			iterDelete++;
		}
	}

	// Iterate over all structural links
	iterDelete = this->m_StructuralLinkList.begin();
	while( iterDelete != this->m_StructuralLinkList.end() )
	{
		// If the link is destroyed
		if( ( *iterDelete )->Destroyed )
		{
			// Remove the link from list
			TClothLink* pLink = ( *iterDelete );
			iterDelete = m_StructuralLinkList.erase( iterDelete );

			// Delete the link
			delete ( pLink );
		}
		// Increment iterator
		else
		{
			iterDelete++;
		}
	}

}

//**************************************************
//
// OnUpdate: Handles update of all cloth system
//
// @author: Aiden Storey
//
// @param[in]	_kfTimeDelta			time since last update
//
// @returns		true if update successful false otherwise
//
//**************************************************
bool CClothSimulation::OnUpdate( const float& _kfTimeDelta )
{
	bool bReturn{ true };
	
	// Call interaction function
	( this->*m_pInteractionFunction )();

	// Add delta to time elapsed
	this->m_fTimeElapsed += _kfTimeDelta;

	while( this->m_fTimeElapsed > this->m_TimePerUpdate )
	{
		// Move particles using integration
		this->VerletIntegration( this->m_TimePerUpdate );

		// Satisfy constraints the set amount per frame
		for( unsigned i = 0; i < m_uSatisfyIterationCount; ++i )
		{
			this->SatisfyConstraints( this->m_TimePerUpdate );
		}

		// Check if satisfy constraints has caused an intersection
		if( this->m_bSelfIntersectionEnabled )
		{
			this->CheckSelfCollision();
		}

		// Check collisions with active collider
		if( this->m_pActiveCollider != nullptr )
		{
			this->CheckCollisions();
		}

		// Ensure static doesn't cross over when sliding
		if( this->m_bSlideEnabled )
		{
			this->KeepStaticDistance();
		}

		// Continue to burn links if burning has been activated
		if( this->m_bBurning )
		{
			this->BurnLinks();
		}

		// Remove destroyed links
		if( this->m_bCleanup )
		{
			this->CleanupLinks();
			this->m_bCleanup = false;
		}
		
		//this->m_fTimeElapsed = 0.0f;
		this->m_fTimeElapsed -= this->m_TimePerUpdate;
	}	

	// Remap buffer for drawing
	MapLinkBuffer();

	return ( bReturn );
}

//**************************************************
//
// OnDraw: Draws link vertex buffer
//
// @author: Aiden Storey
//
// @param[in]	_pRenderCamera			Camera to render with
// @param[in]	_krOffset			Parents offset 
//
//**************************************************
void CClothSimulation::OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{
	// Creates variables for drawing
	UINT stride = sizeof( VDefault3D );
	UINT offset = 0;

	// Set the device variables for drawing
	this->m_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_LINELIST );
	this->m_pDevice->IASetInputLayout( this->m_pShader->GetInputLayout() );

	// Give the variables to the shader
	D3DXMATRIX matWVP = ( this->m_matWorld * _krOffset ) * _pRenderCamera->GetViewProjMatrix();
	this->m_pShader->GetVarWVP()->SetMatrix( reinterpret_cast< float* >( &matWVP ) );
	this->m_pShader->GetTechnique()->GetPassByIndex( 0 )->Apply( 0 );

	// Draw link buffer
	this->m_pDevice->IASetVertexBuffers( 0, 1, &m_pLinkBuffer, &stride, &offset );
	this->m_pDevice->Draw( m_LinkDrawCount, 0 );
}