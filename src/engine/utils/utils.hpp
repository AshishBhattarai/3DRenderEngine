#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>

namespace Utils {

	// get current data and time
	inline const std::string currentDateTime() {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t tt = std::chrono::system_clock::to_time_t(now);
		std::stringstream ss;
		ss << std::put_time(std::localtime(&tt), "%c");
		return ss.str();
	}
}

#endif