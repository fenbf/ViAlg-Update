/*-----------------------------------------------------------------------------+
| File name: clog.hpp							   						       |
| Date:		 5 October 2005													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Log system that is represented by CLog class.   							   |
| Based on the NeHe tutorials and code to the "Focus on 3D Terrain Programming"|
| by Trent Polack.															   |
+-----------------------------------------------------------------------------*/

#ifndef CLOG_HPP
#define CLOG_HPP

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <windows.h>

enum LOG_MODE { lmNormal = 0, lmError, lmSuccess };

const char LogColors[3][7] = { "000000",    // for lmNormal
                               "aa6666",    // for lmError
							   "66aa66" };  // for lmSuccess

/*-----------------------------------------------------------------------------+
|                                The CLog class                                |
+-----------------------------------------------------------------------------*/
class CLog {
public:
	CLog();
	CLog::CLog(char *szFileName);
	~CLog();

	// methods:
	bool Init(const char *szFileName);
	void AddMsg(LOG_MODE lmMode, char *szMsg, ...);
	
	inline void Enable() { m_bEnabled = true; }
	inline void Disable() { m_bEnabled = false; }
	inline bool IsEnabled() { return m_bEnabled; }
private:
	const char *GetColorName(LOG_MODE lmMode);

	char m_szFileName[256];
	bool m_bEnabled;
};

// there will be one log per application...
// it should be done by usigng a singleton class but for a while it is not
extern CLog g_Log;

#endif // CLOG_HPP

// end of file ----------------------------------------------------------------+