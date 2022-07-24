#include <ctime>
#include <stdarg.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "Log.h"

std::string format(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	std::vector<char> v(1024);
	while (true) {
		va_list args2;
		va_copy(args2, args);
		int res = vsnprintf(v.data(), v.size(), fmt, args2);
		if ((res >= 0) && (res < static_cast<int>(v.size())))
		{
			va_end(args);
			va_end(args2);
			return std::string(v.data());
		}
		size_t size;
		if (res < 0)
			size = v.size() * 2;
		else
			size = static_cast<size_t>(res) + 1;
		v.clear();
		v.resize(size);
		va_end(args2);
	}
}

using namespace AntBattle;

Log& Log::instance()
{
	static Log Singleton;
	return Singleton;
}

void Log::setDir(const std::string& dir)
{
#ifdef _WIN32
	char sep = '\\';
#else
	char sep = '/'
#endif

	std::lock_guard<std::mutex> guard(m_mutex);

	m_dir = dir;

	if (m_dir.back() != sep) {
		m_dir += sep;
	}
}

void Log::put(Level level, const std::string& text)
{
	if (level > m_level) {
		return;
	}

	std::time_t t = std::time(nullptr);
	std::tm* dt = std::localtime(&t);
	std::string filename = m_dir + format("%08x.log", t / (24 * 60 * 60));
	std::string fulltext = format("%02i.%02i.%04i %02i:%02i:%02i %s\n",
								  dt->tm_mday, dt->tm_mon + 1, dt->tm_year + 1900,
								  dt->tm_hour, dt->tm_min, dt->tm_sec,
								  text.c_str());

	std::lock_guard<std::mutex> guard(m_mutex);

	std::ofstream file(filename, std::ios_base::app);
	if (file.is_open()) {
		file << fulltext;
		file.close();
	}

	std::cout << fulltext;
}
