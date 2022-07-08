#pragma once

#include <mutex>
#include <string>
#include "export.h"

namespace AntBattle {

class Log
{
// Singleton
public:
	virtual ~Log() = default;
	Log(const Log&) = delete;
	Log(const Log&&) = delete;

	static Log& instance();

private:
	Log() = default;
	Log& operator=(Log&);

public:
	void put(const std::string& text);
	void setDir(const std::string& dir);

private:
	std::mutex m_mutex;
	std::string m_dir = "";
};

};

std::string format(const char *fmt, ...);
