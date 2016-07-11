/*-----------------------------------------------------------------------------+
| File name: ctimer.cpp							   						       |
| Date:		 6 December 2005		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| High Performance Timer for 3D Application									   |
+-----------------------------------------------------------------------------*/

#include "ctimer.h"

/*-----------------------------------------------------------------------------+
|                    Implementation of the CTimer class                        |
+-----------------------------------------------------------------------------*/

// constructor:
CTimer::CTimer():
    m_fAspect(0.0),
	m_fAspectLast(0.0),
	m_fTicksInv(0.0),
	m_fFps(0.0f),
	m_fDelta(0.0),
	m_iTicks(0),
	m_iTickStart(0),
	m_iPauseTicks(0),
	m_bFreeze(false)
{

}

// destructor:
CTimer::~CTimer() {

}

/*-----------------------------------------------------------------------------+
| Function name: CTimer::Init                                                  |
| Description: Initializes the timer,                                          |
|              returns true for succes and false for err                       |
+-----------------------------------------------------------------------------*/
bool CTimer::Init() {
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&m_iTicks) == 0) {
		return false;
	}

	m_fTicksInv = 1.0 / (double)(m_iTicks);
	QueryPerformanceCounter((LARGE_INTEGER *)&m_iTickStart);
	return true;
}

/*-----------------------------------------------------------------------------+
| Function name: CTimer::Update                                                |
| Description: Updates tick count... must be called befor drawing of the scene |
+-----------------------------------------------------------------------------*/
void CTimer::Update() {
	//if (m_bFreeze) return;
	
	m_fAspectLast = m_fAspect;

	QueryPerformanceCounter((LARGE_INTEGER *)&m_iTicks);

	m_iTicks -= m_iTickStart;
	m_fAspect = (double)m_iTicks * m_fTicksInv;
	m_fDelta = m_fAspect - m_fAspectLast;

	// calculate FPS for each second
	if ((unsigned long)m_fAspect > (unsigned long)m_fAspectLast) {
		m_fFps = (float)(1.0/m_fDelta);
	}
}

/*-----------------------------------------------------------------------------+
| Function name: CTimer::Unfreeze                                              |
| Description: Reduces "time gap" after the window was paused or so...         |
+-----------------------------------------------------------------------------*/
void CTimer::Unfreeze() {
	if (m_bFreeze) {
		Update();
		m_iTickStart += m_iTicks - m_iPauseTicks;
		m_bFreeze = false;
	}
}

// end of the file ------------------------------------------------------------+