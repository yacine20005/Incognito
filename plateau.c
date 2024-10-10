#include <stdio.h> //Tjr là tjr présent
#include "types.h" //Flm de mettre les types ici donc je les ai mis dans un autre fichier
#include <time.h> 
#include <stdlib.h.>

#define T 5 //taille du plateau TxT

int directions_haut[3][2] = {{0, 1}, {1, 0}, {1, 1}}; // Les directions où il est possible de placer les chevaliers pour le château en haut à gauche

int directions_bas[3][2] = {{0, -1}, {-1, 0}, {-1, -1}}; // même chose pour le château en bas à droite

void generer_plateau(int t, Pion plateau[t][t]) 
{
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < t; j++)
        {
            plateau[i][j].type  = -1; //On met -1 en attendant de les spécifiés
            plateau[i][j].couleur = -1; //La meme là
        }
    }
}

int calculer_chevalier(int t)
{
    int N;
    N = (((t - 1) * (t - 2)) / 2) - 1; //Formule du pdf pour calculer le nombre de chevalier
    return N;
}

void placer_chateau(int t, Pion plateau[t][t])
{
    plateau[0][0].type = CHATEAU;
    plateau[0][0].couleur = BLANC;
    plateau[t - 1][t - 1].type = CHATEAU;
    plateau[t - 1][t - 1].couleur = NOIR;
}

void placer_chevalier_autour_chateau(int t, Pion plateau[t][t], int x, int y, int chevalier, int directions[3][2], Couleur Couleur)
{
    srand(time(NULL));
    int nbrandomblanc = rand() %chevalier;
    int nbrandomnoir = rand() %chevalier;
    for (int k = 1; k < 8 && chevalier > 0; k++)
    {
        for (int i = 0; i < 8 && chevalier > 0; i++)
        {
            int nx = x + (directions[i][0] * k); // En gros on essaie de placer les chevalier collé au chateau
            int ny = y + (directions[i][1] * k); // et si tout est rempli on essaie d'aller un cran plus loin avec k
            if (nx >= 0 && nx < t && ny >= 0 && ny < t && plateau[nx][ny].type == -1) //On verifie si on est sur le plateau et si la case est pas déjà occupé par un frero
            {
                if (chevalier == nbrandomblanc && Couleur == BLANC)
                    plateau[nx][ny].type = ESPION;
                else if (chevalier == nbrandomnoir && Couleur == NOIR)
                    plateau[nx][ny].type = ESPION;
                else
                    plateau[nx][ny].type = CHEVALIER;
                plateau[nx][ny].couleur = Couleur;
                printf("Chevalier placé en (%d), (%d) et il reste %d chevalier en boucle numéro :%d \n", nx, ny, chevalier, i);
                chevalier--;
            }
        }
    }
}

void placer_chevalier(int t, Pion plateau[t][t], int chevalier) //Pour placer les chevalier des deux camps
{
    placer_chevalier_autour_chateau(t, plateau, 0, 0, chevalier, directions_haut, BLANC);
    placer_chevalier_autour_chateau(t, plateau, t - 1, t - 1, chevalier, directions_bas, NOIR);
}

void choisir_espion(int t, Pion plateau[t][t])
{
    //Il faut choisir aléatoirement un chevalier parmis les deux camps pour que ce soit des espions
}

void afficher_plateau(int t, Pion plateau[t][t])
{
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < t; j++)
        {
            if (plateau[i][j].type == -1)
            {
                printf(".");
            }
            else if (plateau[i][j].type == CHEVALIER)
            {
                printf(plateau[i][j].couleur == BLANC ? "b" : "n"); // Si bro est un chevalier, affiche 'b' si il est blanc et 'n' si il est noir
            }
            else if (plateau[i][j].type == ESPION)
            {
                printf(plateau[i][j].couleur == BLANC ? "e" : "M");
            }
            else if (plateau[i][j].type == CHATEAU)
            {
                 printf(plateau[i][j].couleur == BLANC ? "B" : "N");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int t = T;
    if (t >= 8) //On vérifie si T est pas plus grand que 8 car c'est interdit par la consigne
    {
        printf("La taille du plateau est trop grande\n");
        return 1;
    }
    int chevalier;
    Pion plateau[T][T];
    chevalier = calculer_chevalier(T);
    printf("Le nombre de chevalier sur le plateau est de %d\n", chevalier);
    generer_plateau(T, plateau);
    afficher_plateau(T, plateau);
    placer_chateau(T, plateau);
    afficher_plateau(T, plateau);
    placer_chevalier(T, plateau, chevalier);
    afficher_plateau(T, plateau);
    printf("malagangx"); //mdr
    return 0;
}