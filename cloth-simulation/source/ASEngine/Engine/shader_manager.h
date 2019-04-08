//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_SHADER_MANAGER_H_201409182158
#define GUARD_AIDEN_STOREY_SHADER_MANAGER_H_201409182158

// Library Includes
#include <vector>

// Local Include
#include "shader.h"

// Prototypes
class CShaderManager
{
	// Member Functions
public:
	// Constructors
	CShaderManager();
	~CShaderManager();

	// Accessors 
	CShader* GetShader( const int _kiShaderID );

	std::wstring GetShaderPath() const;
	void SetShaderPath( const std::wstring _kstrPath );
	
	// Other
	template< typename T > bool CreateShader( const int _kiShaderID );

protected:
private:

	// Member Variables
public:
protected:
private:
	std::vector< CShader* > m_ShaderList;
	CShader*				m_pCurrentShader{ nullptr };
	std::wstring			m_strShaderPath;
};

// Templated function definitions
#include "shader_manager.hpp"

#endif // GUARD_AIDEN_STOREY_SHADER_MANAGER_H_201409182158