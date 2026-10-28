#pragma once
#include <vector>
#include <string>

class LogManager {
public:
	static std::vector<std::string> logs;

	static void addLog(const std::string& log);
		
	static void addDelimiter();

	static std::vector<std::string> getLogs();
};