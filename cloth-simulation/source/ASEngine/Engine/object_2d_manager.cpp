//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Local Includes
#include "math_functions.h"

// This Include
#include "object_2d_manager.h"

// Implementation
CObject2DManager::CObject2DManager() :
	m_PointsPerCircle( 30 )
{}

CObject2DManager::~CObject2DManager()
{
	for( auto& aObject : this->m_ObjectList )
	{
		delete aObject;
	}
}

CTextured2D* CObject2DManager::CreateObjectAsRectangle( const std::wstring& _kstrName, const std::wstring _kstrTexture, const float& _kfWidth, const float& _kfHeight, const float& _kfPositionX,
														const float& _kfPositionY, const float& _kfRotation )
{
	CTextured2D* pReturn = new CTextured2D( _kstrName, _kstrTexture, _kfPositionX, _kfPositionY, _kfRotation );

	this->FillBufferRectanlge( _kfWidth, _kfHeight, &pReturn->m_VertexList );
	pReturn->InitialiseObject();

	this->m_ObjectList.push_back( pReturn );

	return ( pReturn );
}

CTextured2D* CObject2DManager::CreateObjectAsCircle( const std::wstring& _kstrName, const std::wstring _kstrTexture, const float& _kfRadius, const float& _kfPositionX, const float& _kfPositionY,
													 const float& _kfRotation )
{
	CTextured2D* pReturn = new CTextured2D( _kstrName, _kstrTexture, _kfPositionX, _kfPositionY, _kfRotation );

	this->FillBufferCircle( _kfRadius, &pReturn->m_VertexList );
	pReturn->InitialiseObject();

	this->m_ObjectList.push_back( pReturn );

	return ( pReturn );
}

CButtonRectangle* CObject2DManager::CreateButtonAsRectangle( const std::wstring& _kstrName, const std::wstring _kstrTexture, const float& _kfWidth, const float& _kfHeight, const float& _kfPositionX,
																  const float& _kfPositionY, const float& _kfRotation )
{
	CButtonRectangle* pReturn = new CButtonRectangle( _kstrName, _kstrTexture, _kfWidth, _kfHeight, _kfPositionX, _kfPositionY, _kfRotation );

	this->FillBufferRectanlge( _kfWidth, _kfHeight, &pReturn->m_VertexList );
	pReturn->InitialiseObject();

	this->m_ObjectList.push_back( pReturn );

	return ( pReturn );
}

CButtonCircle* CObject2DManager::CreateButtonAsCircle( const std::wstring& _kstrName, const std::wstring _kstrTexture, const float& _kfRadius, const float& _kfPositionX, const float& _kfPositionY,
															   const float& _kfRotation )
{
	CButtonCircle* pReturn = new CButtonCircle( _kstrName, _kstrTexture, _kfRadius * 2.0f, _kfPositionX, _kfPositionY, _kfRotation );

	this->FillBufferCircle( _kfRadius, &pReturn->m_VertexList );
	pReturn->InitialiseObject();

	this->m_ObjectList.push_back( pReturn );

	return ( pReturn );
}

void CObject2DManager::FillBufferRectanlge( const float& _kfWidth, const float& _kfHeight, std::vector< VTextured2D >* _pBuffer )
{
	// Calculate Variables for generation
	float fhW = _kfWidth * 0.5f;
	float fhH = _kfHeight * 0.5f;

	// Populate list
	_pBuffer->push_back( { D3DXVECTOR2( -fhW, +fhH ), D3DXVECTOR2( 0.0f, 0.0f ) } );
	_pBuffer->push_back( { D3DXVECTOR2( +fhW, +fhH ), D3DXVECTOR2( 1.0f, 0.0f ) } );
	_pBuffer->push_back( { D3DXVECTOR2( -fhW, -fhH ), D3DXVECTOR2( 0.0f, 1.0f ) } );
	_pBuffer->push_back( { D3DXVECTOR2( +fhW, -fhH ), D3DXVECTOR2( 1.0f, 1.0f ) } );
}

void CObject2DManager::FillBufferCircle( const float& _kfRadius, std::vector< VTextured2D >* _pBuffer )
{
	// Calculate variables for generation
	float fAngle = ( 2.0f * Math::PI ) / m_PointsPerCircle;
	float fUVScalar = 1.0f / ( _kfRadius * 2.0f );

	std::vector< VTextured2D > VertexList;

	// Push center
	VertexList.push_back( { D3DXVECTOR2( 0.0f, 0.0f ), D3DXVECTOR2( 0.5f, 0.5f ) } );

	// Calculate vertices around exterior
	for( unsigned i = 0; i < m_PointsPerCircle; ++i )
	{
		D3DXVECTOR2 pos( _kfRadius * cosf( fAngle * i ), _kfRadius * sinf( fAngle * i ) );
		D3DXVECTOR2 texc( 0.5f + pos.x * fUVScalar, 0.5f + pos.y * -fUVScalar );

		VertexList.push_back( { pos, texc } );
	}

	int iVertexNum = VertexList.size();

	// Populate list
	for( unsigned i = 0; i <= m_PointsPerCircle; ++i )
	{
		_pBuffer->push_back( VertexList[ 0 ] );
		_pBuffer->push_back( VertexList[ ( i + 2 ) % m_PointsPerCircle + 1 ] );
		_pBuffer->push_back( VertexList[ ( i + 1 ) % m_PointsPerCircle + 1 ] );
	}
}
