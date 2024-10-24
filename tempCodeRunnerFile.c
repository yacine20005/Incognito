void afficher_couppossible(int couppossible[T*4][2]) {
    for (int i = 0; i < T*4; i++) {
        printf("\n");
        for (int j = 0; j < 2; j++) {
            printf(" %c ", couppossible[i][j]);
        }
    }
}