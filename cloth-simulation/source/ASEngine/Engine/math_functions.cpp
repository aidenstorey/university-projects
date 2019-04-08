//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#define NOMINMAX

// Library Includes
#include <algorithm>

// This Include
#include "math_functions.h"

namespace Math
{
	// Implementation
	float DegreesToRadians( const float& _kf )
	{
		return ( ( PI / 180.0f ) * _kf );
	}

	float RadiansToDegrees( const float& _kf )
	{
		return ( ( 180.0f / PI ) * _kf );
	}

	D3DXCOLOR RGBtoD3D( const float& _kfR, const float& _kfG, const float& _kfB )
	{
		float fScalar = 1.0f / 255.0f;

		return ( D3DXCOLOR( fScalar * _kfR, fScalar * _kfG, fScalar * _kfB, 1.0f ) );
	}

	float Magnitude( const D3DXVECTOR2& _kr )
	{
		return ( sqrt( ( _kr.x * _kr.x ) + ( _kr.y * _kr.y ) ) );
	}

	float Magnitude( const D3DXVECTOR3& _kr )
	{
		return ( sqrt( ( _kr.x * _kr.x ) + ( _kr.y * _kr.y ) + ( _kr.z * _kr.z ) ) );
	}

	float AngleBetween( const D3DXVECTOR2& _krA, const D3DXVECTOR2& _krB )
	{
		float fReturn = 0.0f;

		float fDot = D3DXVec2Dot( &_krA, &_krB );
		float fDet = _krA.x * _krB.y - _krA.y * _krB.x;

		fReturn = atan2( fDet, fDot );

		return ( fReturn );
	}

	float AngleBetween( const D3DXVECTOR3& _krA, const D3DXVECTOR3& _krB )
	{
		float fReturn = 0.0f;

		float fDot = D3DXVec3Dot( &_krA, &_krB );

		if( fDot != 0.0f )
		{
			float fMagA = Magnitude( _krA );
			float fMagB = Magnitude( _krB );

			float fTheta = fDot / ( fMagA * fMagB );

			fReturn = acosf( fTheta );
		}

		return ( fReturn );
	}

	float DistanceBetween( const D3DXVECTOR2& _vA, const D3DXVECTOR2& _vB )
	{
		float fX = _vA.x - _vB.x;
		float fY = _vA.y - _vB.y;

		return ( sqrt( fX * fX + fY * fY ) );
	}

	float DistanceBetween( const D3DXVECTOR3& _vA, const D3DXVECTOR3& _vB )
	{
		float fX = _vA.x - _vB.x;
		float fY = _vA.y - _vB.y;
		float fZ = _vA.z - _vB.z;

		return ( sqrt( fX * fX + fY * fY + fZ * fZ ) );
	}

	D3DXVECTOR2 RotatePointAroundOrigin( const D3DXVECTOR2& _vPoint, const float& _fAngle )
	{
		float fSin = sin( _fAngle );
		float fCos = cos( _fAngle );

		return( D3DXVECTOR2( _vPoint.x * fCos - _vPoint.y * fSin, _vPoint.x * fSin + _vPoint.y * fCos ) );
	}

	D3DXVECTOR2 RotatePointAroundPoint( const D3DXVECTOR2& _vOrigin, const D3DXVECTOR2& _vPoint, const float& _fAngle )
	{
		return ( RotatePointAroundOrigin( _vPoint - _vOrigin, _fAngle ) + _vOrigin );
	}

	D3DXVECTOR3 CrossProduct( const D3DXVECTOR3& _krA, const D3DXVECTOR3& _krB )
	{
		D3DXVECTOR3 vReturn;

		vReturn.x = ( _krA.y * _krB.z ) - ( _krA.z * _krB.y );
		vReturn.y = ( _krA.z * _krB.x ) - ( _krA.x * _krB.z );
		vReturn.z = ( _krA.x * _krB.y ) - ( _krA.y * _krB.x );

		return ( vReturn );
	}

	bool IsLineSegmentIntersection( const D3DXVECTOR2& _startA, const D3DXVECTOR2& _endA, const D3DXVECTOR2& _startB, const D3DXVECTOR2& _endB )
	{
		bool bReturn{ false };

		float fA1 = _endA.y - _startA.y;
		float fB1 = _startA.x - _endA.x;
		float fC1 = fA1 * _startA.x + fB1 * _startA.y;

		float fA2 = _endB.y - _startB.y;
		float fB2 = _startB.x - _endB.x;
		float fC2 = fA2 * _startB.x + fB2 * _startB.y;

		float fDelta = fA1 * fB2 - fA2 * fB1;
		if( fDelta != 0 )
		{
			bReturn = true;

			float fX = ( fB2 * fC1 - fB1 * fC2 ) / fDelta;
			float fY = ( fA1 * fC2 - fA2 * fC1 ) / fDelta;

			if( fX < std::min( _startA.x, _endA.x ) || fX > std::max( _startA.x, _endA.x ) ||
				fX < std::min( _startB.x, _endB.x ) || fX > std::max( _startB.x, _endB.x ) ||
				fY < std::min( _startA.y, _endA.y ) || fY > std::max( _startA.y, _endA.y ) ||
				fY < std::min( _startB.y, _endB.y ) || fY > std::max( _startB.y, _endB.y ) )
			{
				bReturn = false;
			}
		}

		return ( bReturn );
	}

	ESideOfPlane CalculateSideOfPlane( const TPlane& _krPlane, const D3DXVECTOR3& _krPoint )
	{
		ESideOfPlane eReturn{ ON_PLANE };

		float fD = D3DXVec3Dot( &_krPlane.normal, &_krPlane.point );
		float fVal = D3DXVec3Dot( &_krPlane.normal, &_krPoint ) - fD;

		if( fVal > 0 )
		{
			eReturn = INFRONT_PLANE;
		}
		else if( fVal < 0 )
		{
			eReturn = BEHIND_PLANE;
		}

		return ( eReturn );
	}

	float DistancePointToPlane( const TPlane& _krPlane, const D3DXVECTOR3& _krPoint, D3DXVECTOR3* _pOut )
	{
		float sn = -D3DXVec3Dot( &_krPlane.normal, &( _krPoint - _krPlane.point ) );
		float sd = D3DXVec3Dot( &_krPlane.normal, &_krPlane.normal );
		float sb = sn / sd;

		*_pOut = _krPoint + sb * _krPlane.normal;
		return ( DistanceBetween( _krPoint, *_pOut ) );
	}

	bool IsLineTriangleIntersection( const D3DXVECTOR3& _krStart, const D3DXVECTOR3& _krEnd, const D3DXVECTOR3& _krTriangleA, const D3DXVECTOR3& _krTriangleB, const D3DXVECTOR3& _krTriangleC )
	{
		D3DXVECTOR3 vLine{ _krEnd - _krStart };
		D3DXVECTOR3 vSTA{ _krTriangleA - _krStart };
		D3DXVECTOR3 vSTB{ _krTriangleB - _krStart };
		D3DXVECTOR3 vSTC{ _krTriangleC - _krStart };

		D3DXVECTOR3 vM;
		D3DXVec3Cross( &vM, &vLine, &vSTC );

		float fU = D3DXVec3Dot( &vSTB, &vM );
		float fV = -D3DXVec3Dot( &vSTA, &vM );

		D3DXVECTOR3 vUV;
		D3DXVec3Cross( &vUV, &vLine, &vSTB );
		float fW = D3DXVec3Dot( &vUV, &vSTA );

		if( std::signbit( fU ) != std::signbit( fV ) )
		{
			return ( false );
		}

		if( std::signbit( fU ) != std::signbit( fW ) )
		{
			return ( false );
		}

		return ( true );
	}
}