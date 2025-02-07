/**
 *    __                                  
   * / /   ___    __ _   __ _   ___  _ __ 
 *  / /   / _ \  / _` | / _` | / _ \| '__|
 * / /___| (_) || (_| || (_| ||  __/| |   
 * \____/ \___/  \__, | \__, | \___||_|   
 *               |___/  |___/          
 * 
 * @file Logger.hpp
 * @author ClemtoClem
 * @date 06/02/2025
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <mutex>

// ================================================================
// Logger constants
#define LOG_FILE "debug.log"
#define LOG_SIZE_SEPARATOR 80
#define LOG_TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
	

/**
 * @brief Logger class
 * @ingroup Logger
 *
 * This class provides functionality to log messages to a file and console with thread-safety.
 */
class Logger {
public:
	// Logging levels
	enum Level {
		None,
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

	static Logger *instance;

	static void createInstance();

	/**
	 * @brief Get the singleton instance of the Logger.
	 * @return The Logger instance.
	 */
	static Logger &getInstance();

	static Logger &setInstance(Logger *logger);

	static void destroyInstance();

	/**
	 * @brief Set the logging level.
	 * @param[in] level The minimum logging level.
	 */
	static void setLevel(Level level) {
		getInstance()._logLevel = level;
	}

	/**
	 * @brief Get the current logging level.
	 * @return The current logging level.
	 */
	static Level getLevel() {
		return getInstance()._logLevel;
	}

	/**
	 * @brief Outputs a separator line to the log.
	 * @param[in] character The character used for the separator.
	 */
	static void separator(const std::string& character = "-") {
		std::string line(character.size() * LOG_SIZE_SEPARATOR, character[0]);
		getInstance().writeSeparator(line);
	}

	/**
	 * @brief Stream class for formatted logging.
	 */
	class LogStream {
	public:
		LogStream(const std::string& filename, int line, Level level)
			: _filename(filename), _line(line), _level(level) {}

		~LogStream() {
			Logger::getInstance().writeLog(_filename, _line, _level, _stream.str());
		}

		template <typename T>
		LogStream& operator<<(const T& value) {
			_stream << value;
			return *this;
		}

	private:
		std::string _filename;
		int _line;
		Level _level;
		std::ostringstream _stream;
	};

private:
	Logger() : _logLevel(Info), _mutex() {}
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	~Logger() {
		_mutex.unlock();
	}

	void writeLog(const std::string& filename, int line, Level level, const std::string& message) {
		_mutex.lock();

		if (level < _logLevel || level == None) {
			_mutex.unlock();
			return;
		}

		std::string timestamp = getCurrentTimestamp();
		std::string levelStr = levelToString(level);
		std::string logMessage = "[" + filename + ":" + std::to_string(line) + "] " + message + "\n";

		// Write to console
		std::cout << "[" << timestamp << "] " << levelColor(level) << levelStr << "\033[0m " << logMessage;

		// Write to file
		std::ofstream logFile(LOG_FILE, std::ios::app);
		if (logFile.is_open()) {
			logFile << "[" << timestamp << "] " << levelStr << " " << logMessage;
			logFile.close();
		} else {
			std::cerr << "Failed to open log file: " << LOG_FILE << std::endl;
		}

		_mutex.unlock();

		// Handle fatal errors
		if (level == Fatal) {
			std::cerr << "Fatal error occurred. Exiting program." << std::endl;
			exit(1);
		}
	}

	void writeSeparator(const std::string& separatorLine) {
		_mutex.lock();

		std::ofstream logFile(LOG_FILE, std::ios::app);
		if (logFile.is_open()) {
			logFile << separatorLine << "\n";
			logFile.close();
		} else {
			std::cerr << "Failed to open log file: " << LOG_FILE << std::endl;
		}

		_mutex.unlock();
	}

	std::string levelToString(Level level) const {
		switch (level) {
			case Debug: return "Debug";
			case Info:  return "Info ";
			case Warning: return "WARN ";
			case Error: return "Error";
			case Fatal: return "Fatal";
			default: return "";
		}
	}

	std::string levelColor(Level level) const {
		switch (level) {
			case Debug: return "\033[36m";   // Cyan
			case Info:  return "\033[32m";   // Green
			case Warning: return "\033[1;33m"; // Yellow
			case Error: return "\033[1;31m";  // Red
			case Fatal: return "\033[4;35m";  // Purple
			default: return "\033[0m";       // Default
		}
	}

	std::string getCurrentTimestamp() const {
		std::time_t now = std::time(nullptr);
		char buffer[100];
		std::strftime(buffer, sizeof(buffer), LOG_TIMESTAMP_FORMAT, std::localtime(&now));
		return std::string(buffer);
	}

	Level _logLevel;
	std::mutex _mutex;
};

// Helper macro for logging
#define LOG(level) Logger::LogStream(__FILE__, __LINE__, Logger::level)

#endif // LOGGER_HPP
