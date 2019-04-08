//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_MATH_H_201409211703
#define GUARD_AIDEN_STOREY_MATH_H_201409211703

// Library Includes
#include <D3DX10.h>

namespace Math
{
	// Constants
	const float PI = 3.14159265f;

	enum ESideOfPlane
	{
		ON_PLANE,
		INFRONT_PLANE,
		BEHIND_PLANE,
	};

	// Prototypes
	struct TPlane
	{
		TPlane() {}
		TPlane( const D3DXVECTOR3& _krPoint, const D3DXVECTOR3& _krNormal ) :
			point( _krPoint ), normal( _krNormal )
		{}

		D3DXVECTOR3 point;
		D3DXVECTOR3 normal;
	};

	struct TTriangle
	{
		D3DXVECTOR3 v0;
		D3DXVECTOR3 v1;
		D3DXVECTOR3 v2;
	};

	struct TRay
	{
		D3DXVECTOR3 v0;
		D3DXVECTOR3 v1;
	};

	float DegreesToRadians( const float& _kf );
	float RadiansToDegrees( const float& _kf );

	D3DXCOLOR RGBtoD3D( const float& _kfR, const float& _kfG, const float& _kfB );

	float Magnitude( const D3DXVECTOR2& _kr );
	float Magnitude( const D3DXVECTOR3& _kr );
	float AngleBetween( const D3DXVECTOR2& _krA, const D3DXVECTOR2& _krB );
	float AngleBetween( const D3DXVECTOR3& _krA, const D3DXVECTOR3& _krB );
	float DistanceBetween( const D3DXVECTOR2& _vA, const D3DXVECTOR2& _vB );
	float DistanceBetween( const D3DXVECTOR3& _vA, const D3DXVECTOR3& _vB );
	D3DXVECTOR2 RotatePointAroundOrigin( const D3DXVECTOR2& _vPoint, const float& _fAngle );
	D3DXVECTOR2 RotatePointAroundPoint( const D3DXVECTOR2& _vOrigin, const D3DXVECTOR2& _vPoint, const float& _fAngle );

	D3DXVECTOR3 CrossProduct( const D3DXVECTOR3& _krA, const D3DXVECTOR3& _krB );

	bool IsLineSegmentIntersection( const D3DXVECTOR2& _startA, const D3DXVECTOR2& _endA, const D3DXVECTOR2& _startB, const D3DXVECTOR2& _endB );

	ESideOfPlane CalculateSideOfPlane( const TPlane& _krPlane, const D3DXVECTOR3& _krPoint );
	float DistancePointToPlane( const TPlane& _krPlane, const D3DXVECTOR3& _krPoint, D3DXVECTOR3* _pOut );

	bool IsLineTriangleIntersection( const D3DXVECTOR3& _krStart, const D3DXVECTOR3& _krEnd, const D3DXVECTOR3& _krTriangleA, const D3DXVECTOR3& _krTriangleB, const D3DXVECTOR3& _krTriangleC );
};

#endif // GUARD_AIDEN_STOREY_MATH_H_201409211703