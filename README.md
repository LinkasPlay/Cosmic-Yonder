# Cosmic-Yonder
Projet d'informatique de la première année de la prépa intégré à CY-Tech.

Pour compiler sur le terminal = "make"
Pour lancer le jeu = "../bin/CosmicYonder"

Séléctionner une option sur l'écran de démarrage = flèches directionnelles
Validation du choix = touche ESPACE
Mettre le jeu en pause = touche TAB
Passer de la saisie du nom à la saisie de la graine = TAB

Commande de jeu : 

- Déplacer le personnage = flèches directionnelles
- Interaction avec monstres/objets = ESPACE / E

Expliquation gameplay du jeu :

    on commence avec l'invetaire vide
    1) l'épée/tuyau permet de frapper autour de soi, fait 50 de dégats
    2 ) le pistolet tire sur une ligne droite devant soi, fait 20 dégats (NON IMPLEMENTE)
    3) les potions de vies redonnes 1 coeurs au joueur
    4) les potions d'xp donne 1/4 de l'xp nécessaire pour ganer un niveau
    5) les clé a molette servent à réparé les machines
    6) les clés servent à ouvrir les coffres
    7) la grande clé sert à ouvrir la dernière salle

    une épée/tuyau est généré dans l'une des 5 premières salles (1/3 chance par sale, 1/1 si salle 5) (DONNEE AU DEBUT)
    une salle de boss, fermé avec une porte spéciale est générée dans les alles 10 à 25 ==> vérifié qu'il y a des portes libres autrepart pour ne pas bloquer joueur (NON IMPLEMENTE)

    on trouve forcement un pistolet dans l'un des 10 premiers coffres ouverts
    on trouve la clé du boss dans l'un des coffres entre le 10eme et 20eme
    les coffres on 40% de chance de donner une clé a molette, 30% pour une potion de vie, 30% pour une potion d'xp

    utiliser une clé à molette sur une machine permet de gagner 3minutes en plus au chrono total
    il faut utiliser 2 clé a molette sur les grandes machines pour les réparés
    il faut réparé 3 grandes machines pour avoir la vrai fin

    il y a quatre type de monstres :
        niv 1 : 80 pv, 10 xp
        niv 2 : 180 pv, 20 xp | drop : clé * 0.05
        niv 3 : 260 pv, 30 xp | drop : clé * 0.1, clé a molette * 0.1
        niv 5 (boss) : 1000 pv, 0 xp => fin du jeu


