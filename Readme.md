# Description

Projet de création et de gestion de plugin en utilisant une approche flexible.
Ce projet a été développé dans le but d'ajouter une approche modulaire à une application C++.

## Options
- Échange de ressources entre le programme principal et les plugins.
- Possibilité d'instancier des variables et des commandes auxquelles on leur attribue un nom.
- Accès et modification des variables du plugin par leur nom et par le nom de la variable.
- Accès aux commandes du plugin par leur nom et par le nom de la commande.
- Chaque commande est suivie d'une description, d'un nombre maximum de paramètres fixe, d'un nombre fixe de valeurs renvoyées et de paramètres par défaut optionnels.

## Information
La gestion des ressources dans le programme principal est sécurisée grâce à l'utilisation de mutex permettant une interaction sécurisée et contrôlée entre les plugins et les ressources partagées.
Cela garantit que les plugins suivent les règles de concurrence et évite les conflits potentiels sur l'utilisation des ressources.

Tous les plugins doivent dépendre de la classe de base PluginInterface.

Les codes sources des plugins doivent être dans le répertoire '.plugins', dans le répertoire nom du plugin puis dans le répertoire 'src', exemple './plugins/Plugin1/src/'
Tous les plugins doivent avoir leur propre Makefile qui leur permet d'être compilés et générés le fichier .so
Tous les codes sources communs au programme principal et aux plugins sont enregistrés dans le répertoire './common'.
Tous les codes sources appartenant à un plugin ou au programme principal sont dans leur répertoire './src'