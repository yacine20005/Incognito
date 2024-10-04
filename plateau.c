# include <stdio.h>
# define T 5

int calculer_pion(int t) {
    int N;
    N = (((t - 1) * (t - 2))/2) - 1;
    return N;
}

int generer_plateau(int t) {
    int plateau[t][t];
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            plateau[i][j] = 0;
        }
    }
    return plateau;
}

int placer_chateau(int plateau, int t) {
    plateau[t/2][t/2] = 1;
    return plateau;
}

int main() {
    int t = T;
    if (t >= 8) {
        printf("La taille du plateau est trop grande\n");
        return 1;
    }
    int N = calculer_pion(t);
    printf("Le nombre de pions sur le plateau est de %d\n", N);
    return 0;
}