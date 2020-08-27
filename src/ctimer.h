/*-----------------------------------------------------------------------------+
| File name: ctimer.hpp							   						       |
| Date:		 6 December 2005		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| High Performance Timer for 3D Application									   |
+-----------------------------------------------------------------------------*/

#ifndef CTIMER_HPP
    #define CTIMER_HPP 

#include <windows.h>

class CTimer {
public:
	CTimer();
	~CTimer();

	bool Init();
	void Update();
	void Freeze()     { m_bFreeze = true; m_iPauseTicks = m_iTicks; }
	void Unfreeze();
	bool IsFreezed()  { return m_bFreeze; }
	float GetFps()    { return m_fFps; }
	double GetTime()  { return m_fAspect; } // time in second since start of the program
	double GetDelta() { return m_fDelta; }
	double GetLast()  { return m_fAspectLast; }
private:
	double m_fAspect;         // aspect between actual number of ticks and num of ticks per second
	double m_fAspectLast;
	double m_fTicksInv;       // 1/num_of_ticks_per_second
	float m_fFps;             // number of frames per seconds...
	double m_fDelta;          // m_fAspect - m_fAspectLast
	__int64 m_iTicks;         // actual num of the ticks since start
	__int64 m_iTickStart;     // start :)
	__int64 m_iPauseTicks; 
	bool m_bFreeze;
};

extern CTimer g_Timer;	// one global timer...

#endif // CTIMER_HPP

// end of file ----------------------------------------------------------------+