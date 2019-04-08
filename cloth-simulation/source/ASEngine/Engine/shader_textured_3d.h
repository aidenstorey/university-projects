//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SHADER_TEXTURED_3D_H_201410091454
#define GUARD_AIDEN_STOREY_SHADER_TEXTURED_3D_H_201410091454

// Local Includes
#include "shader.h"

// Prototypes
class CShaderTextured3D : public CShader
{
	// Member Functions
public:
	// Constructors
	CShaderTextured3D( const int _kiShaderID, const std::wstring _kstrPath );

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

#endif // GUARD_AIDEN_STOREY_SHADER_TEXTURED_3D_H_201410091454