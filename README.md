# Travail Pratique 3 : Le Jeu Circles

## Description

Le jeu Circles est un projet d'arcade développé dans le cadre du cours INF3135 à l'Université du Québec à Montréal. Ce jeu met le joueur au défi de contrôler un joueur en rotation pour éviter des obstacles circulaires générés dynamiquement autour d'un point central.

### Objectifs :
- Survivre le plus longtemps possible.
- Accumuler un score basé sur le temps de survie.

Le jeu utilise les bibliothèques SDL2 et ses extensions pour offrir une expérience visuelle et sonore immersive.


## Auteurs

- Mamadou Lamine Dabo
- Naouel Ouazar


## Dépendances

Le jeu nécessite l'installation des bibliothèques suivantes :

- [SDL2](https://www.libsdl.org/)
- [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
- [SDL2_gfx](http://www.ferzkopp.net/joomla/content/view/19/14/)

Ces bibliothèques gèrent les graphismes, les polices, les images, les effets sonores et la manipulation de données JSON. Consultez leur documentation pour l'installation.


## Fonctionnement

### Compilation

Dans le répertoire du projet, exécutez la commande suivante pour compiler les fichiers sources et générer l'exécutable circles :

```sh
make
```


### Lancement de l'Application

Après une compilation réussie, lancez le jeu avec la commande suivante :

```sh
./circles
```


## Comment Jouer

- But du Jeu : Survivez le plus longtemps possible en évitant les obstacles circulaires générés dynamiquement.
- Contrôles :
  - Flèche Gauche (LEFT) : Tourner le triangle vers la gauche.
  - Flèche Droite (RIGHT) : Tourner le triangle vers la droite.
  - Echap (ESCAPE) : Quitter le jeu.

### Description du Gameplay

Le joueur contrôle un triangle qui tourne autour d’un point central. Les obstacles, sous forme d'arcs ou de cercles, se déplacent vers le centre. Le joueur doit éviter de toucher les obstacles pour accumuler un meilleur score.


## Captures d’Écran

### Menu de Jeu

![Exemple de l’écran de jeu](images/menu.png){width=300}

### Jeu en cours

![Exemple de l’écran de jeu](images/enjeux.png){width=300}

### Fin de Partie

![Écran affiché lorsque le joueur perd](images/gameOver.png){width=300}

---

## Plateformes Supportées

Le jeu a été testé et fonctionne sur les plateformes suivantes :

- Ubuntu 20.04 LTS  
  Version : 20.04
- Windows 10  
  Version : 10
- macOS Catalina  
  Version : 10.15

---

## Références

- [Documentation SDL2](https://wiki.libsdl.org/)
- [Documentation Jansson](https://digip.org/jansson/)
- [Documentation SDL2_gfx](http://www.ferzkopp.net/joomla/content/view/19/14/)

---

## Division des Tâches

| Tâche                                   | Auteur                |
|----------------------------------------|-----------------------|
| Gestion du menu                        | Naouel Ouazar        |
| Animation du joueur                    | Mamadou Lamine Dabo  |
| Affichage de la scène                  | Naouel Ouazar        |
| Affichage du chronomètre               | Mamadou Lamine Dabo  |
| Animation des murs                     | Naouel Ouazar        |
| Détection des collisions               | Mamadou Lamine Dabo  |
| Affichage d’une partie terminée        | Naouel Ouazar        |
| Gestion de la musique principale       | Mamadou Lamine Dabo  |
| Gestion des sons (navigation dans menu)| Naouel Ouazar        |
| Gestion de la musique de fin de partie | Mamadou Lamine Dabo  |

---

## État du Projet

Le projet fonctionne sans bugs majeurs ni fuites de mémoire, à l'exception de quelques fuites mineures dans SDL. Les fonctionnalités principales suivantes sont entièrement opérationnelles :

- Gestion du menu
- Animation du joueur
- Musique principale

Les tâches restantes incluent :
- Affichage de la scène
- Gestion du chronomètre
- Animation des murs
- Détection des collisions
- Affichage de la fin de partie
- Gestion des sons supplémentaires

---

### Remarques
