    int chargerPartie = 0; // variable pour indiquer si on doit charger une partie
    int sauvegarderPartie = 0; // variable pour indiquer si on doit sauvegarder chaque coup
    FILE* fichierSauvegarde = NULL;
    FILE* fichierChargement = NULL;
    // Vérification des arguments de la ligne de commande
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            fichierChargement = fopen(argv[i + 1], "r");  // Ouvre le fichier pour charger la partie
            if (fichierChargement == NULL) {
                printf("Une erreur s'est produite pendant l'ouverture du fichier de chargemnt \n");
                return 1;
            }
            chargerPartie = 1; // Active le chargement de la partie
            i++;  // Saute l'argument suivant qui est le chemin
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            fichierSauvegarde = fopen(argv[i + 1], "w");  // Ouvre le fichier de sauvegarde
            if (fichierSauvegarde == NULL) {
                printf("Une erreur s'est produite pendant l'ouverture du fichier de sauvegarde \n");
                return 1;
            }
            sauvegarderPartie = 1; // Active la sauvegarde
            i++;  // Saute l'argument suivant qui est le chemin
        }
        }
    }