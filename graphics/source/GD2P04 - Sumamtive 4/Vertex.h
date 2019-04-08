

#ifndef VERTEX_H
#define VERTEX_H

// Local Includes
#include "d3dUtil.h"

struct Vertex
{
	Vertex(){}

	Vertex( D3DXVECTOR3 _pos, D3DXVECTOR3 _tangent, D3DXVECTOR3 _normal, D3DXVECTOR2 _texC ) :
		pos( _pos ), tangent( _tangent ), normal( _normal ), texC( _texC ) {}

	Vertex(float x, float y, float z, float tx, float ty, float tz, float nx, float ny, float nz, float u, float v) : 
		pos(x,y,z), tangent( tx, ty, tz ), normal(nx,ny,nz), texC(u,v){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

struct VertexID
{
	VertexID(){}

	VertexID( D3DXVECTOR3 _pos, D3DXVECTOR3 _tangent, D3DXVECTOR3 _normal, D3DXVECTOR2 _texC, UINT _texID ) :
		pos( _pos ), tangent( _tangent ), normal( _normal ), texC( _texC ), texID( _texID ) {}

	VertexID(float x, float y, float z, float tx, float ty, float tz, float nx, float ny, float nz, float u, float v, UINT _texID ) : 
		pos(x,y,z), tangent( tx, ty, tz ), normal(nx,ny,nz), texC(u,v), texID( _texID ){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
	UINT		texID;
};

struct VInstance
{
	VInstance() {}

	VInstance( D3DXVECTOR3 _pos, UINT _texID ) :
		pos( _pos ), texID( _texID ) {}
	
	VInstance( float x, float y, float z, UINT _texID ) :
		pos( x, y, z ), texID( _texID ) {}
		
	D3DXVECTOR3 pos;
	UINT		texID;
};

struct VBillboard
{
	VBillboard() {}

	VBillboard( D3DXVECTOR3 _pos, D3DXVECTOR2 _size, UINT _texID ) :
		pos( _pos ), size( _size ), texID( _texID ) {}

	VBillboard( float x, float y, float z, float sx, float sy, UINT _texID ) :
		pos( x, y, z ), size( sx, sy ), texID( _texID ) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR2 size;
	UINT		texID;
};

struct VQuad
{
	VQuad() {}
	
	VQuad( D3DXVECTOR3 _pos, D3DXVECTOR2 _texC ) :
		pos( _pos ), texC( texC ) {};

	D3DXVECTOR3 pos;
	D3DXVECTOR2 texC;
};

#endif // VERTEX_H

