#include "Logger.hpp"

Logger *Logger::instance = nullptr;

Logger::Logger() : _minLevel(Debug), _maxLevel(Fatal), _writeInTerminal(false) {
	_file.open(LOG_FILE, std::ios::app);
	if (!_file.is_open()) {
		throw std::runtime_error("Unable to open log file");
	}
}

Logger::~Logger() {
	write_separation();
	if (_file.is_open()) {
		_file.close();
	}
}

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

void Logger::enableWriteInTerminal() {
	_writeInTerminal = true;
}

void Logger::disableWriteInTerminal() {
	_writeInTerminal = false;
}

void Logger::setMinLevel(LogLevel level) {
	_minLevel = level;
}

void Logger::setMaxLevel(LogLevel level) {
	_maxLevel = level;
}

void Logger::write(const std::string &msg) {
	std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for the duration of this scope
	if (_msg_level >= _minLevel && _msg_level <= _maxLevel) {
		std::ostringstream ss;

		// time
		time_t now = time(nullptr);
		struct tm* timeinfo = localtime(&now);
		char date[20];
		strftime(date, sizeof(date), "%y-%m-%d %H:%M:%S", timeinfo);

		// Logger
		ss  << "[" << getLabel(_msg_level) << "] " << date << " - " << _msg_file << ":" << _msg_line << " - ";
		ss << msg << std::endl;

		if (_file.is_open()) {
			_file << ss.str();
		}
		if (_writeInTerminal) {
			std::cerr << getColor(_msg_level) << ss.str() << "\e[0m";
		}
	}
}

void Logger::write_separation(char sig) {
	std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for the duration of this scope
	std::string sep(60, sig);

	if (_file.is_open()) {
		_file << sep << std::endl;
	}
	if (_writeInTerminal) {
		std::cerr << sep << std::endl;
	}
}

void Logger::write_break_line() {
	std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for the duration of this scope
	if (_file.is_open()) {
		_file << std::endl;
	}
	if (_writeInTerminal) {
		std::cerr << std::endl;
	}
}

std::string Logger::getLabel(LogLevel type) {
	switch (type) {
		case Debug:		return " DEBUG ";
		case Info:		return " INFO  ";
		case Success:	return "SUCCESS";
		case Warning:	return "WARNING";
		case Error:		return " ERROR ";
		case Fatal:		return " FATAL ";
		default:		return "UNKNOWN";
	}
}

std::string Logger::getColor(LogLevel type) {
	switch (type) {
		case Debug:		return "\e[1;37m"; // white
		case Info:		return "\e[0;34m"; // blue
		case Success:	return "\e[1;32m"; // green
		case Warning:	return "\e[1;33m"; // yellow
		case Error:		return "\e[1;31m"; // red
		case Fatal:		return "\e[4;31m"; // red underlined
		default:		return "\e[0m";	// reset
	}
}
