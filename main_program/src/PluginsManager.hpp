/**
 *    ___ _             _                                                      
 *   / _ \ |_   _  __ _(_)_ __  ___    /\/\   __ _ _ __   __ _  __ _  ___ _ __ 
 *  / /_)/ | | | |/ _` | | '_ \/ __|  /    \ / _` | '_ \ / _` |/ _` |/ _ \ '__|
 * / ___/| | |_| | (_| | | | | \__ \ / /\/\ \ (_| | | | | (_| | (_| |  __/ |   
 * \/    |_|\__,_|\__, |_|_| |_|___/ \/    \/\__,_|_| |_|\__,_|\__, |\___|_|   
 *                |___/                                        |___/           
 * 
 * @file PluginsManager.hpp
 * @author ClemtoClem
 * @date 06/07/2024
 */

#ifndef PLUGINSMANAGER_HPP
#define PLUGINSMANAGER_HPP

#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include <map>
#include <variant>
#include <stdexcept>
#include <memory>
#include "../../common/src/PluginInterface.hpp"

namespace fs = std::filesystem;

struct Plugin {
	void* handle;
	PluginInterface* instance;
	PluginInfo info;
};

class PluginsManager {
private:
	std::string	_pluginsDir;
	Version		_mainVersion;
	std::vector<Plugin> _plugins;

	// Helper functions
	bool loadPlugin(const fs::path& path);

public:
	PluginsManager(const std::string& dir, const Version &mainVersion);
	~PluginsManager();

	void loadPlugins();

	void initPlugins(int argc, char* argv[]);

	void shutdownPlugins();
	
	void unloadPlugins();

	void setVariable(const std::string& pluginName, const std::string& varName, const VariantType& value);
	VariantType getVariable(const std::string& pluginName, const std::string& varName);

	template<typename T>
	T getValue(const std::string& pluginName, const std::string& varName);

	// Iterator support to iterate over loaded plugins
	auto begin() { return _plugins.begin(); }
	auto end() { return _plugins.end(); }
};

#endif // PLUGINSMANAGER_HPP
