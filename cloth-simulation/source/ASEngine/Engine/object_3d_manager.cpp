//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// This Include
#include "object_3d_manager.h"

// Implementation
CObject3DManager::CObject3DManager()
{}

CObject3DManager::~CObject3DManager()
{
	for( auto& aObject : this->m_ObjectList )
	{
		delete ( aObject );
	}
}

CMesh* CObject3DManager::CreateMesh( const std::wstring _kstrMeshFile, const float& _kfPositionX, const float& _kfPositionY, const float& _kfPositionZ,
									 const float& _kfRotationX, const float& _kfRotationY, const float& _kfRotationZ,
									 const float _kfScaleX, const float& _kfScaleY, const float _kfScaleZ )
{
	CMesh* pReturn = new CMesh( _kstrMeshFile, _waccess_s( ( _kstrMeshFile + L"c" ).c_str(), 04 ) == 0,
								_kfPositionX, _kfPositionY, _kfPositionZ, _kfRotationX, _kfRotationY, _kfRotationZ, _kfScaleX, _kfScaleY, _kfScaleZ );

	if( pReturn->IsLoadSuccessful() )
	{
		this->m_ObjectList.push_back( pReturn );
	}
	else
	{
		delete ( pReturn );
		pReturn = nullptr;
	}

	return ( pReturn );
}