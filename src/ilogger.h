#pragma once

#include <string>

enum class LogMode { Info = 0, Err, Success };

class ILogger {
public:
	ILogger() = default;
	virtual ~ILogger() = default;

	// methods:
	virtual bool Init(std::string filename) noexcept = 0;
	virtual void AddMsg(LogMode lmMode, const char* szMsg, ...) const noexcept = 0;

	inline void Enable() noexcept { m_bEnabled = true; }
	inline void Disable() noexcept { m_bEnabled = false; }
	inline bool IsEnabled() const noexcept { return m_bEnabled; }

protected:
	bool m_bEnabled;
};

