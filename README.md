# Projet - Programmation embarquée

## Infos en vrac sur projet
- accélromètre
- appli avec le bluethott
- servomoteur et courant continu
- IHM avec le LCD

- broche 0 et 1 occupée en permanence
- émulateur de port pour le bluetoot pour pouvoir émuler le port 0 1 
- pile 9V pour courant continu sur servomoteur
module bluetooth supplémentaire
- accéléromètre doit être sur la platine et quand on orietne la platine, il faut que l'accéléromètre aille dans un sens ou dans l'autre,  avec une vitesse en fonction de l'inclinaison
- LCD parallèle
- sur l'appli mobile, on utilise l'accéléromètre du téléphone pour controler l'accélérètre, renvoyer vitesse de rotation du moteur
- faire système qui fait qu'on prend le dessus quand on utilise l'appli du téléphone, autrement il faut que ce soit avec la platine
- afficher sur téléphone vitesse de rotation
- quand on transmet donnée en donénes blutooth il faudra baisser la tension (en carte Uno -> bluetooth) et quand bluetooth -> Uno pas de soucis
- bibliothèque HC06 pour le bluetooth
- pas trop de boutons poussoirs pour le LCD (4 ça passera pas)
- pour ihm utiliser pointeur de fonction
- plus il y a de flottants, mieux c'est
- regarder bouqin pour trouver exemple pour LCD, regarder le schéma préfait sur Tinkercad

## Florentin
Réaliser une IHM (afficheur LCD associé à des boutons poussoirs) pour afficher les
informations suivantes et contrôler le comportement du système :
- Afficher l'angle d'inclinaison des 2 axes en degré °
- Afficher la Vitesse de rotation de l'arbre moteur et le sens de rotation.
- Afficher le mode de pilotage du servo-moteur et du moteur.
- Choisir le pilotage du servo-moteur et du moteur soit par le(s)
potentiomètre(s) (mode Manuel), soit par l'accéléromètre (mode Auto)
<br></br>

### Composants
- Faire un bouton switchant entre les 3 menus au-dessus
- Faire un deuxième bouton qui change entre Manuel et Automatique

- Mode Manuel : utilisation avec potentiomètre
- Mode Automatique : utilisation avec accéléromètre
### Idées
- faire en sorte que on puisse voir sur l'affichage dans quel mode on est
- genre on prend une partie de l'écran pour afficher 'MAN' ou 'AUTO'
- on écrit sur la ligne 1 le mode et sur la ligne 2 l'affichage de valeurs
- faire le changeemnt entre les affichages avec un modulo 3 et une incrémention sur le setCursor(0,0)
- faire le changement de mode pareil en modulo 2 sur setCursor(0,1)




