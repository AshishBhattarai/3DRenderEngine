#ifndef SLOGGER_HPP
#define SLOGGER_HPP

#define CSLOG(...) SLogger::getInstance().printToConsole(__FILE__,	__LINE__,	__VA_ARGS__)
#define FSLOG(...) SLogger::getInstance().printToFile(__FILE__,__LINE__,	__VA_ARGS__)
#define SLOG(...) SLogger::getInstance().printToFileConsole(__FILE__,	__LINE__, __VA_ARGS__)

#include <fstream>
#include <iostream>

#include "utils.hpp"

// Simple Logger

// A singleton class to print errors/warnings to console and file
// uses varadic templates
class SLogger {
private:
	std::ofstream file;
	static constexpr const char* logFile = "logs.txt";

	SLogger():
			file(logFile, std::ofstream::app)
	{
		if(!file.is_open())
			printToConsole(__FILE__,	__LINE__,	"Failed to open log file", logFile);
	}

	~SLogger() {
		if(file.is_open())
			file.close();
	}

	SLogger(const SLogger&) = delete;
	SLogger& operator=(const SLogger&) = delete;

	// print the message on give output strema
	template<typename... Args>
	void print(std::ostream& out, std::string fname, size_t line, Args&&... args) {
		// file name and line number
		fname = fname.substr(fname.find_last_of('/') + 1, fname.length());
		fname += (" in " + std::to_string(line) + ":");

		out << Utils::currentDateTime() << ", file: " << fname;
		((out << " " << args), ... ) << std::endl;
	}

public:
	static SLogger& getInstance() {
		static SLogger logger;
		return logger;
	}

	// print on console
	template<typename... Args>
	void printToConsole(std::string fname, size_t line, Args&&... args) {
		print(std::cerr, fname, line, args...);
	}

	// print on file
	template<typename... Args>
	void printToFile(std::string fname, size_t line, Args&&... args) {
		print(file, fname, line, args...);
	}

	// print on both file and console
	template<typename... Args>
	void printToFileConsole(std::string fname, size_t line, Args&&... args) {
		print(file, fname, line, args...);
		print(std::cerr, fname, line, args...);
	}
};

#endif