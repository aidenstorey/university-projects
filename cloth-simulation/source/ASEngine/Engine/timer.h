//********************
//
// ASEngine
//
// (c) Aiden Storey
//
//********************

#ifndef GUARD_AIDEN_STOREY_TIMER_H_201408211217
#define GUARD_AIDEN_STOREY_TIMER_H_201408211217

// Prototypes
class CTimer
{
	// Member Function
public:
	// Constructors
	CTimer();

	// Accessors
	float GetGameTime() const;
	float GetDeltaTime() const;

	// Other
	void Reset();
	void Start();
	void Stop();
	void Tick();

protected:
private:

	// Member Variables
public:
protected:
private:
	double	m_dSecondsPerCount;
	double	m_dTimeDelta;

	__int64 m_iBaseTime;
	__int64 m_iPausedTime;
	__int64 m_iStopTime;
	__int64 m_iPrevTime;
	__int64 m_iCurrTime;

	bool	m_bStopped;
};

#endif //GUARD_AIDEN_STOREY_TIMER_H_201408211217