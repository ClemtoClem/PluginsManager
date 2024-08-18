/**
 *    ___ _             _       ____  
 *   / _ \ |_   _  __ _(_)_ __ |___ \ 
 *  / /_)/ | | | |/ _` | | '_ \  __) |
 * / ___/| | |_| | (_| | | | | |/ __/ 
 * \/    |_|\__,_|\__, |_|_| |_|_____|
 *                |___/               
 * 
 * @file Plugin2.cpp
 * @author ClemtoClem
 * @date 06/07/2024
 */

#include <iostream>
#include "../../../common/src/Logger.hpp"
#include "../../../common/src/PluginInterface.hpp"

const PluginInfo informations = {
	.name			= "Plugin2",
	.author			= "ClemtoClem",
	.description	= "This is a plugin for testing purpose",
	.version		= {1, 0, 0},
	.mainVersion	= {1, 0, 0},
	.priority		= 0,
	.type			= PluginType::Module
};

class Plugin2 : public PluginInterface {
public:
	Plugin2() : PluginInterface(informations) {}

	~Plugin2() override {
		LOG(Info) << "Unloading '" << _info.name << "'";
	}

	int init(int argc, char* argv[]) override {
		LOG(Info) << "Initializing '" << _info.name << "'";

		return 0;
	}

	int shutdown() noexcept override {
		LOG(Info) << "Shutting down '" << _info.name << "'";
		
		return 0;
	}
};

extern "C" PluginInterface* create() {
	return new Plugin2();
}

extern "C" void destroy(PluginInterface* plugin) {
	delete plugin;
}
