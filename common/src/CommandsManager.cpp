#include "CommandsListener.hpp"
#include "Logger.hpp"

CommandsListener::CommandsListener() = default;
CommandsListener::~CommandsListener() = default;

bool CommandsListener::addCommand(const std::string& command_name, const std::string& description, size_t nb_args, size_t nb_returns,
	std::function<std::vector<VariantType>(const std::vector<VariantType>&)> function, const std::vector<VariantType>& default_args)
{
	if (default_args.size() > nb_args) {
		LOG(Error) << "Too many default arguments provided";
		return false;
	}	
	_commands.push_back({command_name, "", description, nb_args, nb_returns, default_args, function});
	return true;
}

bool CommandsListener::setAlias(const std::string& command, const std::string& alias) {
	for (auto& cmd : _commands) {
		if (cmd.name == command) {
			cmd.alias = alias;
			return true;
		}
	}
	return false;
}

bool CommandsListener::isAlias(const std::string& alias) const {
	for (const auto& cmd : _commands) {
		if (cmd.alias == alias) {
			return true;
		}
	}
	return false;
}

const std::string& CommandsListener::getAlias(const std::string& command) const {
	for (const auto& cmd : _commands) {
		if (cmd.name == command) {
			return cmd.alias;
		}
	}
	throw CommandNotFoundException(command);
}

bool CommandsListener::removeCommand(const std::string& commandOrAlias) {
	auto it = std::remove_if(_commands.begin(), _commands.end(), [&](const CommandInfo& cmd) {
		return cmd.name == commandOrAlias || cmd.alias == commandOrAlias;
	});
	if (it != _commands.end()) {
		_commands.erase(it, _commands.end());
		return true;
	}
	return false;
}

bool CommandsListener::isCommand(const std::string& commandOrAlias) const {
	try {
		findCommand(commandOrAlias);
		return true;
	} catch (...) {
		return false;
	}
}

std::vector<std::string> CommandsListener::getCommands() const {
	std::vector<std::string> command_names;
	for (const auto& cmd : _commands) {
		command_names.push_back(cmd.name);
	}
	return command_names;
}

const std::string& CommandsListener::getDescription(const std::string& commandOrAlias) const {
	return findCommand(commandOrAlias).description;
}

size_t CommandsListener::getNbArgs(const std::string& commandOrAlias) const {
	return findCommand(commandOrAlias).nb_args;
}

size_t CommandsListener::getNbReturns(const std::string& commandOrAlias) const {
	return findCommand(commandOrAlias).nb_returns;
}

const CommandInfo& CommandsListener::findCommand(const std::string& commandOrAlias) const {
	auto it = std::find_if(_commands.begin(), _commands.end(), [&](const CommandInfo& cmd) {
		return cmd.name == commandOrAlias || cmd.alias == commandOrAlias;
	});
	if (it == _commands.end()) {
		throw CommandNotFoundException(commandOrAlias);
	}
	return *it;
}

std::vector<VariantType> CommandsListener::callCommand(const std::string& commandOrAlias, const std::vector<VariantType>& args) {
	const auto& cmd = findCommand(commandOrAlias);
	
	// Vérifier si le nombre d'arguments fournis est correct
	if (args.size()-cmd.default_args.size() > cmd.nb_args) {
		throw InvalidArgumentsException(commandOrAlias, cmd.nb_args, cmd.default_args.size(), args.size());
	}

	// Préparer les arguments en utilisant les valeurs par défaut si nécessaire
	std::vector<VariantType> full_args = cmd.default_args;
	for (size_t i = 0; i < args.size(); ++i) {
		full_args[i] = args[i];
	}

	// Appeler la fonction avec les arguments complétés
	return cmd.function(args);
}
