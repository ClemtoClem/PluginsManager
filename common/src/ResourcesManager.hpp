/**
 *    __                                                                                    
 *   /__\ ___  ___  ___  _   _ _ __ ___ ___  ___    /\/\   __ _ _ __   __ _  __ _  ___ _ __ 
 *  / \/// _ \/ __|/ _ \| | | | '__/ __/ _ \/ __|  /    \ / _` | '_ \ / _` |/ _` |/ _ \ '__|
 * / _  \  __/\__ \ (_) | |_| | | | (_|  __/\__ \ / /\/\ \ (_| | | | | (_| | (_| |  __/ |   
 * \/ \_/\___||___/\___/ \__,_|_|  \___\___||___/ \/    \/\__,_|_| |_|\__,_|\__, |\___|_|   
 *                                                                          |___/           
 * @file ResourcesManager.hpp
 * @author ClemtoClem
 * @date 06/07/2024
 */

#ifndef RESOURCEMANAGERIMPL_HPP
#define RESOURCEMANAGERIMPL_HPP

#include <string>
#include <memory>
#include <vector>
#include <mutex>
#include <stdexcept>
#include <algorithm>
#include "VariantType.hpp"

struct ResourceInfo {
	std::string name;		///< Nom de la ressource
	VariantType value;	   ///< Valeur de la ressource
	bool isLocked;		   ///< Indique si la ressource est verrouillée
};

class ResourcesManager {
private:
	std::vector<ResourceInfo> _resources;
	mutable std::mutex _mutex;

	/**
	 * @brief Trouver une ressource par son nom
	 * @param[in] resourceName Nom de la ressource
	 * @return ResourceInfo de la ressource trouvée
	 * @throw std::runtime_error si la ressource n'existe pas
	 */
	ResourceInfo &findResource(const std::string& resourceName);

public:
	static ResourcesManager *instance;

	static void createInstance();
	static ResourcesManager &getInstance();
	static ResourcesManager &setInstance(ResourcesManager *res);
	static void destroyInstance();

	/**
	 * @brief Demander une ressource spécifique par son nom
	 * @param[in] resourceName Nom de la ressource demandée
	 * @return La ressource demandée sous forme de VariantType
	 */
	VariantType &getResource(const std::string& resourceName);

	/**
	 * @brief Enregistrer une ressource spécifique
	 * @param[in] resourceName Nom de la ressource
	 * @param[in] resource La ressource à enregistrer
	 */
	void registerResource(const std::string& resourceName, const VariantType& resource);

	/**
	 * @brief Verrouiller une ressource spécifique
	 * @param[in] resourceName Nom de la ressource à verrouiller
	 */
	void lockResource(const std::string& resourceName);

	/**
	 * @brief Déverrouiller une ressource spécifique
	 * @param[in] resourceName Nom de la ressource à déverrouiller
	 */
	void unlockResource(const std::string& resourceName);
};

#endif // RESOURCEMANAGERIMPL_HPP
