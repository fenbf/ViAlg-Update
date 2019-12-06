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

enum class LOG_MODE { lmNormal = 0, lmError, lmSuccess };

/*-----------------------------------------------------------------------------+
|                                The CLog class                                |
+-----------------------------------------------------------------------------*/
class CLog {
public:
	CLog() noexcept;
	CLog(const char* szFileName) noexcept;

	// methods:
	bool Init(const char* szFileName) noexcept;
	void AddMsg(LOG_MODE lmMode, const char* szMsg, ...) noexcept;
	
	inline void Enable() noexcept{ m_bEnabled = true; }
	inline void Disable() noexcept { m_bEnabled = false; }
	inline bool IsEnabled() const noexcept { return m_bEnabled; }
private:
	const char *GetClassName(LOG_MODE lmMode) const noexcept;

	char m_szFileName[256];
	bool m_bEnabled;
};

// there will be one log per application...
// it should be done by usigng a singleton class but for a while it is not
extern CLog g_Log;

#endif // CLOG_HPP

// end of file ----------------------------------------------------------------+