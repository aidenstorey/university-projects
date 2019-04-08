//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_TEXTURE_MANAGER_H_201409202133
#define GUARD_AIDEN_STOREY_TEXTURE_MANAGER_H_201409202133

// Library Includes
#include <map>
#include <string>

// Local includes
#include "texture.h"

// Prototypes
class CTextureManager
{
	// Member Functions
public:
	// Constructors
	CTextureManager();
	~CTextureManager();

	// Accessors 
	CTexture* GetTexture( const std::wstring& _kstrTextureFile );

	std::wstring GetTexturePath() const;
	void SetTexturePath( const std::wstring _kstrPath );
protected:
private:

	// Member Variables
public:
protected:
private:
	std::map< std::wstring, CTexture* > m_Textures;
	std::wstring						m_strTexturePath;
};

#endif // GUARD_AIDEN_STOREY_TEXTURE_MANAGER_H_201409202133