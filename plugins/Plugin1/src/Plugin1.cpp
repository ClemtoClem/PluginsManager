/**
 *    ___ _             _       _ 
 *   / _ \ |_   _  __ _(_)_ __ / |
 *  / /_)/ | | | |/ _` | | '_ \| |
 * / ___/| | |_| | (_| | | | | | |
 * \/    |_|\__,_|\__, |_|_| |_|_|
 *                |___/           
 * 
 * @file Plugin1.cpp
 * @author ClemtoClem
 * @date 06/07/2024
 */

#include <iostream>
#include "../../../common/src/Logger.hpp"
#include "../../../common/src/PluginInterface.hpp"

const PluginInfo informations = {
	.name			= "Plugin1",
	.author			= "ClemtoClem",
	.description	= "This is a plugin for testing purpose",
	.version		= {1, 0, 0},
	.mainVersion	= {1, 0, 0},
	.priority		= 0,
	.type			= PluginType::System
};

class Plugin1 : public PluginInterface {
public:
	Plugin1() : PluginInterface(informations) {}

	~Plugin1() override {
		LOG(Info) << "Unloading '" << _info.name << "'";
	}

	int init(int argc, char* argv[]) override {
		LOG(Info) << "Initializing '" << _info.name << "'";

		// Création des variables
		addVariable("message", "Message à afficher", "Hello World");
		addVariable("level", "Niveau de log", "Info");

		// Création des commandes
		addCommand("say", "Affiche un message", 0, 0, [&](const std::vector<VariantType>& args) {
			std::string message = std::get<std::string>(getVariable("message"));
			std::string level = std::get<std::string>(getVariable("level"));
			if (level == "Debug") {
				LOG(Debug) << message;
			} else if (level == "Info") {
				LOG(Info) << message;
			} else if (level == "Success") {
				LOG(Success) << message;
			} else if (level == "Warning") {
				LOG(Warning) << message;
			} else if (level == "Error") {
				LOG(Error) << message;
			} else if (level == "Fatal") {
				LOG(Fatal) << message;
			} else {
				LOG(Info) << message;
			}
			return std::vector<VariantType>();
		});

		return 0;
	}

	int shutdown() noexcept override {
		LOG(Info) << "Shutting down '" << _info.name << "'";

		return 0;
	}
};

extern "C" PluginInterface* create() {
	return new Plugin1();
}

extern "C" void destroy(PluginInterface* plugin) {
	delete plugin;
}
