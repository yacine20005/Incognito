#include <stdio.h>
#include "types.h"

void generer_plateau(int t, int plateau[t][t])
{
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < t; j++)
        {
            plateau[i][j] = '.';
        }
    }
}

int calculer_pion(int t)
{
    int N;
    N = (((t - 1) * (t - 2)) / 2) - 1;
    return N;
}

void placer_chateau(int t, int plateau[t][t])
{
    plateau[0][0] = 'C';
    plateau[t - 1][t - 1] = 'C';
}

void placer_pion(int t, int plateau[t][t], int pion)
{
    int k = 0;
    int x = 1;
    int y = 1;
    while (pion > 0)
    {
        if (k % 2)
        {
            plateau[0][0 + x] = 'P';
            x++;
        }
        else
        {
            plateau[0 + y][0] = 'P';
            y++;
        }
        k++;
    }
}

void afficher_plateau(int t, int plateau[t][t])
{
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < t; j++)
        {
            printf("%c", plateau[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int t = T;
    if (t >= 8)
    {
        printf("La taille du plateau est trop grande\n");
        return 1;
    }
    int pion, plateau[T][T];
    pion = calculer_pion(T);
    printf("Le nombre de pions sur le plateau est de %d\n", pion);
    generer_plateau(T, plateau);
    afficher_plateau(T, plateau);
    placer_chateau(T, plateau);
    afficher_plateau(T, plateau);
    placer_pion(T, plateau, pion);
    afficher_plateau(T, plateau);
    return 0;
}