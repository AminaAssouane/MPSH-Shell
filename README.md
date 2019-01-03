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




Le but de ce projet
------------------------------------

Le but de ce projet est de vous faire écrire un interpréteur de commandes (connu également sous le nom de shell). Dans le reste de l'énoncé, nous appellerons ce shell mpsh.

Le shell mpsh fonctionnera comme les shells les plus courants : une invite de commande attendra la demande de l'utilisateur, puis y répondra. Cette demande prendra la forme d'une combinaison de commandes simples et d'éventuelles redirections, une commande simple étant soit une affectation de variable, soit une commande (interne à mpsh ou externe), soit un alias, suivi d'éventuelles options et d'éventuels arguments. Les combinaisons possibles de commandes et les redirections sont détaillées dans la partie Enchaînements de commandes et redirections ci-dessous.

mpsh est un shell assez élémentaire, mais un utilisateur habituel d'un shell usuel comme bash doit pouvoir s'en servir sans difficulté ; on vous demande en particulier de respecter les points suivants :

le shell mpsh doit supporter les références de fichiers relatives et absolues ;
les commandes sur mpsh sont toutes lancées en avant-plan ;
les commandes externes qui fonctionnent sous les shells usuels doivent continuer à fonctionner sous mpsh (et on ne vous demande évidemment pas de les réécrire).
À son lancement, mpsh devra lire, s'il existe, le fichier de configuration ~/.mpshrc. La description des informations contenues dans ce fichier apparaît répartie à plusieurs endroits dans la suite du sujet.

Pour chaque fonctionnalité demandée ci-dessous, merci de coller au plus près avec les notations de bash, cela permettra des tests plus rapides de vos programmes.


Fonctionnalités
--------------------------------------------------

Nous décrivons ici les fonctionnalités de votre shell. Certaines seront implémentées comme commandes internes, d'autres comme commandes externes, à vous de décider. Pour vous faire une idée, vous pouvez consulter la liste des commandes internes de bash à la page du manuel de bash-builtins, et celle de zsh à la page du manuel de zshbuiltins. Vous avez le droit d'utiliser des commandes externes qui existent déjà sur les ordinateurs des salles de TP de l'UFR si cela fonctionne.

Les fonctionnalités qu'on doit pouvoir utiliser avec mpsh sont les suivantes (les arguments optionnels pour une commande sont spécifiés entre crochets [ ]; quand l'action d'une option n'est pas précisée, c'est qu'elle correspond à la même option que la commande courante) :

alias [name=value] : affiche les alias ou met en place un alias
cat ref : affiche le contenu de la référence
cd [dir] : change le répertoire courant
echo $var : affiche la valeur de la variable var
exit [n] : permet de sortir du shell avec la valeur de retour n si n est spécifié, la valeur de retour de la dernière commande lancée sinon
export var[=word] : exporte une variable ( i.e. la transforme en variable d'environnement)
history [n] :
sans argument, affiche la liste numérotée de l'historique des commandes,
avec un argument n entier positif, relance la commande dont le numéro dans la liste est n,
avec un argument -n entier négatif, fixe à n le nombre de commandes enregistrées dans l'historique.
ls [ref] : liste le contenu d'un répertoire (le répertoire courant en l'absence d'arguments)
mkdir [-p] ref : crée un nouveau répertoire
pwd : affiche la référence absolue du répertoire courant
type name [name ...] : indique comment chaque nom est interprété (comme alias, commande interne ou commande externe) s'il est utilisé pour lancer une commande
umask mode : met en place un masque pour les droits
unalias name : supprime un alias

Variables
----------------------------------------------

mpsh doit gérer les variables : on doit pouvoir en définir, afficher les variables existantes et spécifier qu'une variable n'existe plus ; de plus l'export de variables doit être supporté.

La gestion des variables suivantes vous est imposée :

? donne la valeur de retour de la dernière commande lancée.

INVITE contient le format du texte affiché par l'invite de commande de mpsh ; il y a une invite par défaut, mais l'utilisateur doit pouvoir définir le format de l'invite dans son fichier de configuration ~/.mpshrc (équivalent de la variable PS1 en bash).

CHEMIN est une variable d'environnement qui joue le rôle de la variable PATH en bash : CHEMIN est une suite de références de répertoires espacées par deux points (:) ; lorsque l'utilisateur lance une commande qui n'est ni un alias, ni une commande interne, l'exécutable doit être cherché dans les répertoires listés par CHEMIN (l'alias, puis la commande interne, s'il en existe du nom donné, sont toujours prioritaires dans ce choix). Quand le nom d'un répertoire listé dans CHEMIN est suivi de //, alors la recherche de l'exécutable est faite dans toute la sous-arborescence. C'est le premier exécutable trouvé par la recherche d'exécutable qui doit être exécuté (dans le cas de sous-arborescences, l'ordre de visite des fichiers n'est pas imposé).

Par exemple, si on définit CHEMIN=~/bin//:/bin, le lancement de la commande ls doit amener mpsh à vérifier dans l'ordre (en s'arrêtant dès que c'est possible) : s'il y a un alias ls, s'il y a une commande interne ls, s'il y a une commande ls dans l'arborescence de racine ~/bin, s'il y a une commande /bin/ls. Si aucune de ces recherches n'aboutit, mpsh doit afficher un message d'erreur.

Enchaînements de commandes et redirections
On veut pouvoir enchaîner des commandes avec les connecteurs logiques "et" (&&) et "ou" (||). L'évaluation doit se faire de façon paresseuse, c'est-à-dire que l'exécution de la deuxième commande est conditionnée au résultat de l'exécution de la première.
Les enchaînements d'un nombre quelconque de commandes doivent être possibles. On supposera pour simplifier que le parenthésage se fait implicitement de droite à gauche.

mpsh doit gérer les redirections d'entrée (<), de sortie (>) et de sortie erreur (2>), et permettre l'enchaînement de commandes par tubes (|).

Il n'est pas demandé de pouvoir combiner sur une même ligne de commande des connecteurs logiques et des redirections.

Conventions syntaxiques
---------------------------------------

Pour simplifier la lecture d'une ligne de commande, on suppose que les caractères utilisés dans une ligne de commande sont soit des espaces, soit de type alphanumérique, soit un des caractères suivants :

. _ $ / ~ - = ? : > < | &

L'espace servira uniquement et systématiquement à séparer deux éléments différents de la ligne de commande (attention : une affectation de variable est considérée comme un unique élément, donc pas d'espaces autour du signe =). On ne vous demande pas de gérer un caractère d'échappement (en conséquence on suppose qu'aucun fichier ne contient d'espace dans son nom par exemple). Le caractère $ correspond toujours à l'évaluation de la variable dont le nom suit ce caractère.

