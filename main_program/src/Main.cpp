#include <iostream>
#include <sstream>
#include <filesystem>
#include <dlfcn.h>
#include "../../common/src/Logger.hpp"
#include "../../common/src/ResourcesManager.hpp"
#include "../../common/src/PluginInterface.hpp"
#include "PluginsManager.hpp"

namespace fs = std::filesystem;

std::string to_string(const std::vector<std::string>& vec) {
	std::stringstream ss;
	for (size_t i = 0; i < vec.size(); ++i) {
		ss << vec[i];
		if (i < vec.size() - 1) {
			ss << ", ";
		}
	}
	return ss.str();
}

int main(int argc, char *argv[]) {
	Logger::createInstance();
	Logger::getInstance().enableWriteInTerminal();

	ResourcesManager::createInstance();

	try {

		const std::string pluginDir = "./plugins";
		const Version mainVersion = {1, 0, 0};
	
		PluginsManager manager(pluginDir, mainVersion);
		manager.loadPlugins();
		manager.initPlugins(argc, argv);

		for (auto& plugin : manager) {
			LOG(Info) << plugin.instance->getInfoToString();
			LOG(Info) << "Description: " << plugin.instance->getDescription();
			LOG(Info) << "Variables: " << to_string(plugin.instance->getVariables());
			LOG(Info) << "Commands: " << to_string(plugin.instance->getCommands());
		}

		LOG(Info) << "....";

		manager.shutdownPlugins();
		manager.unloadPlugins();
	} catch (const std::exception& e) {
		LOG(Error) << "Exception: " << e.what();
		return 1;
	}


	ResourcesManager::destroyInstance();
	Logger::destroyInstance();

	return 0;
}
