#!/bin/bash

# Fichier de log
LOG="resultats_test.log"
> "$LOG"

# Liste des commandes à tester
COMMANDS=(
    "ls"
    "pwd"
    "echo Hello World"
    "foobar"
    "/bin/ls"
    "/bin/false"
    "/bin/true"
    "/root/secret.sh"
    "exit"
    "exit 5"
    "exit 42"
    "env"
)

echo "==== TEST DE ./hsh vs /bin/sh ====" | tee -a "$LOG"
for cmd in "${COMMANDS[@]}"; do
    echo -e "\n--- Commande: '$cmd' ---" | tee -a "$LOG"

    # Exécution avec ton shell
    OUT_HSH=$(echo "$cmd" | ./hsh 2>&1)
    RET_HSH=$?

    # Exécution avec /bin/sh
    OUT_SH=$(echo "$cmd" | /bin/sh 2>&1)
    RET_SH=$?

    # Comparaison
    if [[ "$OUT_HSH" == "$OUT_SH" && $RET_HSH -eq $RET_SH ]]; then
        echo "✅ OK" | tee -a "$LOG"
    else
        echo "❌ Différences !" | tee -a "$LOG"
        echo "  ./hsh => [$OUT_HSH] (ret=$RET_HSH)" | tee -a "$LOG"
        echo "  /bin/sh => [$OUT_SH] (ret=$RET_SH)" | tee -a "$LOG"
    fi
done

echo -e "\nRésultats complets dans $LOG"
