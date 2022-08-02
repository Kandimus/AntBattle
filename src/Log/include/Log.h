#pragma once

#include <mutex>
#include <string>
#include "export.h"

namespace AntBattle {

class Log
{
// Singleton
public:
	enum Level
	{
		Error,
		Warning,
		Info,
		Debug,
	};


	virtual ~Log() = default;
	Log(const Log&) = delete;
	Log(const Log&&) = delete;

	static Log& instance();

private:
	Log() = default;
	Log& operator=(Log&);

public:
	void put(Level level, const std::string& text);
	void setDir(const std::string& dir);
	Level level(void) const { return m_level; }
	void setLevel(Level level) { m_level = level; }

private:
	std::mutex m_mutex;
	std::string m_dir = "";
	Level m_level = Level::Info;
};

};

std::string format(const char *fmt, ...);
