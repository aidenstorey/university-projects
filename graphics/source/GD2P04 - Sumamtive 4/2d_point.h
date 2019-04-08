#ifndef GUARD_AIDEN_STOREY_2D_POINT_H_20140621
#define GUARD_AIDEN_STOREY_2D_POINT_H_20140621

struct TPoint
{
	TPoint() {};
	TPoint( int _x, int _y ) :
		x( _x ), y( _y ) {}

	TPoint operator + ( const TPoint& _kr )
	{
		return( TPoint( x + _kr.x, y + _kr.y ) );
	}

	TPoint operator - ( const TPoint& _kr )
	{
		return ( TPoint( x - _kr.x, y - _kr.y ) );
	}
	
	template< typename T >
	TPoint operator * ( const T& _kr )
	{
		return ( TPoint( x * _kr, y * _kr ) );
	}

	template< typename T >
	TPoint operator / ( const T& _kr )
	{
		return ( TPoint( x / _kr, y / _kr ) );
	}

	int x;
	int y;
};

#endif // GUARD_AIDEN_STOREY_2D_POINT_H_20140621