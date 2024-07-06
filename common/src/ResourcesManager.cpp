#include "ResourcesManager.hpp"

ResourcesManager *ResourcesManager::instance = nullptr;

ResourceInfo &ResourcesManager::findResource(const std::string &resourceName) {
	auto it = std::find_if(_resources.begin(), _resources.end(), [&resourceName](const ResourceInfo& res) {
		return res.name == resourceName;
	});
	if (it == _resources.end()) {
		throw std::runtime_error("Resource not found: " + resourceName);
	}
	return *it;
}

void ResourcesManager::createInstance() {
	if (instance!= nullptr) {
		throw std::runtime_error("ResourcesManager already initialized");
	}
	instance = new ResourcesManager();
}

ResourcesManager &ResourcesManager::getInstance() {
	if (instance == nullptr) {
		throw std::runtime_error("ResourcesManager not initialized");
	}
	return *instance;
}

ResourcesManager &ResourcesManager::setInstance(ResourcesManager *res) {
	if (instance!= nullptr) {
		throw std::runtime_error("ResourcesManager already initialized");
	}
	instance = res;
	return *instance;
}

void ResourcesManager::destroyInstance() {
	if (instance!= nullptr) {
		delete instance;
		instance = nullptr;
	}
}

VariantType &ResourcesManager::getResource(const std::string &resourceName) {
	std::lock_guard<std::mutex> lock(_mutex);
	auto& resource = findResource(resourceName);
	if (resource.isLocked) {
		throw std::runtime_error("Resource is locked: " + resourceName);
	}
	return resource.value;
}

void ResourcesManager::registerResource(const std::string& resourceName, const VariantType& resource) {
	std::lock_guard<std::mutex> lock(_mutex);
	_resources.push_back({resourceName, resource, false});
}

void ResourcesManager::lockResource(const std::string &resourceName) {
	std::lock_guard<std::mutex> lock(_mutex);
	auto& resource = findResource(resourceName);
	resource.isLocked = true;
}

void ResourcesManager::unlockResource(const std::string &resourceName) {
	std::lock_guard<std::mutex> lock(_mutex);
	auto& resource = findResource(resourceName);
	resource.isLocked = false;
}
