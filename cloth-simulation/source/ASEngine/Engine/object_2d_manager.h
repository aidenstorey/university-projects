//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_OBJECT_2D_MANAGER_H_201409231357
#define GUARD_AIDEN_STOREY_OBJECT_2D_MANAGER_H_201409231357

// Library Includes
#include <vector>

// Local Includes
#include "button_circle.h"
#include "button_rectangle.h"
#include "object_2d.h"
#include "textured_2d.h"
#include "vertex.h"

// Prototypes
class CObject2DManager
{
	// Member Functions
public:
	// Constructors
	CObject2DManager();
	~CObject2DManager();

	// Other
	CTextured2D* CreateObjectAsRectangle( const std::wstring& _kstrName = L"", const std::wstring _kstrTexture = L"DefaultTexture.png",
										  const float& _kfWidth = 1.0f, const float& _kfHeight = 1.0f, const float& _kfPositionX = 0.0f,
										  const float& _kfPositionY = 0.0f, const float& _kfRotation = 0.0f );

	CTextured2D* CreateObjectAsCircle( const std::wstring& _kstrName = L"", const std::wstring _kstrTexture = L"DefaultTexture.png",
									   const float& _kfRadius = 0.5f, const float& _kfPositionX = 0.0f, const float& _kfPositionY = 0.0f,
									   const float& _kfRotation = 0.0f );
	
	CButtonRectangle* CreateButtonAsRectangle( const std::wstring& _kstrName = L"", const std::wstring _kstrTexture = L"DefaultTexture.png",
											   const float& _kfWidth = 1.0f, const float& _kfHeight = 1.0f, const float& _kfPositionX = 0.0f,
											   const float& _kfPositionY = 0.0f, const float& _kfRotation = 0.0f );

	CButtonCircle* CreateButtonAsCircle( const std::wstring& _kstrName = L"", const std::wstring _kstrTexture = L"DefaultTexture.png",
										 const float& _kfRadius = 0.5f, const float& _kfPositionX = 0.0f, const float& _kfPositionY = 0.0f,
										 const float& _kfRotation = 0.0f );
			
	void FillBufferRectanlge( const float& _kfWidth, const float& _kfHeight, std::vector< VTextured2D >* _pBuffer );
	void FillBufferCircle( const float& _kfRadius, std::vector< VTextured2D >* _pBuffer );

protected:
private:

	// Member Variables
public:
protected:
private:
	std::vector< CObject2D* > m_ObjectList;

	const unsigned m_PointsPerCircle;
};

#endif // GUARD_AIDEN_STOREY_OBJECT_2D_MANAGER_H_201409231357