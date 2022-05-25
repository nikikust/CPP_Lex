#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <sstream>


void enable_colors(bool x);
std::string colorText(int color, int background = 40); // black, red, green, yellow, blue, purple, lightblue, white;
std::string colorText();

class LogDuration {
public:
	explicit LogDuration(const std::string& msg = "", bool scale_up = false)
		: message(msg + ": ")
		, start(std::chrono::steady_clock::now())
		, scale_up(scale_up)
	{
	}

	~LogDuration() {
		auto finish = std::chrono::steady_clock::now();
		auto dur = finish - start;
		std::ostringstream os;
		if (scale_up)
		{
			os << message
			   << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
			   << " milliseconds";
		}
		else
		{
			os << message
			   << std::chrono::duration_cast<std::chrono::microseconds>(dur).count()
			   << " microseconds";
		}
		std::cerr << colorText(32) << os.str() << colorText() << std::endl;
	}
private:
	std::string message;
	std::chrono::steady_clock::time_point start;

	bool scale_up = 0;
};

#ifndef UNIQ_ID
#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)
#endif

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__LINE__){message};
