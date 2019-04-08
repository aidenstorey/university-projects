//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

// Library Includes
#include <Windows.h>

// This Include 
#include "timer.h"

// Implementation
CTimer::CTimer() : 
	m_dSecondsPerCount( 0.0 ), m_dTimeDelta( -1.0 ), 
	m_iBaseTime( 0 ), m_iPausedTime( 0 ), m_iPrevTime( 0 ), m_iCurrTime( 0 ),
	m_bStopped( false )
{
	__int64 iCountsPerSecond;
	QueryPerformanceFrequency( ( LARGE_INTEGER* ) &iCountsPerSecond );
	m_dSecondsPerCount = 1.0 / static_cast< double >( iCountsPerSecond );
}

float CTimer::GetGameTime() const
{
	float fReturn = 0.0f;

	if( m_bStopped )
	{
		fReturn = static_cast< float >( ( m_iStopTime - m_iBaseTime ) * m_dSecondsPerCount );
	}
	else
	{
		fReturn = static_cast< float >( ( ( m_iCurrTime - m_iPausedTime ) - m_iBaseTime ) * m_dSecondsPerCount );
	}

	return ( fReturn );
}

float CTimer::GetDeltaTime() const
{
	return ( static_cast< float >( m_dTimeDelta ) );
}

void CTimer::Reset()
{
	__int64 iCurrTime;
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &iCurrTime );

	m_iBaseTime = iCurrTime;
	m_iPrevTime = iCurrTime;
	m_iStopTime = 0;
	m_bStopped	= false;
}

void CTimer::Start()
{
	__int64 iStartTime;
	QueryPerformanceCounter( ( LARGE_INTEGER* ) &iStartTime );

	if( m_bStopped )
	{
		m_iPausedTime += ( iStartTime - m_iStopTime );

		m_iPrevTime = iStartTime;
		m_iStopTime = 0;
		m_bStopped	= false;
	}
}

void CTimer::Stop()
{
	if( !m_bStopped )
	{
		__int64 iCurrTime;
		QueryPerformanceCounter( ( LARGE_INTEGER* ) &iCurrTime );

		m_iStopTime = iCurrTime;
		m_bStopped	= true;
	}
}

void CTimer::Tick()
{
	if( m_bStopped )
	{
		m_dTimeDelta = 0.0;
	}
	else
	{
		__int64 iCurrTime;
		QueryPerformanceCounter( ( LARGE_INTEGER* ) &iCurrTime );
		m_iCurrTime = iCurrTime;

		m_dTimeDelta = ( m_iCurrTime - m_iPrevTime ) * m_dSecondsPerCount;

		m_iPrevTime = m_iCurrTime;

		if( m_dTimeDelta < 0.0 )
		{
			m_dTimeDelta = 0.0;
		}
	}
}