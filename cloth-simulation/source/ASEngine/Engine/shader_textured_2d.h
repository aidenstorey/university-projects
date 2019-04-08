//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SHADER_TEXTURED_2D_H_201409210031
#define GUARD_AIDEN_STOREY_SHADER_TEXTURED_2D_H_201409210031

// Local Includes
#include "shader.h"

// Prototypes
class CShaderTextured2D : public CShader
{
	// Member Functions
public:
	// Constructors
	CShaderTextured2D( const int _kiShaderID, const std::wstring _kstrPath );

	// Accessors
	ID3D10EffectMatrixVariable* GetVarWVP();
	ID3D10EffectShaderResourceVariable* GetVarTexture();
protected:
private:
	
	// Member Variables
public:
protected:
private:
	ID3D10EffectMatrixVariable*			m_pVarWVP;
	ID3D10EffectShaderResourceVariable* m_pVarTexture;
};

#endif // GUARD_AIDEN_STOREY_SHADER_TEXTURED_2D_H_201409210031