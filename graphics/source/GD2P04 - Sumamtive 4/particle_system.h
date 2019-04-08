#ifndef GUARD_AIDEN_STOREY_PARTICLE_SYSTEM_H_20140623
#define GUARD_AIDEN_STOREY_PARTICLE_SYSTEM_H_20140623

// Local Includes
#include "d3dUtil.h"

// Constant variables
const unsigned g_ParticleSystemMaxParticles = 10000;

// Prototypes
struct PVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vel;
	D3DXVECTOR2 size;
	float		age;
	unsigned	type;
};


class CParticles
{
	// Member Functions
public:
	// Constructors
	CParticles();
	~CParticles();

	// Accessors
	float GetAge() const;

	void SetEyePosition( const D3DXVECTOR3& _vPosition );
	void SetEmitPosition( const D3DXVECTOR3& _vPosition );
	void SetEmitDirection( const D3DXVECTOR3& _vDirection );

	// Other
	void Initialise();

	void Reset();
	void Update( float _fDT, float _fGT );
	void Draw( D3DXMATRIX _matView, D3DXMATRIX _matProj );
protected:
	// Other 
	void BuildFX();
	void BuildVertexLayout();
	void CreateTexture();

private:

	// Member Varaibles
public:
protected:
private:
	bool									m_bFirst;

	float									m_fGameTime;
	float									m_fTimeStep;
	float									m_fAge;

	D3DXVECTOR4								m_vEyePositionW;
	D3DXVECTOR4								m_vEmitPositionW;
	D3DXVECTOR4								m_vEmitDirectionW;

	ID3D10Device*							m_pDevice;
	ID3D10Buffer*							m_pIVertexBuffer;
	ID3D10Buffer*							m_pDVertexBuffer;
	ID3D10Buffer*							m_pSVertexBuffer;

	ID3D10ShaderResourceView*				m_RandomTextureRV;

	ID3D10Effect*							m_ParticleFX;

	ID3D10InputLayout*						m_PVertexLayout;

	ID3D10EffectTechnique*					m_StreamTech;
	ID3D10EffectTechnique*					m_DrawTech;

	ID3D10EffectMatrixVariable*				m_fxViewProjVar;
	ID3D10EffectScalarVariable*				m_fxGameTimeVar;
	ID3D10EffectScalarVariable*				m_fxTimeStepVar;
	ID3D10EffectVectorVariable*				m_fxEyePositionVar;
	ID3D10EffectVectorVariable*				m_fxEmitPositionVar;
	ID3D10EffectVectorVariable*				m_fxEmitDirectionVar;
	ID3D10EffectShaderResourceVariable*		m_fxRandomTextureVar;


};

#endif // GUARD_AIDEN_STOREY_PARTICLE_SYSTEM_H_20140623