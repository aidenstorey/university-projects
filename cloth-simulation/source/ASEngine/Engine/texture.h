//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_TEXTURE_H_201409202124
#define GUARD_AIDEN_STOREY_TEXTURE_H_201409202124

// Library Includes
#include <D3DX10.h>
#include <string>

// Prototypes
class CTexture
{
	// Member Functions
public:
	// Constructors
	CTexture();
	~CTexture();
	
	// Accessors
	std::wstring GetName() const;
	void SetName( const std::wstring _kwstrName );

	ID3D10ShaderResourceView* GetTexture();

	// Other
	bool Initialise( const std::wstring& _kstrTextureFile );

protected:
private:

	// Member Variables
public:
protected:
private:
	std::wstring				m_strName;
	ID3D10ShaderResourceView*	m_pTexture;
};

#endif // GUARD_AIDEN_STOREY_TEXTURE_H_201409202124