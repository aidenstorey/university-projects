//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_VERTEX_H_201409211155
#define GUARD_AIDEN_STOREY_VERTEX_H_201409211155

// Library Includes
#include <D3DX10.h>

// Prototypes
struct VDefault2D
{
	VDefault2D( const D3DXVECTOR2& _pos, const D3DXCOLOR& _color ) :
		pos( _pos ), color( _color )
	{}

	VDefault2D( const float& _x, const float& _y, const float& _r, const float& _g, const float& _b, const float& _a ) :
		pos( _x, _y ), color( _r, _g, _b, _a )
	{}

	D3DXVECTOR2 pos;
	D3DXCOLOR	color;
};

struct VDefault3D
{
	VDefault3D( const D3DXVECTOR3& _pos, const D3DXCOLOR& _color ) :
		pos( _pos ), color( _color )
	{}

	VDefault3D( const float& _x, const float& _y, const float& _z, const float& _r, const float& _g, const float& _b, const float& _a ) :
		pos( _x, _y, _z ), color( _r, _g, _b, _a )
	{}

	D3DXVECTOR3 pos;
	D3DXCOLOR	color;
};

struct VTextured2D
{
	VTextured2D( const D3DXVECTOR2& _pos, const D3DXVECTOR2& _texc ) :
		pos( _pos ), texc( _texc )
	{}

	VTextured2D( const float& _x, const float& _y, const float& _u, const float& _v ) :
		pos( _x, _y ), texc( _u, _v )
	{}

	D3DXVECTOR2 pos;
	D3DXVECTOR2 texc;
};

struct VTextured3D
{
	VTextured3D( const D3DXVECTOR3& _pos, const D3DXVECTOR2& _texc ) :
		pos( _pos ), texc( _texc )
	{}

	VTextured3D( const float& _x, const float& _y, const float& _z, const float& _u, const float& _v ) :
		pos( _x, _y, _z ), texc( _u, _v )
	{}
	D3DXVECTOR3 pos;
	D3DXVECTOR2 texc;
};

#endif // GUARD_AIDEN_STOREY_VERTEX_H_201409211155