PROJET C & SYSTÈME
=================================
*Author: Equipe 33 (Amina ASSOUANE Mickeal Darmon Yuchen BAI )*



Installation
----------------------------
Pour installer le programme : mpsh il suffit d'avoir installé gcc sur son ordinateur. Puis de cloner le dépôt:
$  git clone git@moule.informatique.univ-paris-diderot.fr:mpsh-shell/mpsh.git


Build le programme
--------------------------------
Un Makefile est à disposition pour compiler le programme:

$ cd mpsh
$ make            # Build le programme


Configuration
--------------------------------
Pour rendre le programme accessible par tout, il faut :

(1) Lance : ./config_bashrc ou ./config_bash_profile 

(2) Tape : source ~/.bash_profile ou source ~/.bashrc

(3) la commande mpsh marche sur tout environement.


Commandes implémentées
------------------------------------
Toutes les commandes sont éxecutables et marchent parfaitement, + ajout de l'option n pour cat (pour afficher le nombre de lignes) en bonus.


Variables
------------------------------------
? : éxecutable à partir du shell.
INVITE : créé et initialisable dans mpshrc
CHEMIN : créé dans mpshrc mais non utilisé/non utilisable


Enchaînements de commandes et redirections
------------------------------------
Les enchaînements de commandes et redirections marchent parfaitement, mais une seule redirection est effectuable à la fois.


Conventions syntaxiques
------------------------------------
Marchent toutes parfaitement, excépté un problème du caractère "~" dans les enchaînements.


Complétions automatiques
------------------------------------
Marche parfaitement sur les fichiers et répertoires, mais ne fonctionne pas sur les commandes.
