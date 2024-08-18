/**
 *    __                             
 *   / /  ___   __ _  __ _  ___ _ __ 
 *  / /  / _ \ / _` |/ _` |/ _ \ '__|
 * / /__| (_) | (_| | (_| |  __/ |   
 * \____/\___/ \__, |\__, |\___|_|   
 *             |___/ |___/           
 *
 * @file Logger.hpp
 * @author ClemtoClem
 * @date 06/07/2024
*/

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <memory>
#include <mutex> // Ajout du header pour std::mutex

#define LOG_FILE "debug.log"

enum LogLevel {
	Debug,
	Info,
	Success,
	Warning,
	Error,
	Fatal,
};

class Logger {
public:
	static Logger *instance;

	using Stream = std::ostringstream;
	using Buffer_p = std::unique_ptr<Stream, std::function<void(Stream*)>>;

	Logger();

	virtual ~Logger();

	static void createInstance();
	static Logger &getInstance();
	static Logger &setInstance(Logger *logger);
	static void destroyInstance();

	void enableWriteInTerminal();
	void disableWriteInTerminal();

	void setMinLevel(LogLevel level);
	void setMaxLevel(LogLevel level);

	void write(const std::string &msg);
	void write_separation(char sig = '=');
	void write_break_line();

	Buffer_p log(LogLevel level, const std::string &file, int line) {
		_msg_level = level;
		_msg_file = file;
		_msg_line = line;
		return Buffer_p(new Stream, [&](Stream* st) {
			write(st->str());
			delete st;  // Explicitly delete the stream to prevent memory leaks
		});
	}

private:
	std::ofstream _file;
	LogLevel _minLevel, _maxLevel;
	bool _writeInTerminal;

	LogLevel _msg_level;
	std::string _msg_file;
	int _msg_line;

	std::mutex _mutex; // Ajout du mutex pour la synchronisation

	static std::string getLabel(LogLevel type);
	static std::string getColor(LogLevel type);

	Logger(const Logger &);

	Logger &operator=(const Logger &) = delete;
};

#define LOG(level) (*Logger::getInstance().log(level, __FILE__, __LINE__))
#define LOG_SEPARATION(sig) Logger::getInstance().write_separation(sig);
#define LOG_BREAK_LINE() Logger::getInstance().write_break_line();

inline void LogTest() {
	LOG_SEPARATION('-');
	LOG(Debug) << "This is a debug message";
	LOG(Info) << "This is an information";
	LOG(Success) << "This is an success";
	LOG(Warning) << "This is a warning message";
	LOG(Error) << "This is an error";
	LOG(Fatal) << "This is an fatal error";

	LOG_BREAK_LINE();
	LOG(Success) << "Successful log test";
}

#endif // LOGGER_HPP
