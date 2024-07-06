#include "VariablesListener.hpp"
#include "Logger.hpp"

VariablesListener::VariablesListener() = default;
VariablesListener::~VariablesListener() = default;

bool VariablesListener::addVariable(const std::string& variable_name, const std::string& description, const VariantType& value) {
	if (isVariable(variable_name)) {
		return false; // La variable existe déjà
	}
	_variables.push_back({variable_name, description, value});
	return true;
}

bool VariablesListener::removeVariable(const std::string &variable_name) {
	auto it = std::remove_if(_variables.begin(), _variables.end(), [&](const VariableInfo& var) {
		return var.name == variable_name;
	});
	if (it != _variables.end()) {
		_variables.erase(it, _variables.end());
		return true;
	}
	return false;
}

bool VariablesListener::setVariable(const std::string& variable_name, const VariantType& value) {
	for (auto& var : _variables) {
		if (var.name == variable_name) {
			var.value = value;
			return true;
		}
	}
	return false;
}

VariantType VariablesListener::getVariable(const std::string& variable_name) const {
	for (const auto& var : _variables) {
		if (var.name == variable_name) {
			return var.value;
		}
	}
	throw VariableNotFoundException(variable_name);
}

bool VariablesListener::isVariable(const std::string& variable_name) const {
	for (const auto& var : _variables) {
		if (var.name == variable_name) {
			return true;
		}
	}
	return false;
}

std::vector<std::string> VariablesListener::getVariables() const {
	std::vector<std::string> variable_names;
	for (const auto& var : _variables) {
		variable_names.push_back(var.name);
		LOG(Info) << "Variable: " << var.name << " - " << VariantToString(var.value);
	}
	return variable_names;
}

const std::string& VariablesListener::getDescription(const std::string& variable_name) const {
	for (const auto& var : _variables) {
		if (var.name == variable_name) {
			return var.description;
		}
	}
	throw VariableNotFoundException(variable_name);
}
