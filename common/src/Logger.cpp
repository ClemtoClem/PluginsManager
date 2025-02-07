#include "Logger.hpp"

Logger *Logger::instance = nullptr;

void Logger::createInstance() {
	if (instance!= nullptr) {
		throw std::runtime_error("Logger already initialized");
	}
	instance = new Logger();
}

Logger &Logger::getInstance()
{
	if (instance == nullptr) {
		throw std::runtime_error("Logger not initialized");
	}
	return *instance;
}

Logger &Logger::setInstance(Logger *logger) {
	if (instance!= nullptr) {
		throw std::runtime_error("Logger already initialized");
	}
	instance = logger;
	return *instance;
}

void Logger::destroyInstance() {
	if (instance!= nullptr) {
		delete instance;
		instance = nullptr;
	}
}