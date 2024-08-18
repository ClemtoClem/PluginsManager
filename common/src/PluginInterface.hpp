/**
 *     ___ _             _          _____       _             __                
 *   / _ \ |_   _  __ _(_)_ __     \_   \_ __ | |_ ___ _ __ / _| __ _  ___ ___ 
 *  / /_)/ | | | |/ _` | | '_ \     / /\/ '_ \| __/ _ \ '__| |_ / _` |/ __/ _ \
 * / ___/| | |_| | (_| | | | | | /\/ /_ | | | | ||  __/ |  |  _| (_| | (_|  __/
 * \/    |_|\__,_|\__, |_|_| |_| \____/ |_| |_|\__\___|_|  |_|  \__,_|\___\___|
 *                |___/                                                        
 *
 * @file PluginInterface.hpp
 * @author ClemtoClem
 * @date 06/07/2024
 */


#ifndef PLUGIN_INTERFACE_HPP
#define PLUGIN_INTERFACE_HPP

#include <cstdint>
#include <variant>
#include <string>
#include <map>
#include "Logger.hpp"
#include "CommandsListener.hpp"
#include "VariablesListener.hpp"
#include "ResourcesManager.hpp"
#include "VariantType.hpp"

struct Version {
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
};

std::string to_string(const Version& version);

/* ------------------------------------------------------------------------------ */

enum class PluginType {
	System,     ///< Plugin système
	Core,		///< Plugin pour le service de base
	Module,		///< Plugin module
	Default 	///< Plugin par défaut
};

std::string to_string(PluginType type);

/* ------------------------------------------------------------------------------ */

struct PluginInfo {
	std::string name;			///< Nom du plugin
	std::string author;			///< Auteur du plugin
	std::string description;	///< Description du plugin
	Version version;			///< Version du plugin
	Version mainVersion;		///< Version du programme principal pour lequel le plugin a été programmé
	int priority; 				///< Priorité d'initialisation du plugin, le plus propriétaire est celui avec la plus petite priorité
	PluginType type;			///< Type du plugin
};

/* ------------------------------------------------------------------------------ */

class PluginInterface: public CommandsListener, public VariablesListener {
protected:
	PluginInfo _info;
public:
	/**
	 * @brief Constructeur de PluginInterface
	 * @param[in] name Nom du plugin
	 * @param[in] author Auteur du plugin
	 * @param[in] description Description du plugin
	 * @param[in] version Version du plugin
	 * @param[in] mainVersion Version du programme principal pour lequel le plugin a été programmé
	 * @param[in] priority Priorité du plugin, le plus propriétaire est celui avec la plus petite priorité
	 */
	PluginInterface(const std::string &name, const std::string &author, const std::string &description, const Version& version, const Version& mainVersion, int priority = 0, PluginType type = PluginType::Default) noexcept;

	/**
	 * @brief Constructeur de PluginInterface
	 * @param[in] info Informations du plugin
	 */
	PluginInterface(const PluginInfo& info) noexcept;
	
	virtual ~PluginInterface() {}

	/**
	 * @brief Fonction pour définir les instances de Logger pour quelles soient commune entre le programme principal et les différents plugins
	 * @param[in] logger Instance de Logger du programme principal
	 * @param[in] res Instance de ResourcesManager du programme principal
	 */
	virtual void setInstances(Logger* logger, ResourcesManager *res) noexcept;

	/**
	 * @brief Fonction pour initialiser le plugin
	 * @param[in] argc Nombre d'arguments passés au programme principal
	 * @param[in] argv Tableau des arguments passés au programme principal
	 * @return 0 si l'initialisation s'est bien passée, autre sinon
	 */
	virtual int init(int argc, char* argv[]) = 0;

	/**
	 * @brief Fonction pour arréter le plugin avant de le détruire
	 * @return 0 si l'arrêt s'est bien passé, autre sinon
	 */
	virtual int shutdown() noexcept = 0;

	/**
	 * @brief Fonction pour récupérer les informations du plugin
	 * @return Informations du plugin
	 */
	const PluginInfo& getInfo() const noexcept { return _info; }

	/**
	 * @brief Fonction pour convertir les informations du plugin en une chaîne de caractères
	 * @return Chaîne de caractères contenant les informations du plugin
	 */
	std::string getInfoToString() const noexcept;

	/**
	 * @brief Fonction pour récupérer le nom du plugin
	 * @return Nom du plugin
	 */
	const std::string& getName() const noexcept { return _info.name; }

	/**
	 * @brief Fonction pour récupérer l'auteur du plugin
	 * @return Auteur du plugin
	 */
	const std::string& getAuthor() const noexcept { return _info.author; }

	/**
	 * @brief Fonction pour récupérer la description du plugin
	 * @return Description du plugin
	 */
	const std::string& getDescription() const noexcept { return _info.description; }

	/**
	 * @brief Fonction pour récupérer la version du plugin
	 * @return Version du plugin
	 */
	const Version& getVersion() const noexcept { return _info.version; }

	/**
	 * @brief Fonction pour récupérer la version du programme principal pour lequel le plugin a été programmé
	 * @return Version du programme principal pour lequel le plugin a été programmé
	 */
	const Version& getMainVersion() const noexcept { return _info.mainVersion; }

	/**
	 * @brief Fonction pour récupérer la priorité du plugin
	 * @return Priorité du plugin
	 */
	int getPriority() const noexcept { return _info.priority; }
	
	/**
	 * @brief Fonction pour vérifier si le plugin est compatible avec le programme principal
	 * @param[in] mainVersion Version du programme principal
	 */
	bool isCompatible(const Version& mainVersion) const noexcept;
};

typedef PluginInterface* create_t();
typedef void destroy_t(PluginInterface*);

#endif // PLUGIN_INTERFACE_HPP
