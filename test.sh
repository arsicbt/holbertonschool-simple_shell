#!/bin/bash
# 1. Choisir le commit à restaurer (exemple : HEAD~5 ou un hash précis)
COMMIT=070099e

# 2. Choisir le dossier de destination
DEST=/tmp/restore_$COMMIT

# 3. Créer le dossier de destination
mkdir -p $DEST

# 4. Extraire tous les fichiers du projet au commit choisi
git archive --format=tar $COMMIT | tar -x -C $DEST

# 5. Lister les fichiers extraits pour vérifier
echo "✅ Projet restauré dans $DEST"
ls -R $DEST