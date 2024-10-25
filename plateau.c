#include <stdio.h> //Tjr là tjr présent
#include "types.h" //Flm de mettre les types ici donc je les ai mis dans un autre fichier
#include <time.h>
#include <stdlib.h>
#include <string.h>

// taille du plateau TxT
#define T 5

// Les directions où il est possible de placer les chevaliers pour le château en haut à gauche
int directions_haut[3][2] = {{0, 1}, {1, 0}, {1, 1}};

// même chose pour le château en bas à droite
int directions_bas[3][2] = {{0, -1}, {-1, 0}, {-1, -1}};

int changement_tour(int t, Couleur tour[t])
{
    Couleur tmp = tour[0];
    tour[0] = tour[1];
    tour[1] = tmp;
}

char *couleur_to_string(Couleur c)
{
    switch (c)
    {
    case BLANC:
        return "Blanc";
    case NOIR:
        return "Noir";
    default:
        return "inconnue";
    }
}

void reset_tab(int ligne, int colonne, int tab[ligne][colonne])
{
    for (int i = 0; i < ligne; i++)
    {
        for (int j = 0; j < colonne; j++)
        {
            tab[i][j] = -1;
        }
    }
}

int deplacement_possible_bidirection(int x, int y, int t, Pion plateau[t][t], int actionpossible[T * 4][2])
{
    int index = 0;
    // Déplacement à droite
    for (int i = x + 1; i < t && plateau[y][i].type == -1; i++)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = y;
        index++;
    }
    // Déplacement à gauche
    for (int i = x - 1; i >= 0 && plateau[y][i].type == -1; i--)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = y;
        index++;
    }
    // Déplacement vers le bas
    for (int j = y + 1; j < t && plateau[j][x].type == -1; j++)
    {
        actionpossible[index][0] = x;
        actionpossible[index][1] = j;
        index++;
    }
    // Déplacement vers le haut
    for (int j = y - 1; j >= 0 && plateau[j][x].type == -1; j--)
    {
        actionpossible[index][0] = x;
        actionpossible[index][1] = j;
        index++;
    }
    return index;
}

void deplacement_possible_diagonale(int x, int y, int t, Pion plateau[t][t], int actionpossible[T * 4][2], int index)
{
    int i = x;
    int j = y;
    while (i < t && j < t && plateau[j][i].type == -1)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i += 1;
        j += 1;
    }
    i = x;
    while (i > 0 && j > 0 && plateau[j][i].type == -1)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i += 1;
        j -= 1;
    };
    while (j < t && i > 0 && plateau[j][i].type == -1)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i -= 1;
        j -= 1;
    }
    j = y;
    while (j > 0 && i < t && plateau[j][i].type == -1)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i -= 1;
        j += 1;
    };
};

void deplacement_possible(int x, int y, int t, Pion plateau[t][t], int actionpossible[T * 4][2])
{
    reset_tab(T * 4, 2, actionpossible);
    int index = deplacement_possible_bidirection(x, y, t, plateau, actionpossible);
    deplacement_possible_diagonale(x, y, t, plateau, actionpossible, index);
}

int deplacer_pion(int x_depart, int y_depart, int t, Pion plateau[t][t], int actionpossible[T * 4][2], int x_arrive, int y_arrive)
{
    int i = 0;
    while (actionpossible[i][0] != -1 && actionpossible[i][1] != -1)
    {
        if (actionpossible[i][0] == x_arrive && actionpossible[i][1] == y_arrive)
        {
            plateau[y_arrive][x_arrive].type = plateau[y_depart][x_depart].type;
            plateau[y_arrive][x_arrive].couleur = plateau[y_depart][x_depart].couleur;
            plateau[y_depart][x_depart].type = -1;
            plateau[y_depart][x_depart].couleur = -1;
            return 0;
        }
        i++;
    }
    return 1;
}

int victoire(int t, Pion plateau[t][t], int nbtour)
{
    /*
    Condition de victoire, parcoure tout le plateau et verifie s'il y a 2 espion ou pas,
    s'il y en a qu'un ou 0 la partie est fini donc on renvoie False, sinon True
    */

    if (nbtour > 2)
        return 0;
    int nbespion = 0;
    for (int y = 0; y < t; y++)
    {
        for (int x = 0; x < t; x++)
        {
            if (plateau[y][x].type == ESPION)
                nbespion += 1;
        }
    }
    return nbespion == 2 ? 1 : 0;
}

void generer_plateau(int t, Pion plateau[t][t])
{
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < t; j++)
        {
            plateau[i][j].type = -1;    // On met -1 en attendant de les spécifiés
            plateau[i][j].couleur = -1; // La meme là
        }
    }
}

int calculer_chevalier(int t)
{
    int N;
    N = (((t - 1) * (t - 2)) / 2) - 1; // Formule du pdf pour calculer le nombre de chevalier
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
    int nbrandomblanc = rand() % chevalier + 1;
    int nbrandomnoir = rand() % chevalier + 1;
    for (int k = 1; k < 8 && chevalier > 0; k++)
    {
        for (int i = 0; i < 8 && chevalier > 0; i++)
        {
            int nx = x + (directions[i][0] * k);                                      // En gros on essaie de placer les chevalier collé au chateau
            int ny = y + (directions[i][1] * k);                                      // et si tout est rempli on essaie d'aller un cran plus loin avec k
            if (nx >= 0 && nx < t && ny >= 0 && ny < t && plateau[nx][ny].type == -1) // On verifie si on est sur le plateau et si la case est pas déjà occupé par un frero
            {
                if (chevalier == nbrandomblanc && Couleur == BLANC)
                    plateau[nx][ny].type = ESPION;
                else if (chevalier == nbrandomnoir && Couleur == NOIR)
                    plateau[nx][ny].type = ESPION;
                else
                    plateau[nx][ny].type = CHEVALIER;
                plateau[nx][ny].couleur = Couleur;
                chevalier--;
            }
        }
    }
}

void placer_chevalier(int t, Pion plateau[t][t], int chevalier) // Pour placer les chevalier des deux camps
{
    placer_chevalier_autour_chateau(t, plateau, 0, 0, chevalier, directions_haut, BLANC);
    placer_chevalier_autour_chateau(t, plateau, t - 1, t - 1, chevalier, directions_bas, NOIR);
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
                printf(plateau[i][j].couleur == BLANC ? "e" : "E");
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

void afficher_couppossible(int couppossible[T * 4][2])
{
    for (int i = 0; i < T * 4; i++)
    {
        if (couppossible[i][0] != -1 || couppossible[i][1] != -1)
        {
            printf("(%d,%d) ", couppossible[i][0], couppossible[i][1]);
        }
    }
}

void vider_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Lire et ignorer tous les caractères jusqu'à la fin de la ligne
}

int main()
{
    int t = T;
    if (t >= 8) // On vérifie si T est pas plus grand que 8 car c'est interdit par la consigne
    {
        printf("La taille du plateau est trop grande\n");
        return 1;
    }
    printf("Taille valide \n");
    int nbtour = 0;
    Couleur tour[] = {BLANC, NOIR};
    int chevalier;
    Pion plateau[T][T];
    chevalier = calculer_chevalier(T);
    printf("Le nombre de chevalier sur le plateau est de %d\n", chevalier);
    generer_plateau(T, plateau);
    placer_chateau(T, plateau);
    placer_chevalier(T, plateau, chevalier);
    printf("Bienvenue dans Incognito \n");
    afficher_plateau(T, plateau);
    while (victoire(T, plateau, nbtour) == 1)
    {
        int pionselectionne[2] = {0, 0};
        int pion_arrive[2] = {0, 0};
        int actionpossible[T * 4][2];
        printf("\n Joueur %s, quelle pion voulez vous selectionner (Format : X Y ) : ", couleur_to_string(tour[0]));

        scanf("%d", &pionselectionne[0]);
        scanf("%d", &pionselectionne[1]);
        printf("La piece selectionnee est (%d,%d)\n", pionselectionne[0], pionselectionne[1]);

        while ((plateau[pionselectionne[1]][pionselectionne[0]].type != CHEVALIER && plateau[pionselectionne[1]][pionselectionne[0]].type != ESPION) ||
               (plateau[pionselectionne[1]][pionselectionne[0]].couleur != tour[0]))
        {
            afficher_plateau(T, plateau);
            printf("Entree invalide veuillez recommencez \n (Soit ce n'est pas un pion de votre couleur, soit l'entree est hors du plateau)");
            printf("\n Joueur %s, quelle pion voulez vous selectionner (Format : X Y ) : ", couleur_to_string(tour[0]));
            vider_buffer(); // temporairement c'est chagpt qui l'as fait prcq la boucle infinie elle clc
            // fflush(stdin) ; le prof avait dit de use ca mais ca marchais pas.
            scanf("%d", &pionselectionne[0]);
            scanf("%d", &pionselectionne[1]);
        }

        deplacement_possible(pionselectionne[0], pionselectionne[1], T, plateau, actionpossible);
        printf("\n Les coups possible sont : \n");
        afficher_couppossible(actionpossible);
        printf("\n");
        printf("Choisissez votre deplacement (Format : X Y ) : ");
        scanf("%d", &pion_arrive[0]);
        scanf("%d", &pion_arrive[1]);
        while (deplacer_pion(pionselectionne[0], pionselectionne[1], T, plateau, actionpossible, pion_arrive[0], pion_arrive[1]))
        {
            printf("Deplacement impossible\n");
            printf("Choisissez votre deplacement (Format : X Y ) : ");
            scanf("%d", &pion_arrive[0]);
            scanf("%d", &pion_arrive[1]);
        }
        afficher_plateau(T, plateau);
        nbtour += 1;
        changement_tour(2, tour);
    }
    return 0;
}
