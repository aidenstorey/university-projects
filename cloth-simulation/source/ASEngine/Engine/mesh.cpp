//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Library Includes
#include <fstream>
#include <sstream>

// Local Includes
#include "engine.h"
#include "safe_release.h"
#include "shader_textured_3d.h"
#include "shader_id_list.h"

// This Include 
#include "mesh.h"

// Implementation
CMesh::CMesh( const std::wstring _kstrMeshFile, bool _bCompiled,
			  const float& _kfPositionX, const float& _kfPositionY, const float& _kfPositionZ,
			  const float& _kfRotationX, const float& _kfRotationY, const float& _kfRotationZ,
			  const float _kfScaleX, const float& _kfScaleY, const float _kfScaleZ ) :
	CObject3D( _kfPositionX, _kfPositionY, _kfPositionZ, _kfRotationX, _kfRotationY, _kfRotationZ, _kfScaleX, _kfScaleY, _kfScaleZ )
{
	this->m_bLoadSuccessful = true;

	if( _bCompiled )
	{
		this->m_bLoadSuccessful &= this->LoadCompiledOBJ( _kstrMeshFile );
	}
	else
	{
		this->m_bLoadSuccessful &= this->LoadOBJ( _kstrMeshFile );
	}

	if( this->m_bLoadSuccessful )
	{
		this->m_uVertexCount = this->m_VertexList.size();
		this->m_pDevice = &CEngine::GetInstance().GetDevice();

		D3D10_BUFFER_DESC vbDesc;
		vbDesc.Usage = D3D10_USAGE_DEFAULT;
		vbDesc.ByteWidth = sizeof( VTextured3D ) * this->m_uVertexCount;
		vbDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;

		D3D10_SUBRESOURCE_DATA vsData;
		vsData.pSysMem = this->m_VertexList.data();

		this->m_pDevice->CreateBuffer( &vbDesc, &vsData, &this->m_pVertexBuffer );

		this->m_pTexture = CEngine::GetInstance().GetTextureManager().GetTexture( L"stripes.jpg" );
	}
}

CMesh::~CMesh()
{
	ReleaseCOM( this->m_pVertexBuffer );
}

bool CMesh::IsLoadSuccessful()
{
	return ( this->m_bLoadSuccessful );
}

void CMesh::SetTexture( std::wstring _kstrTexture )
{
	this->m_pTexture = CEngine::GetInstance().GetTextureManager().GetTexture( _kstrTexture );
}

bool CMesh::LoadOBJ( const std::wstring _kstrMeshFile )
{
	bool bLoadSuccesful{ false };

	// Input lists
	std::vector< D3DXVECTOR3 > VertexList;
	std::vector< D3DXVECTOR3 > NormalList;
	std::vector< D3DXVECTOR2 > UVList;
	std::vector< D3DXVECTOR3 > IndexList;

	std::string strLine;
	std::fstream fsLoader;

	fsLoader.open( _kstrMeshFile );
	if( fsLoader.is_open() )
	{
		bLoadSuccesful = true;

		while( std::getline( fsLoader, strLine ) )
		{
			std::stringstream sLoader;
			sLoader << strLine << '\0';

			char LineBuffer[ 256 ] = { 0 };
			sLoader >> LineBuffer;

			char* Context = NULL;

			if( strncmp( LineBuffer, "f", 10 ) == 0 )
			{
				for( int i = 0; i < 3; ++i )
				{
					D3DXVECTOR3 vIndice;
					sLoader >> LineBuffer;

					vIndice.x = static_cast< float > ( atof( strtok_s( LineBuffer, "/", &Context ) ) );
					vIndice.y = static_cast< float > ( atof( strtok_s( NULL, "/", &Context ) ) );
					vIndice.z = static_cast< float > ( atof( strtok_s( NULL, " ", &Context ) ) );

					IndexList.push_back( vIndice );
				}
			}
			else if( strncmp( LineBuffer, "v", 2 ) == 0 )
			{
				D3DXVECTOR3 vVect;
				sLoader >> vVect.x >> vVect.y >> vVect.z;
				VertexList.push_back( vVect );
			}
			else if( strncmp( LineBuffer, "vn", 2 ) == 0 )
			{
				D3DXVECTOR3 vNorm;
				sLoader >> vNorm.x >> vNorm.y >> vNorm.z;
				NormalList.push_back( vNorm );
			}
			else if( strncmp( LineBuffer, "vt", 2 ) == 0 )
			{
				D3DXVECTOR2 vUV;
				sLoader >> vUV.x >> vUV.y;
				UVList.push_back( vUV );
			}
		}

		fsLoader.close();
	}

	for( unsigned index = 0; index < IndexList.size(); ++index )
	{
		this->m_VertexList.push_back( VTextured3D( VertexList[ static_cast< int >( IndexList[ index ].x - 1 ) ], UVList[ static_cast< int >( IndexList[ index ].y - 1 ) ] ) );
	}


	return ( bLoadSuccesful );
}

bool CMesh::LoadCompiledOBJ( const std::wstring _kstrMeshFile )
{
	bool bLoadSuccesful{ true };
	return ( bLoadSuccesful );
}

void CMesh::OnDraw( CCamera* _pRenderCamera, const D3DXMATRIX& _krOffset )
{
	CShaderTextured3D* pShader = reinterpret_cast< CShaderTextured3D* >( CEngine::GetInstance().GetShaderManager().GetShader( SHADER_TEXTURED3D ) );

	unsigned stride = sizeof( VTextured3D );
	unsigned offset = 0;

	this->m_pDevice->IASetInputLayout( pShader->GetInputLayout() );
	this->m_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	D3DXMATRIX matWVP = this->m_matWorld * _krOffset * _pRenderCamera->GetViewProjMatrix();
	pShader->GetVarWVP()->SetMatrix( reinterpret_cast< float* >( &matWVP ) );
	pShader->GetVarTexture()->SetResource( this->m_pTexture->GetTexture() );
	pShader->GetTechnique()->GetPassByIndex( 0 )->Apply( 0 );

	this->m_pDevice->IASetVertexBuffers( 0, 1, &this->m_pVertexBuffer, &stride, &offset );
	this->m_pDevice->Draw( this->m_uVertexCount, 0 );
}