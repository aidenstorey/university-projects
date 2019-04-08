//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: cloth_simulation.h
// Description	: Declaration for CClothSimulation
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

#ifndef GUARD_AIDEN_STOREY_CLOTH_SIMULATION_H_201410091646
#define GUARD_AIDEN_STOREY_CLOTH_SIMULATION_H_201410091646

// Library Includes
#include <vector>
#include <unordered_map>

// Local Includes
#include "cloth_link.h"
#include "cloth_particle.h"
#include "collider.h"
#include "drawable.h"
#include "math_functions.h"
#include "self_intersection_grid.h"
#include "shader_default_3d.h"

// Constants
enum class EInteractionType
{
	BURN,
	CUT,
	GRAB,
};

// Prototypes
struct TLinkDefinition
{
	ELinkType	Type;
	int			OffsetX;
	int			OffsetY;
	float		Spacing;
	bool		Horizontal;
};

class CClothSimulation : public CDrawable
{
	// Member Functions
public:
	// Constructors
	CClothSimulation( const unsigned& _uRowCount, const unsigned& _uColCount, const unsigned& _uPinCount );
	~CClothSimulation();

	// Accessors
	void SetInteractionType( const EInteractionType& _eType );
	void SetTearingEnabled( const bool& _kbTearingEnabled );
	void SetSlidingEnabled( const bool& _kbSlidingEnabled );
	void SetSelfIntersectingEnabled( const bool& _kbEnabled );
	void SetWindEnabled( const bool& _kbWindEnabled );
	void SetActiveCollider( CCollider* _pCollider );
	void SetWindOrigin( const D3DXVECTOR3& _krOrigin );
	void SetWindDirection( const D3DXVECTOR3& _kvDirection );
	void SetWindSpeed( const float& _kfSpeed );
	void SetWindConeSize( const float& _kfSize );

	// Other
	void ReleaseCloth();

protected:
	// Other
	bool OnUpdate( const float& _kfTimeDelta ) override;
	void OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset ) override;

private:
	// Accessors
	bool IsOnMesh( int _iRow, int _iCol );

	// Other
	void SetupCloth();
	void SetupLinkBuffer();

	void AddParticleLinks( const int& _iRow, const int& _iCol );

	void MapLinkBuffer();

	TClothParticle* ClosestToMouse( const bool& _kbStatic );

	void InteractionGrab();
	void InteractionCut();
	void InteractionBurn();

	void BurnLinks();

	void VerletIntegration( const float& _kfTimeDelta );
	void SatisfyConstraints( const float& _kfTimeDelta );
	void CleanupLinks();

	void CheckCollisions();
	void CheckSelfCollision();
	bool IsSelfCollision( TClothParticle* _pParticle, const D3DXVECTOR3& _krOffset );

	void KeepStaticDistance();
	
	// Member Variables
public:
protected:
private:
	ID3D10Device*					m_pDevice{ nullptr };
	ID3D10Buffer*					m_pLinkBuffer{ nullptr };

	CShaderDefault3D*				m_pShader;

	std::vector< TClothParticle* >	m_ParticleList;
	std::vector< TClothParticle* >	m_StaticParticles;

	std::vector< TClothLink* >		m_LinkList;
	std::vector< TClothLink* >		m_StructuralLinkList;

	std::vector< TLinkDefinition >	m_LinkDefinitionList;

	unsigned						m_LinkDrawCount;

	bool							m_bTearingEnabled{ false };
	bool							m_bSlideEnabled{ false };
	bool							m_bWindEnabled{ false };
	bool							m_bSelfIntersectionEnabled{ false };

	bool							m_bBurning{ false };
	bool							m_bCleanup{ false };

	TClothParticle*					m_pSelectedParticle{ nullptr };
	D3DXVECTOR2						m_vTearMousePosition;

	void( CClothSimulation::*m_pInteractionFunction )( );

	float							m_fTimeElapsed{ 0.0f };
	unsigned						m_uSatisfyIterationCount;

	CCollider*						m_pActiveCollider{ nullptr };

	D3DXVECTOR3						m_vWindOrigin{ 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3						m_vWindDirection{ 0.0f, 0.0f, 0.0f };
	float							m_fWindSpeed{ 0.0f };
	float							m_fWindConeSize{ 0.0f };

	TSelfIntersectionGrid			m_SelfIntersectionGrid;

	const unsigned					m_RowCount;
	const unsigned					m_ColCount;
	const unsigned					m_PinCount;
	const float						m_Spacing{ 0.2f };
	const float						m_SpacingDouble{ m_Spacing * 2 };
	const float						m_SpacingDiagonal{ sqrt( m_Spacing * m_Spacing + m_Spacing * m_Spacing ) };
	const float						m_GrabDistance{ m_Spacing * 0.2f };
	const float						m_TearDistance{ 1.0f };
	const float						m_StaticDistance{ m_Spacing * 0.5f };
	const float						m_FloorPosition{ -3.0f };
	const float						m_TimePerUpdate{ 1.0f / 60.0f };
	const float						m_TouchHeat{ 0.1f };
	const float						m_BurnHeat{ 10.0f };
	const float						m_HeatTransferRate{ 0.1f };
	const unsigned					m_HeatTrasnferChance{ 100 };
	const float						m_WindScalar{ 1.0f / ( Math::PI * 0.5f ) };
	const float						m_MaxWindSpeed{ 10.0f };
	const float						m_MaxWindConeSize{ Math::PI * 0.2f };
	const D3DXCOLOR					m_ClothColorBase{ 0.08f, 0.45f, 0.6f, 1.0f };
	const D3DXCOLOR					m_ClothColorBurnt{ 0.852941f, 0.403922f, 0.039216f, 1.0f };
	const D3DXCOLOR					m_ClothColorStep{ ( m_ClothColorBurnt - m_ClothColorBase ) / m_BurnHeat };
	const D3DXVECTOR3				m_Gravity{ 0.0f, -0.98f, 0.0f };
	const float						m_FloorFriction{ 0.9f };
	const float						m_ClothStiffness{ 100.0f };
	const float						m_ClothMass{ 1.0f };
	const float						m_RodMaxX{ 3.5f };
	const float						m_RodMinX{ -3.5f };
};

#endif // GUARD_AIDEN_STOREY_CLOTH_SIMULATION_H_201410091646