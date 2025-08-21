#!/bin/bash

# Fonction pour exécuter une commande dans le shell personnalisé
run_test() {
    local cmd="$1"
    echo ">>> Test: \"$cmd\""
    echo "$cmd" | ./hsh
    echo "Code de retour = $?"
    echo "-----------------------------"
}
# Test de commandes non existantes
run_test "foobar"                # Commande inconnue (devrait retourner 127)
run_test "/bin/foobar"            # Commande inexistante (devrait retourner 127)
# Test d'une commande avec mauvaise permission
run_test "/bin/ls"               # Devrait réussir (ls existe)
run_test "/bin/false"            # Devrait échouer (retourne 1)
# Test de changement de répertoire (cd)
run_test "cd /nonexistent"       # Répertoire inexistant (devrait échouer)
run_test "cd /root"              # Requiert des privilèges root (devrait échouer)
# Test de la commande `exit`
run_test "exit"                  # Sortie sans code (devrait réussir)
run_test "exit 42"               # Sortie avec code 42 (devrait réussir)
# Test avec un argument invalide pour exit
run_test "exit abc"              # Argument non numérique (devrait retourner 2)
# Test de commandes avec un chemin relatif
run_test "./hsh --version"       # Commande invalide (devrait échouer avec 127)
# Test avec une commande valide mais non dans PATH
run_test "echo Hello"            # Commande valide (devrait réussir)
# Test de la gestion de `env`
run_test "env | head -n 3"       # Devrait afficher les 3 premières variables d'environnement
# Test avec des espaces autour des arguments
run_test "    ls    -l     "     # Devrait exécuter "ls -l" correctement
run_test "    exit   1   "       # Devra_