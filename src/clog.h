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

#include <string>

enum class LogMode { Info = 0, Err, Success };

/*-----------------------------------------------------------------------------+
|                                The CLog class                                |
+-----------------------------------------------------------------------------*/
class CLog {
public:
	CLog() { }
	explicit CLog(std::string filename) noexcept;

	// methods:
	bool Init(std::string filename) noexcept;
	void AddMsg(LogMode lmMode, const char* szMsg, ...) noexcept;
	
	inline void Enable() noexcept{ m_bEnabled = true; }
	inline void Disable() noexcept { m_bEnabled = false; }
	inline bool IsEnabled() const noexcept { return m_bEnabled; }

	static CLog* Instance() { static CLog onlyInstance; return &onlyInstance; }

private:
	const char *GetLogClassName(LogMode lmMode) const noexcept;

	std::string m_filename;
	bool m_bEnabled;
};

#endif // CLOG_HPP

// end of file ----------------------------------------------------------------+