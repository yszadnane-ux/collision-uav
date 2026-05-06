# Système de Collision pour Essaim Autonome (UAV)

## Description
Ce projet implémente un système temps réel de détection des deux drones les plus proches dans un essaim de 10 000 micro-drones.  
L’objectif est d’éviter les collisions en identifiant instantanément la paire critique, dans des contraintes matérielles sévères.

## Contraintes respectées
- **Pas d’utilisation de `[ ]`** : toute la navigation mémoire se fait par arithmétique pure des pointeurs (`*(ptr + offset)` ou `ptr->membre`).
- **Allocation unique sur le tas** : un seul bloc `malloc` contigu pour les 10 000 drones.
- **Complexité optimisée** : abandon de l’approche naïve O(n²) pour une méthode en O(n log n) (tri + balayage linéaire élagué).

## Algorithme
1. Tri des drones selon la coordonnée `x` (tri fusion, O(n log n)).
2. Parcours linéaire : pour chaque drone, on regarde seulement les voisins dont l’écart en `x` est inférieur à la distance minimale courante.
3. Conservation de la paire ayant la plus petite distance euclidienne.

## Auteur
ZADNANE YOUSRA
