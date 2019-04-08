#ifndef GUARD_AIDEN_STOREY_DIAMSQ_UNIT_H_20140621
#define GUARD_AIDEN_STOREY_DIAMSQ_UNIT_H_20140621

// Local Includes
#include "2d_point.h"

// Prototype
struct TDSUnit
{
	TDSUnit() {};
	TDSUnit( TPoint _fl, TPoint _nr ) :
		fl( _fl ), nr( _nr ) {}

	TPoint fl;
	TPoint nr;
};

#endif // GUARD_AIDEN_STOREY_DIAMSQ_UNIT_H_20140621