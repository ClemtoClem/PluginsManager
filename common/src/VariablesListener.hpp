/**
 *                  _       _     _               __ _     _                       
 * /\   /\__ _ _ __(_) __ _| |__ | | ___  ___    / /(_)___| |_ ___ _ __   ___ _ __ 
 * \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|  / / | / __| __/ _ \ '_ \ / _ \ '__|
 *  \ V / (_| | |  | | (_| | |_) | |  __/\__ \ / /__| \__ \ ||  __/ | | |  __/ |   
 *   \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/ \____/_|___/\__\___|_| |_|\___|_|   
 *                                                                                 
 * @file VariablesListener.hpp
 * @author ClemtoClem
 * @date 06/07/2024
 */

#ifndef VARIABLES_LISTENER_HPP
#define VARIABLES_LISTENER_HPP

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>
#include "VariantType.hpp"

struct VariableInfo {
	std::string name;		///< Nom de la variable
	std::string description; ///< Description de la variable
	VariantType value;	   ///< Valeur de la variable
};

class VariableNotFoundException : public std::runtime_error {
public:
	VariableNotFoundException(const std::string& variable)
		: std::runtime_error("Variable not found: " + variable) {}
};

class VariablesListener {
private:
	std::vector<VariableInfo> _variables;
public:
	VariablesListener();

	~VariablesListener();

protected:
	/**
	 * @brief Fonction pour ajouter des variables
	 * @param[in] variable_name Nom de la variable
	 * @param[in] description Description de la variable
	 * @param[in] value Valeur de la variable
	 * @return true si la variable a été ajoutée, false sinon
	 */
	bool addVariable(const std::string& variable_name, const std::string& description, const VariantType& value);

	/**
	 * @brief Fonction pour supprimer une variable
	 * @param[in] variable_name Nom de la variable
	 * @return true si la variable a été supprimée, false sinon
	 */
	bool removeVariable(const std::string& variable_name);

public:
	/**
	 * @brief Fonction pour modifier la valeur d'une variable
	 * @param[in] variable_name Nom de la variable
	 * @param[in] value Valeur de la variable
	 * @return true si la variable a été modifiée, false sinon
	 */
	bool setVariable(const std::string& variable_name, const VariantType& value);

	/**
	 * @brief Fonction pour récupérer la valeur d'une variable
	 * @param[in] variable_name Nom de la variable
	 * @return Valeur de la variable
	 * @throw std::runtime_error si la variable n'existe pas
	 */
	VariantType getVariable(const std::string& variable_name) const;

	/**
	 * @brief Fonction pour vérifier si la variable existe
	 * @param[in] variable_name Nom de la variable
	 * @return true si la variable existe, false sinon
	 */
	bool isVariable(const std::string& variable_name) const;

	/**
	 * @brief Fonction pour récupérer la liste des variables
	 * @return Liste des variables
	 */
	std::vector<std::string> getVariables() const;

	/**
	 * @brief Fonction pour récupérer la description d'une variable
	 * @param[in] variable_name Nom de la variable
	 * @return Description de la variable
	 */
	const std::string &getDescription(const std::string& variable_name) const;

};


#endif // VARIABLES_LISTENER_HPP