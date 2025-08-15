# Simple Shell – Projet en C 🐚

## 👩‍💻 Présentation
Nous sommes deux étudiants en d’informatique, **Mathis et Arsinoé**, et voici notre **premier projet d’école à HOLBERTON School**.    
L’objectif de ce projet était de créer un mini interpréteur de commandes pour Unix, afin de mieux comprendre le fonctionnement interne d’un terminal et la gestion des processus en C.


---

## 🪄 Fonctionnalités principales
Notre shell permet de :  
- Exécuter des commandes avec arguments, par exemple :  
  ```bash
  ls -l
  pwd
  echo Hello
  ```
Fonctionnement :
- Rechercher automatiquement les commandes dans le PATH si le chemin complet n’est pas fourni
- Gérer les erreurs lorsque la commande n’existe pas, avec un message clair
- Utiliser fork() et execve() pour lancer les commandes dans des processus enfants


## 💡 Vous souhaitez l'essayer ?
1. Clonez le projet :
    ```bash
    git clone <URL_DU_REPO>
    ```
2. Complilez-le :
    ```bash
    gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
    ```
3. Executez !
    ```bash
    ./hsh
    ```

## 📄 Utilisation
- Tapez n’importe quelle commande Unix valide et appuyez sur Entrée.
- Si la commande n’existe pas, un message d’erreur s’affiche

>**Exemple :**

    ```bash
    $ ./hsh
    hsh> ls
    hsh> pwd
    hsh> hello
    ./hsh: 1: hello: not found
    ```

## ⚙️ Explications techniques
**Gestion du PATH :** nous recherchons les commandes via la variable d’environnement PATH

**Exécution des commandes :** chaque commande est lancée dans un processus enfant grâce à fork()

**Gestion des erreurs :** les commandes inexistantes renvoient le code d’erreur 127 et affichent un message clair

**Mémoire et sécurité :** nous veillons à libérer correctement les allocations dynamiques


## Conclusion ⭐

### Ce que nous avons appris :
- La manipulation avancée des chaînes de caractères et des tableaux en C
- Le fonctionnement des processus Unix et des appels système (fork, execve, wait)
- La gestion des erreurs et de la mémoire pour des programmes stables et sûrs
- La collaboration en binôme sur un projet réel, avec partage des idées et du code

---

Ce projet nous a permis de plonger dans le fonctionnement interne des shells et de renforcer nos compétences en programmation système.
C’est la plus gros projet que nous avons réalisé en binôme (pour l'instant !), et il reflète notre motivation, notre curiosité et notre passion pour l’informatique.

Nous espérons que ce mini shell vous donnera un aperçu clair de notre travail et de notre approche méthodique.


**Mathis et Arsinoé**