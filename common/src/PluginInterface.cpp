#include "PluginInterface.hpp"

std::string to_string(const Version &version) {
	return std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch);
}

/* ------------------------------------------------------------------------------ */

std::string to_string(PluginType type) {
    switch (type) {
		case PluginType::Admin:
			return "Admin";
		case PluginType::System:
			return "System";
		case PluginType::Core:
			return "Core";
		case PluginType::User:
			return "User";
		default:
			return "Default";
	}
}

/* ------------------------------------------------------------------------------ */

PluginInterface::PluginInterface(const std::string &name, const std::string &author, const std::string &description, const Version &version, const Version &mainVersion, int priority, PluginType type) noexcept
	: _info{name, author, description, version, mainVersion, priority, type} {}

PluginInterface::PluginInterface(const PluginInfo &info) noexcept : _info(info) {}

void PluginInterface::setInstances(Logger *logger, ResourcesManager *res) noexcept {
	Logger::setInstance(logger);
	ResourcesManager::setInstance(res);
}

std::string PluginInterface::getInfoToString() const noexcept {
	return "Plugin Name: '" + _info.name + "'" +
			", Author: " + _info.author +
			", Version: " + to_string(_info.version) +
			", Main Version: " + to_string(_info.mainVersion) +
			", Priority: " + std::to_string(_info.priority) +
			", Type: " + to_string(_info.type);
}

bool PluginInterface::isCompatible(const Version& mainVersion) const noexcept {
	return mainVersion.major == _info.mainVersion.major;
}