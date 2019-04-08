// Local Includes
#include "services.h"
#include "Vertex.h"

// This Include
#include"instance_cube.h"

// Implementation
CInstanceCube::CInstanceCube() :
	m_pVertexBuffer( nullptr )
{}

CInstanceCube::~CInstanceCube()
{
	ReleaseCOM( m_pVertexBuffer );
}

ID3D10Buffer* CInstanceCube::GetVertexBuffer()
{
	return ( m_pVertexBuffer );
}

void CInstanceCube::Initialise()
{
	void* vpDevice;
	CServices::GetService( SERVICETYPE_D3DDEVICE, vpDevice );
	ID3D10Device* pDevice = static_cast< ID3D10Device* >( vpDevice );

	Vertex vertices[ 36 ] =
	{
		// Front face
		Vertex( D3DXVECTOR3(  0.5f,  0.5f, -0.5f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR2( 0.255f, 0.335f ) ), 
		Vertex( D3DXVECTOR3(  0.5f, -0.5f, -0.5f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR2( 0.255f, 0.66f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f, -0.5f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR2( 0.5f,   0.66f ) ),
		Vertex( D3DXVECTOR3(  0.5f,  0.5f, -0.5f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR2( 0.255f, 0.335f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f, -0.5f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR2( 0.5f,   0.66f ) ),
		Vertex( D3DXVECTOR3( -0.5f,  0.5f, -0.5f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR2( 0.5f,   0.335f ) ),
																								    	   
		// Back face											    	   
		Vertex( D3DXVECTOR3( -0.5f,  0.5f,  0.5f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR2( 0.755f, 0.335f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f,  0.5f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR2( 0.755f, 0.66f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f,  0.5f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR2( 1.0f,   0.66f ) ),
		Vertex( D3DXVECTOR3( -0.5f,  0.5f,  0.5f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR2( 0.755f, 0.335f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f,  0.5f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR2( 1.0f,   0.66f ) ),
		Vertex( D3DXVECTOR3(  0.5f,  0.5f,  0.5f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR2( 1.0f,   0.335f ) ),
																								    	   		  
		// Top face
		Vertex( D3DXVECTOR3( -0.5f,  0.5f, -0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  1.0f,  0.0f ), D3DXVECTOR2( 0.255f, 0.0f ) ),
		Vertex( D3DXVECTOR3( -0.5f,  0.5f,  0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  1.0f,  0.0f ), D3DXVECTOR2( 0.255f, 0.33f ) ),
		Vertex( D3DXVECTOR3(  0.5f,  0.5f,  0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  1.0f,  0.0f ), D3DXVECTOR2( 0.5f,   0.33f ) ),
		Vertex( D3DXVECTOR3( -0.5f,  0.5f, -0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  1.0f,  0.0f ), D3DXVECTOR2( 0.255f, 0.0f ) ),
		Vertex( D3DXVECTOR3(  0.5f,  0.5f,  0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  1.0f,  0.0f ), D3DXVECTOR2( 0.5f,   0.33f ) ),
		Vertex( D3DXVECTOR3(  0.5f,  0.5f, -0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f,  1.0f,  0.0f ), D3DXVECTOR2( 0.5f,   0.0f ) ),
																								    			  
		// Bottom face
		Vertex( D3DXVECTOR3( -0.5f, -0.5f, -0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f, -1.0f,  0.0f ), D3DXVECTOR2( 0.255f, 0.665f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f, -0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f, -1.0f,  0.0f ), D3DXVECTOR2( 0.255f, 1.0f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f,  0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f, -1.0f,  0.0f ), D3DXVECTOR2( 0.5f,   1.0f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f, -0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f, -1.0f,  0.0f ), D3DXVECTOR2( 0.255f, 0.665f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f,  0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f, -1.0f,  0.0f ), D3DXVECTOR2( 0.5f,   1.0f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f,  0.5f ), D3DXVECTOR3( -1.0f,  0.0f,  0.0f ), D3DXVECTOR3(  0.0f, -1.0f,  0.0f ), D3DXVECTOR2( 0.5f,   0.665f ) ),
																								    			  
		// Left face
		Vertex( D3DXVECTOR3( -0.5f,  0.5f, -0.5f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.005f, 0.335f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f, -0.5f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.005f, 0.66f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f,  0.5f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.25f,  0.66f ) ),
		Vertex( D3DXVECTOR3( -0.5f,  0.5f, -0.5f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.005f, 0.335f ) ),
		Vertex( D3DXVECTOR3( -0.5f, -0.5f,  0.5f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.25f,  0.66f ) ),
		Vertex( D3DXVECTOR3( -0.5f,  0.5f,  0.5f ), D3DXVECTOR3(  0.0f,  0.0f,  1.0f ), D3DXVECTOR3(  -1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.25f,  0.335f ) ),
																										   		  
		// Right face
		Vertex( D3DXVECTOR3(  0.5f,  0.5f,  0.5f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.505f, 0.335f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f,  0.5f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.505f, 0.66f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f, -0.5f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.75f, 0.66f ) ),
		Vertex( D3DXVECTOR3(  0.5f,  0.5f,  0.5f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.505f, 0.335f ) ),
		Vertex( D3DXVECTOR3(  0.5f, -0.5f, -0.5f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.75f, 0.66f ) ),
		Vertex( D3DXVECTOR3(  0.5f,  0.5f, -0.5f ), D3DXVECTOR3(  0.0f,  0.0f, -1.0f ), D3DXVECTOR3(  1.0f,  0.0f,  0.0f ), D3DXVECTOR2( 0.75f, 0.335f ) ),
	};

	// Fill vertex buffer description
	D3D10_BUFFER_DESC vbDesc;
	vbDesc.Usage			= D3D10_USAGE_DYNAMIC;//D3D10_USAGE_IMMUTABLE;
	vbDesc.ByteWidth		= sizeof( Vertex ) * g_VerticesPerCube;
	vbDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;//0;
	vbDesc.MiscFlags		= 0;

	// Set vertices to buffer memory
	D3D10_SUBRESOURCE_DATA viData;
	viData.pSysMem = vertices;

	// Create Buffer
	HR( pDevice->CreateBuffer( &vbDesc, &viData, &m_pVertexBuffer ) );	
}
