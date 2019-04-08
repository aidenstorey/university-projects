//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SHADER_DEFAULT_2D_H_201409182249
#define GUARD_AIDEN_STOREY_SHADER_DEFAULT_2D_H_201409182249

// Local Includes
#include "shader.h"

// Prototypes
class CShaderDefault2D : public CShader
{
	// Member Functions
public:
	// Constructors
	CShaderDefault2D( const int _kiShaderID, const std::wstring _kstrPath );

	// Accessors
	ID3D10EffectMatrixVariable* GetVarWVP();

protected:
private:
	
	// Member Variables
public:
protected:
private:
	ID3D10EffectMatrixVariable* m_pVarWVP;

};

#endif // GUARD_AIDEN_STOREY_SHADER_DEFAULT_2D_H_201409182249