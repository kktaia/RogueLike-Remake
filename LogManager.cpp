#include "LogManager.h"

std::vector<std::string> LogManager::logs;

void LogManager::addLog(const std::string& log) {
	logs.push_back(log);
}

void LogManager::addDelimiter() {
	logs.push_back("--------------------");
}

std::vector<std::string> LogManager::getLogs() {
	return logs;
}