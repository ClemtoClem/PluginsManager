/**
 *    ___                                          _         __ _     _                       
 *   / __\___  _ __ ___  _ __ ___   __ _ _ __   __| |___    / /(_)___| |_ ___ _ __   ___ _ __ 
 *  / /  / _ \| '_ ` _ \| '_ ` _ \ / _` | '_ \ / _` / __|  / / | / __| __/ _ \ '_ \ / _ \ '__|
 * / /__| (_) | | | | | | | | | | | (_| | | | | (_| \__ \ / /__| \__ \ ||  __/ | | |  __/ |   
 * \____/\___/|_| |_| |_|_| |_| |_|\__,_|_| |_|\__,_|___/ \____/_|___/\__\___|_| |_|\___|_|   
 *
 * @file CommendsListener.hpp
 * @author ClemtoClem
 * @date 06/07/2024                                                                                            
 */

#ifndef COMMANDS_LISTENER_HPP
#define COMMANDS_LISTENER_HPP

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>
#include "VariantType.hpp"

struct CommandInfo {
	std::string name;	   ///< Nom de la commande
	std::string alias;	  ///< Alias de la commande (simplification du mon de la commande)
	std::string description;///< Description de la commande
	size_t nb_args;		 ///< Nombre d'arguments attendus par la commande
	size_t nb_returns;	  ///< Nombre de valeurs retournées par la commande
	std::vector<VariantType> default_args; ///< Liste des arguments par défaut à passer à la commande
	std::function<std::vector<VariantType>(const std::vector<VariantType>&)> function; ///< Fonction de la commande
};

class CommandNotFoundException : public std::runtime_error {
public:
	CommandNotFoundException(const std::string& command)
		: std::runtime_error("Command not found: " + command) {}
};

class InvalidArgumentsException : public std::runtime_error {
public:
	InvalidArgumentsException(const std::string& command, size_t expected, size_t default_expected, size_t provided)
		: std::runtime_error("Invalid number of arguments for command: '" + command +
							 "' Expected: " + std::to_string(expected) + ", Default expected: " + std::to_string(default_expected) +
							 ", Provided: " + std::to_string(provided)) {}
};

class CommandsListener {
private:
	std::vector<CommandInfo> _commands;
public:
	CommandsListener();

	~CommandsListener();

protected:
	/**
	 * @brief Fonction pour ajouter des commandes
	 * @param[in] command_name Nom de la commande
	 * @param[in] description Description de la commande
	 * @param[in] nb_args Nombre d'arguments attendus par la commande
	 * @param[in] nb_returns Nombre de valeurs retournées par la commande
	 * @param[in] function Fonction à appeler pour exécuter la commande
	 * @param[in] default_args Valeurs par défaut à utiliser si aucune n'est fournie
	 * @return true si la commande a été ajoutée, false sinon
	 */
	bool addCommand(const std::string& command_name, const std::string& description, size_t nb_args, size_t nb_returns,
		std::function<std::vector<VariantType>(const std::vector<VariantType>&)> function, const std::vector<VariantType>& default_args = {});

	/**
	 * @brief Fonction pour ajouter un alias à une commande
	 * @param[in] command Nom de la commande
	 * @param[in] alias Alias à donner à la commande
	 * @return true si l'alias a été ajouté, false sinon
	 */
	bool setAlias(const std::string& command, const std::string& alias);

public:
	/**
	 * @brief Fonction savoir si c'est un alias
	 * @param[in] alias Alias de la commande
	 * @return true si c'est un alias, false sinon
	 */
	bool isAlias(const std::string& alias) const;

	/**
	 * @brief Fonction pour connaître l'alias d'une commande
	 * @param[in] command Alias de la commande
	 */
	const std::string &getAlias(const std::string& command) const;

protected:
	/**
	 * @brief Supprimer une commande
	 * @param[in] commandOrAlias Nom de la commande ou alias
	 * @return true si la commande a été supprimée, false sinon
	 */
	bool removeCommand(const std::string& commandOrAlias);

public:
	/**
	 * @brief Fonction pour connaître si une commande existe dans la liste des commandes
	 * @param[in] commandOrAlias Nom de la commande ou alias
	 * @return true si la commande existe, false sinon
	 */
	bool isCommand(const std::string& commandOrAlias) const;

	/**
	 * @brief Fonction pour lister toutes les commandes disponibles, utilse pour les diagnostics ou les interfaces utilisateur
	 * @return Liste des commandes
	 */
	std::vector<std::string> getCommands() const;

	/**
	 * @brief fonction pur connaître la description d'une commande
	 * @param[in] commandOrAlias Nom de la commande ou alias
	 * @return Description de la commande
	 */
	const std::string &getDescription(const std::string& commandOrAlias) const;

	/**
	 * @brief Fonction pour connaître le nombre de valeurs retournées par une commande
	 * @param[in] commandOrAlias Nom de la commande ou alias
	 * @return Nombre d'arguments attendus par la commande
	 */
	size_t getNbArgs(const std::string& commandOrAlias) const;

	/**
	 * @brief Fonction pour connaître le nombre de valeurs retournées par une commande
	 * @param[in] commandOrAlias Nom de la commande ou alias
	 * @return Nombre de valeurs retournées par la commande
	 */
	size_t getNbReturns(const std::string& commandOrAlias) const;

	/**
	 * @brief Fonction pour trouver les informations d'une commande
	 * @param[in] commandOrAlias Nom de la commande ou alias
	 * @return Informations de la commande
	 */
	const CommandInfo &findCommand(const std::string& commandOrAlias) const;

	/**
	 * @brief Fonction pour appeler une commande avec un nombre variable d'arguments
	 * @param[in] commandOrAlias Nom de la commande ou alias
	 * @param[in] args Liste des arguments passés à la commande
	 * @return Liste des valeurs retournées par la commande
	 */
	std::vector<VariantType> callCommand(const std::string& commandOrAlias, const std::vector<VariantType>& args);
};

#endif // COMMANDS_LISTENER_HPP