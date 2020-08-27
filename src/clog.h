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

#include "ilogger.h"


/*-----------------------------------------------------------------------------+
|                                The CLog class                                |
+-----------------------------------------------------------------------------*/
class CLog : public ILogger {
public:
	CLog() = default;
	explicit CLog(std::string filename) noexcept;

	// methods:
	bool Init(std::string filename) noexcept override;
	void AddMsg(LogMode lmMode, const char* szMsg, ...) const noexcept override;

private:
	const char *GetLogClassName(LogMode lmMode) const noexcept;

	std::string m_filename;
};

#endif // CLOG_HPP

// end of file ----------------------------------------------------------------+