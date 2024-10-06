#include <stdio.h>
#include "types.h"

#define T 5

int directions_haut[3][2] = {{0, 1}, {1, 0}, {1, 1}}; // Les directions où il est possible de placer les pions pour le château en haut à gauche

int directions_bas[3][2] = {{0, -1}, {-1, 0}, {-1, -1}}; // même chose pour le château en bas à droite

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

void placer_pions_autour_chateau(int t, int plateau[t][t], int x, int y, int pion, int directions[3][2])
{
    for (int k = 1; k < 8 && pion > 0; k++)
    {
        for (int i = 0; i < 8 && pion > 0; i++)
        {
            int nx = x + (directions[i][0] * k);
            int ny = y + (directions[i][1] * k);
            if (nx >= 0 && nx < t && ny >= 0 && ny < t && plateau[nx][ny] == '.')
            {
                plateau[nx][ny] = 'P';
                printf("Pion placé en (%d), (%d) et il reste %d pion en boucle numéro :%d \n", nx, ny, pion, i);
                pion--;
            }
        }
    }
}

void placer_pion(int t, int plateau[t][t], int pion)
{
    placer_pions_autour_chateau(t, plateau, 0, 0, pion, directions_haut);
    placer_pions_autour_chateau(t, plateau, t - 1, t - 1, pion, directions_bas);
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