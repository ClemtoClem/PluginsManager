#include <iostream>
#include "../../common/src/Logger.hpp"
#include "PluginsManager.hpp"

PluginsManager::PluginsManager(const std::string& dir, const Version& mainVersion)
	: _pluginsDir(dir), _mainVersion(mainVersion) {
	if (!fs::exists(_pluginsDir)) {
		throw std::runtime_error("Plugins directory does not exist");
	}
}

PluginsManager::~PluginsManager() {
	unloadPlugins();
}

void PluginsManager::loadPlugins() {
	size_t loadedPlugins = 0;
	size_t totalPlugins = 0;
	for (const auto& entry : fs::directory_iterator(_pluginsDir)) {
		if (entry.path().extension() == ".so") {
			totalPlugins++;
			if (loadPlugin(entry.path()))
				loadedPlugins++;
		}
	}

	// Trier les plugins par priorité
	std::sort(_plugins.begin(), _plugins.end(), [](const Plugin& a, const Plugin& b) {
		return a.info.priority < b.info.priority;
	});

	LOG(Info) << loadedPlugins << "/" << totalPlugins << " plugins loaded and sorted by priority.";
}

bool PluginsManager::loadPlugin(const fs::path& path) {
	void* handle = dlopen(path.c_str(), RTLD_LAZY);
	if (!handle) {
		LOG(Error) << "Cannot open library: " << dlerror();
		return false;
	}

	create_t* create_plugin = reinterpret_cast<create_t*>(dlsym(handle, "create"));
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		LOG(Error) << "Cannot load symbol create: " << dlsym_error;
		dlclose(handle);
		return false;
	}

	destroy_t* destroy_plugin = reinterpret_cast<destroy_t*>(dlsym(handle, "destroy"));
	dlsym_error = dlerror();
	if (dlsym_error) {
		LOG(Error) << "Cannot load symbol destroy: " << dlsym_error;
		dlclose(handle);
		return false;
	}

	PluginInterface* instance = create_plugin();
	if (!instance) {
		LOG(Error) << "Failed to create plugin instance.";
		dlclose(handle);
		return false;
	}

	if (!instance->isCompatible(_mainVersion)) {
		LOG(Error) << "Plugin '" << instance->getInfo().name << "' is not compatible with main program version.";
		destroy_plugin(instance);
		dlclose(handle);
		return false;
	}

	instance->setInstances(&Logger::getInstance(), &ResourcesManager::getInstance());

	Plugin plugin = { handle, instance, instance->getInfo() };
	_plugins.push_back(std::move(plugin));

	//LOG(Info) << "Loading " << path.filename() << " name: " << instance->getInfoToString();
	return true;
}

void PluginsManager::initPlugins(int argc, char* argv[]) {
	if (_plugins.empty()) {
		return;
	}
	size_t initializedPlugins = 0;
	for (auto& plugin : _plugins) {
		if (plugin.instance->init(argc, argv) != 0) {
			LOG(Error) << "Failed to initialize plugin '" << plugin.info.name << "'";
		} else {
			initializedPlugins++;
		}
	}
	LOG(Info) << initializedPlugins << "/" << _plugins.size() << " plugins initialized.";
}

void PluginsManager::shutdownPlugins() {
	if (_plugins.empty()) {
		return;
	}
	size_t shutDownPlugins = 0;
	for (auto& plugin : _plugins) {
		if (plugin.instance->shutdown() != 0) {
			LOG(Error) << "Failed to shut down plugin '" << plugin.info.name << "'";
		} else {
			shutDownPlugins++;
		}
	}
	LOG(Info) << shutDownPlugins << "/" << _plugins.size() << " plugins shut down.";
}

void PluginsManager::unloadPlugins() {
	if (_plugins.empty()) {
		return;
	}
	size_t unloadedPlugins = 0;
	size_t nbPlugins = _plugins.size();
	for (auto& plugin : _plugins) {
		if (plugin.instance && plugin.handle) {
			destroy_t* destroy_plugin = reinterpret_cast<destroy_t*>(dlsym(plugin.handle, "destroy"));
			if (!destroy_plugin) {
				LOG(Error) << "Cannot load symbol destroy for plugin '" << plugin.info.name << "': " << dlerror();
				continue;
			}

			destroy_plugin(plugin.instance);
			dlclose(plugin.handle);
			plugin.instance = nullptr;
			plugin.handle = nullptr;
			unloadedPlugins++;
		}
	}
	_plugins.clear();
	LOG(Info) << unloadedPlugins << "/" << nbPlugins << " plugins unloaded.";
}

void PluginsManager::setVariable(const std::string& pluginName, const std::string& varName, const VariantType& value) {
	for (auto& plugin : _plugins) {
		if (plugin.info.name == pluginName) {
			plugin.instance->setVariable(varName, value);
			return;
		}
	}
	LOG(Error) << "Plugin '" << pluginName << "' not found.";
}

VariantType PluginsManager::getVariable(const std::string& pluginName, const std::string& varName) {
	for (auto& plugin : _plugins) {
		if (plugin.info.name == pluginName) {
			return plugin.instance->getVariable(varName);
		}
	}
	LOG(Error) << "Plugin '" << pluginName << "' not found.";
	return VariantType(); // Return a default value (empty variant) if plugin not found
}

template<typename T>
T PluginsManager::getValue(const std::string& pluginName, const std::string& varName) {
	VariantType value = getVariable(pluginName, varName);
	try {
		return std::get<T>(value);
	} catch (const std::bad_variant_access& e) {
		throw std::runtime_error("Failed to convert variable '" + varName + "' to type '" + typeid(T).name() + "'");
	}
}
