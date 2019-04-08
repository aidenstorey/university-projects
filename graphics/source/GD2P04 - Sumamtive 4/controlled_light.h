#ifndef GUARD_AIDEN_STOREY_CONTROLLED_LIGHT_H_20140623
#define GUARD_AIDEN_STOREY_CONTROLLED_LIGHT_H_20140623

// Local Includes
#include "d3dApp.h"
#include "keyboard_input.h"
#include "Light.h"
#include "Vertex.h"

// Constant Variables
const int g_TimeMPH = 60;
const int g_TimeSPM = 60;
const int g_TimeSPD = 86400;

const int g_TimeSR	= 26760;
const int g_TimeSS	= 61920;

const int g_TimeMR	= 56128;
const int g_TimeMS	= 17460;

const float g_TimeTransition = 6000.0f;

const float g_TimeSSStart	= g_TimeSS - g_TimeTransition / 2.0f;
const float g_TimeSSFinish	= g_TimeSS + g_TimeTransition / 2.0f;

const float g_TimeSRStart	= g_TimeSR - g_TimeTransition / 2.0f;
const float g_TimeSRFinish	= g_TimeSR + g_TimeTransition / 2.0f;


const float g_TimeMoonUp	= g_TimeSPD - ( g_TimeSSStart - g_TimeSRFinish );

const float g_LightSunRadius	= 500.0f;
const float g_LightMoonRadius	= 200.0f;
const float g_LightTheta		= PI * 1.5f;

const float g_LightUserAddition	= 100.0f;

const D3DXCOLOR g_LightColorDay		= D3DXCOLOR( 0.4f, 0.6f, 1.0f, 1.0f );
const D3DXCOLOR g_LightColorNight	= D3DXCOLOR( 0.03f, 0.04f, 0.08f, 1.0f );
const D3DXCOLOR	g_LightColorDusk	= D3DXCOLOR( 0.4f, 0.2f, 0.3f, 1.0f );

// Enumerations
enum ETimeState
{
	TS_DAY,
	TS_NIGHT,
	TS_DAYOUT,
	TS_DAYIN,
	TS_NIGHTOUT,
	TS_NIGHTIN,
};


// Prototype
class CControlledLight
{
	// Member Fucntions
public:
	// Constructors
	CControlledLight();
	~CControlledLight();

	// Accessors
	Light GetLight();

	D3DXVECTOR3 GetFocus() const;
	void SetFocus( const D3DXVECTOR3 _vFocus );

	bool IsRaining();
	bool IsStartedRaining();

	D3DXCOLOR GetSkyColor() const;

	D3DXVECTOR3	GetDirection() const;
	void SetDirection( const D3DXVECTOR3 _vDirection );

	D3DXMATRIX GetViewMatrix() const;
	D3DXMATRIX GetVolume() const;

	// Other
	void Initialise();
	void Update( float _fDT );
	void Draw( D3DXMATRIX _matView, D3DXMATRIX _matProj, D3DXVECTOR3 _vEyePos );

protected:
	// Other
	void InitialiseGraphics();

	void BuildFX();
	void BuildVertexLayout();
	void LoadTextures();

	void MapBuffer();

	float CalculateSunAngle( float _fTime );
	float CalculateMoonAngle( float _fTime );
	D3DXVECTOR3 CalculatePosition( float _fRadius, float _fAngle );
	D3DXCOLOR CalculateSkyColor( float _fTime );
	ETimeState	CalculateTimeState( float _fTime );

private:

	// Member Variables
public:
protected:
private:
	ETimeState								m_eTimeState;

	Light									m_SunLight;
	Light									m_MoonLight;
	
	float									m_fSunTime;
	float									m_fMoonTime;
	float									m_fTimeSinceMoonUp;

	bool									m_bRaining;
	bool									m_bStartedRaining;
	bool									m_bDayTime;

	D3DXVECTOR3								m_vFocus;
	D3DXVECTOR3								m_vSunPos;
	D3DXVECTOR3								m_vMoonPos;

	D3DXCOLOR								m_vSkyColor;

	CKeyboardInput*							m_pKeyboard;

	ID3D10Device*							m_pDevice;
	ID3D10Buffer*							m_pVertexBuffer;
	
	ID3D10Effect*							m_BillboardFX;
	ID3D10InputLayout*						m_BVertexLayout;

	ID3D10EffectTechnique*					m_BillboardTech;

	ID3D10EffectMatrixVariable*				m_fxViewProjVar;
	ID3D10EffectVectorVariable*				m_fxEyePositionVar;
	ID3D10EffectShaderResourceVariable*		m_fxTextureArray;
	ID3D10ShaderResourceView*				m_TextureArrayRV;

	VBillboard								m_SunMoon[ 2 ];

	D3DXMATRIX								m_matMoonView;
	D3DXMATRIX								m_matSunView;
	D3DXMATRIX								m_matLightVolume;
};

#endif // GUARD_AIDEN_STOREY_CONTROLLED_LIGHT_H_20140623