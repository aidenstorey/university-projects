//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SHADER_H_201409182204
#define GUARD_AIDEN_STOREY_SHADER_H_201409182204

// Library Includes
#include <D3DX10.h>
#include <string>

// Prototypes
class CShader
{
	// Member Functions
public:
	// Constructors
	CShader( const int _kiShaderID, const std::wstring _kstrPath );
	virtual ~CShader();

	// Accessors
	int GetShaderID() const;

	bool IsInitialised() const;

	ID3D10Effect* GetEffect();
	ID3D10EffectTechnique* GetTechnique();
	ID3D10InputLayout* GetInputLayout();

	// Other
protected:
	ID3D10Effect* CreateEffect( const std::wstring _kstrShader );
private:

	// Member Variables
public:
protected:
	int				m_iShaderID;
	bool			m_bIsInitialised;

	ID3D10Effect*			m_pEffect;
	ID3D10EffectTechnique*	m_pTechnique;
	ID3D10InputLayout*		m_pInputLayout;

private:
};

#endif // GUARD_AIDEN_STOREY_SHADER_H_201409182204