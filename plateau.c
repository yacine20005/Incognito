/*
    Project Incognito
    Auteurs : Yacine HAMADOUCHE, Liam GAVAU--PELISSIER
    Description : Jeu Incognito est un jeu de plateau où deux joueurs s'affrontent.
    Chaque joueur a un château et un espion caché dans le plateau.
    Le but du jeu est de découvrir l'espion adverse et de le capturer ou de rentrer son propre espion dans son château.
*/
#include <stdio.h>
#include "types.h"
// Permet d'éviter de réecrire les structures et les constantes du fichier types.h
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Taille du plateau TxT
#define T 5

// Mode debug qui permet l'affichage des espions des deux joueurs pour des raisons de test
int DEBUG = 1;

void afficher_aide()
/*
    Affiche l'aide du programme pour notre cher utilisateur M. Bouillot
*/
{
    printf("Usage: incognito [-c <fichier>] [-s <fichier>]\n");
    printf("Jeu Incognito\n");
    printf("Options:\n");
    printf("  -c <fichier> Charger une partie\n");
    printf("  -s <fichier> Sauvegarder la partie\n");
    printf("  -d Mode debug\n");
}

// Les directions où il est possible de placer les chevaliers pour le château en haut à gauche
int directions_haut[3][2] = {{0, 1}, {1, 0}, {1, 1}};

// Même chose pour le château en bas à droite
int directions_bas[3][2] = {{0, -1}, {-1, 0}, {-1, -1}};

void sauvegarder_partie(const char *nom_fichier, int t, Pion plateau[t][t], Couleur tour[2], int nb_coups, Coup coups[nb_coups], Couleur gagnant, int position_espion_blanc[2], int position_espion_noir[2])
/*
    Sauvegarde la partie dans un fichier
    args:
        nom_fichier : le nom du fichier où sauvegarder la partie
        t : la taille du plateau
        plateau : le plateau de jeu
        tour : liste des tours dans l'ordre
        nb_coups : le nombre de coups joués
        coups : les coups joués
        gagnant : le gagnant de la partie
        position_espion_blanc : la position de l'espion blanc
        position_espion_noir : la position de l'espion noir

    return:
        void

    effet de bord:
        sauvegarde la partie dans un fichier
*/
{
    FILE *fichier = fopen(nom_fichier, "w"); // On ouvre le fichier en écriture
    if (fichier == NULL)                     // Si le fichier n'a pas pu être ouvert
    {
        printf("Erreur\n");
        return;
    }

    fprintf(fichier, "B %d %d\n", position_espion_blanc[0], position_espion_blanc[1]); // On écrit la position de l'espion blanc
    fprintf(fichier, "N %d %d\n", position_espion_noir[0], position_espion_noir[1]);   // On écrit la position de l'espion noir

    fprintf(fichier, "%c\n", tour[0] == BLANC ? 'B' : 'N'); // On écrit le tour actuel

    for (int i = 0; i < nb_coups; i++) // On écrit les coups joués dans la liste des coup
    {
        Coup coup = coups[i];
        fprintf(fichier, "%c %d %d -> %d %d\n", coup.type, coup.x_depart, coup.y_depart, coup.x_arrive, coup.y_arrive);
    }

    // On écrit le gagnant
    if (gagnant == -1)
    {
        fprintf(fichier, "-1\n");
    }
    else if (gagnant == BLANC)
    {
        fprintf(fichier, "B\n");
    }
    else if (gagnant == NOIR)
    {
        fprintf(fichier, "N\n");
    }
    // On ferme le fichier quand on a fini comme on est poli
    fclose(fichier);
}

int changement_tour(int t, Couleur tour[t])
/*
    Change le tour des joueurs
    args:
        t : le nombre de joueurs
        tour : la liste des tours

    return:
        void

    effet de bord:
        change le tour des joueurs
*/
{
    Couleur tmp = tour[0];
    tour[0] = tour[1];
    tour[1] = tmp;
}

char *couleur_to_string(Couleur c)
/*
    Convertit une couleur en chaine de caractères
    args:
        c : la couleur

    return:
        la couleur en chaine de caractères
*/
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
/*
    Remet à -1 toutes les valeurs d'un tableau
    args:
        ligne : le nombre de lignes du tableau
        colonne : le nombre de colonnes du tableau
        tab : le tableau à remettre à -1

    return:
        void

    effet de bord:
        remet à -1 toutes les valeurs du tableau
*/
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
/*
    Renvoie les déplacements possibles d'un pion dans les 4 directions orthogonaux
    args:
        x : la position x du pion
        y : la position x du pion
        t : la taille du plateau
        plateau : le plateau de jeu
        actionpossible : la liste des déplacements possibles

    return:

*/
{
    Couleur couleur = plateau[y][x].couleur;
    int index = 0;
    // Déplacement à droite
    for (int i = x + 1; i < t && (plateau[y][i].type == -1 || (plateau[y][i].type == CHATEAU && plateau[y][i].couleur != couleur)); i++)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = y;
        index++;
    }
    // Déplacement à gauche
    for (int i = x - 1; i >= 0 && (plateau[y][i].type == -1 || (plateau[y][i].type == CHATEAU && plateau[y][i].couleur != couleur)); i--)
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = y;
        index++;
    }
    // Déplacement vers le bas
    for (int j = y + 1; j < t && (plateau[j][x].type == -1 || (plateau[j][x].type == CHATEAU && plateau[j][x].couleur != couleur)); j++)
    {
        actionpossible[index][0] = x;
        actionpossible[index][1] = j;
        index++;
    }
    // Déplacement vers le haut
    for (int j = y - 1; j >= 0 && (plateau[j][x].type == -1 || (plateau[j][x].type == CHATEAU && plateau[j][x].couleur != couleur)); j--)
    {
        actionpossible[index][0] = x;
        actionpossible[index][1] = j;
        index++;
    }
    return index;
}

void deplacement_possible_diagonale(int x, int y, int t, Pion plateau[t][t], int actionpossible[T * 4][2], int index)
/*
    Renvoie les déplacements possibles d'un pion dans les 4 directions diagonaux
    args:
        x : la position x du pion
        y : la position y du pion
        t : la taille du plateau
        plateau : le plateau de jeu
        actionpossible : la liste des déplacements possibles
        index : l'index de la liste des déplacements possibles

    return:
        void

    effet de bord:
        remplit la liste des déplacements possibles
*/
{
    Couleur couleur = plateau[y][x].couleur;
    int i = x + 1;
    int j = y + 1;
    while (i < t && j < t && (plateau[j][i].type == -1 || (plateau[j][i].type == CHATEAU && plateau[j][i].couleur != couleur)))
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i += 1;
        j += 1;
    }
    i = x + 1;
    j = y - 1;
    while (i < t && j >= 0 && (plateau[j][i].type == -1 || (plateau[j][i].type == CHATEAU && plateau[j][i].couleur != couleur)))
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i += 1;
        j -= 1;
    };
    i = x - 1;
    j = y - 1;
    while (j >= 0 && i >= 0 && (plateau[j][i].type == -1 || (plateau[j][i].type == CHATEAU && plateau[j][i].couleur != couleur)))
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i -= 1;
        j -= 1;
    }
    i = x - 1;
    j = y + 1;
    while (i >= 0 && j < t && (plateau[j][i].type == -1 || (plateau[j][i].type == CHATEAU && plateau[j][i].couleur != couleur)))
    {
        actionpossible[index][0] = i;
        actionpossible[index][1] = j;
        index += 1;
        i -= 1;
        j += 1;
    };
};

int deplacer_pion(int x_depart, int y_depart, int t, Pion plateau[t][t], int actionpossible[T * 4][2], int x_arrive, int y_arrive)
/*
    Déplace un pion sur le plateau
    args:
        x_depart : la position x du pion à déplacer
        y_depart : la position y du pion à déplacer
        t : la taille du plateau
        plateau : le plateau de jeu
        actionpossible : la liste des déplacements possibles
        x_arrive : la position x où le pion doit arriver
        y_arrive : la position y où le pion doit arriver

    return:
        0 si le déplacement a été effectué
        1 si le déplacement n'a pas été effectué
*/
{
    int i = 0;
    while (actionpossible[i][0] != -1 && actionpossible[i][1] != -1) // On parcourt la liste des déplacements possibles
    {
        if (actionpossible[i][0] == x_arrive && actionpossible[i][1] == y_arrive) // Si le déplacement est possible
        {
            if (plateau[y_arrive][x_arrive].type == CHATEAU && plateau[y_depart][x_depart].type == CHEVALIER)
            {
                plateau[y_depart][x_depart].type = -1;
                plateau[y_depart][x_depart].couleur = -1;
                return 0;
            }
            plateau[y_arrive][x_arrive].type = plateau[y_depart][x_depart].type;       // On déplace le pion
            plateau[y_arrive][x_arrive].couleur = plateau[y_depart][x_depart].couleur; // On déplace le pion
            plateau[y_depart][x_depart].type = -1;                                     // On vide la case de départ
            plateau[y_depart][x_depart].couleur = -1;                                  // On vide la case de départ
            return 0;                                                                  // Si le déplacement a été effectué
        }
        i++;
    }
    return 1; // Si le déplacement n'a pas été effectué
};

void deplacement_possible(int x, int y, int t, Pion plateau[t][t], int actionpossible[T * 4][2])
/*
    Renvoie les déplacements possibles d'un pion dans les 8 directions donc les 4 orthogonaux et les 4 diagonaux il est vraiment fort cette fonction je l'aime trop de toute facon les profs lisent pas les docstring donc je peux dire ce que je veux twix droit > twix gauche
    args:
        x : la position x du pion
        y : la position y du pion
        t : la taille du plateau
        plateau : le plateau de jeu
        actionpossible : la liste des déplacements possibles

    return:
        void

    effet de bord:
        remplit la liste des déplacements possibles
*/
{
    reset_tab(T * 4, 2, actionpossible); // On remet à -1 la liste des déplacements possibles
    int index = deplacement_possible_bidirection(x, y, t, plateau, actionpossible);
    deplacement_possible_diagonale(x, y, t, plateau, actionpossible, index);
}

void interrogation_possible(int x, int y, int t, Pion plateau[t][t], int interrogation_possible[4][2], Couleur couleur)
/*
    Renvoie les pions à interroger autour d'un pion
    args:
        x : la position x du pion
        y : la position y du pion
        t : la taille du plateau
        plateau : le plateau de jeu
        interrogation_possible : la liste des pions à interroger
        couleur : la couleur du joueur

    return:
        void

    effet de bord:
        remplit la liste des pions qui sont interrogeables
*/
{
    int pos_a_verif[4][2] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}}; // Les positions à vérifier autour du pion
    reset_tab(4, 2, interrogation_possible);                    // On remet à -1 la liste des pions à interroger
    for (int i = 0; i < 4; i++)                                 // On parcourt les positions à vérifier
    {
        int test_x = x + pos_a_verif[i][0];                         // On calcule la position x à vérifier
        int test_y = y + pos_a_verif[i][1];                         // On calcule la position y à vérifier
        if (test_x >= 0 && test_x < t && test_y >= 0 && test_y < t) // Si la position est sur le plateau
        {
            if ((plateau[test_y][test_x].type == CHEVALIER || plateau[test_y][test_x].type == ESPION) && plateau[test_y][test_x].couleur != couleur) // Si le pion est un chevalier ou un espion et qu'il n'est pas de la même couleur
            {
                interrogation_possible[i][0] = test_x; // On ajoute la position x à la liste des pions à interroger
                interrogation_possible[i][1] = test_y; // On ajoute la position y à la liste des pions à interroger
            }
        }
    }
}

int interroger_pion(int x, int y, int t, Pion plateau[t][t], int interrogation_possible[4][2], int x_ennemi, int y_ennemi, Couleur couleur)
/*
    Interroge un pion
    args:
        x : la position x du pion qui interroge sherlock holmes un peu le pion
        y : la position y du pion qui interroge
        t : la taille du plateau
        plateau : le plateau de jeu
        interrogationpossible : la liste des pions à interroger
        x_ennemi : la position x du pion qui se fait interroger
        y_ennemi : la position y du pion qui se fait interroger
        couleur : la couleur du joueur

    return:
        0 si l'espion est capturé
        1 si le chevalier est capturé
        2 si l'interrogation n'est pas possible
*/
{
    for (int i = 0; i < 4; i++)
    {
        if (interrogation_possible[i][0] == x_ennemi && interrogation_possible[i][1] == y_ennemi) // Si le pion à interroger est dans la liste des pions interrogeables
        {
            if (plateau[y_ennemi][x_ennemi].type == ESPION) // Si le pion sui se fait interroger est un espion
            {
                plateau[y_ennemi][x_ennemi].type = -1;    // On le capture
                plateau[y_ennemi][x_ennemi].couleur = -1; // On le capture
                return 0;
            }
            plateau[y][x].type = -1;    // Le chevalier qui a interroger se fait capturer
            plateau[y][x].couleur = -1; // Dommage pour lui
            return 1;
        }
    }
    return 2;
}

int victoire(int t, Pion plateau[t][t])
/*
    Vérifie si un joueur a gagné
    args:
        t : la taille du plateau
        plateau : le plateau de jeu

    return:
        1 si un joueur a gagné
        0 sinon
*/
{
    int listeespion[2][2];  // On crée une liste des espions
    int listechateau[2][2]; // On crée une liste des chateaux
    int nbespion = 0;       // On initialise le nombre d'espion à 0
    int nbchateau = 0;      // On initialise le nombre de chateau à 0
    for (int y = 0; y < t; y++)
    {
        for (int x = 0; x < t; x++)
        {
            if (plateau[y][x].type == ESPION)
            {
                listeespion[nbespion][0] = x;
                listeespion[nbespion][1] = y;
                nbespion += 1;
            }
            if (plateau[y][x].type == CHATEAU)
            {
                listechateau[nbchateau][0] = x;
                listechateau[nbchateau][1] = y;
                nbchateau += 1;
            }
        }
    }
    if (nbchateau < 2) // Si un chateau est capturé
        printf("\n \n Victoire des %s, votre espion est rentrée dans le chateau !\n \n", couleur_to_string(plateau[listechateau[0][1]][listechateau[0][0]].couleur));
    else if (nbespion < 2) // Si un espion est capturé
        printf("\n \n Victoire des %s, vous avez demasquée l'espion adverse !\n \n", couleur_to_string(plateau[listeespion[0][1]][listeespion[0][0]].couleur));
    return nbespion == 2 ? 1 : 0;
}

void generer_plateau(int t, Pion plateau[t][t])
/*
    Génère un plateau de jeu
    args:
        t : la taille du plateau
        plateau : le plateau de jeu

    return:
        void

    effet de bord:
        génère un plateau de jeu
*/
{
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < t; j++)
        {
            plateau[i][j].type = -1;    // On met -1 en type pour dire que la case est vide
            plateau[i][j].couleur = -1; // Même chose ici
        }
    }
}

int calculer_chevalier(int t)
/*
    Calcule le nombre de chevalier à placer sur le plateau
    args:
        t : la taille du plateau

    return:
        le nombre de chevalier à placer sur le plateau
*/
{
    int N;
    N = (((t - 1) * (t - 2)) / 2) - 1; // Formule donné par le pdf pour calculer le nombre de chevalier
    return N;
}

void placer_chateau(int t, Pion plateau[t][t])
/*
    Place les chateaux sur le plateau
    args:
        t : la taille du plateau
        plateau : le plateau de jeu

    return:
        void

    effet de bord:
        place les chateaux sur le plateau
*/
{
    plateau[0][0].type = CHATEAU; // On place le chateau en haut à gauche
    plateau[0][0].couleur = BLANC;
    plateau[t - 1][t - 1].type = CHATEAU;
    plateau[t - 1][t - 1].couleur = NOIR; // On place le chateau en bas à droite
}

void placer_chevalier_autour_chateau(int t, Pion plateau[t][t], int x, int y, int chevalier, int directions[3][2], Couleur couleur, int position_espion[2], int chargement)
/*
    Place les chevaliers autour du chateau
    args:
        t : la taille du plateau
        plateau : le plateau de jeu
        x : la position x du chateau
        y : la position y du chateau
        chevalier : le nombre de chevalier à placer
        directions : les directions où il est possible de placer les chevaliers
        couleur : la couleur des chevaliers
        position_espion : la position de l'espion
        chargement : si on charge une partie ce qui permet de ne pas placer les espions

    return:
        void

    effet de bord:
        place les chevaliers autour du chateau
*/
{
    srand(time(NULL));                          // On initialise la seed pour le random afin d'avoir des valeurs différentes à chaque fois
    int nbrandomblanc = rand() % chevalier + 1; // On génère un nombre aléatoire pour placer l'espion
    int nbrandomnoir = rand() % chevalier + 1;  // On génère un nombre aléatoire pour placer l'espion
    for (int k = 1; k < 8 && chevalier > 0; k++)
    {
        for (int i = 0; i < 8 && chevalier > 0; i++)
        {
            int nx = x + (directions[i][0] * k);                                      // On essaie de coller les chevaliers autour du chateau
            int ny = y + (directions[i][1] * k);                                      // et si tout est rempli on essaie d'aller un cran plus loin avec k
            if (nx >= 0 && nx < t && ny >= 0 && ny < t && plateau[nx][ny].type == -1) // On verifie si on est sur le plateau et si la case est pas déjà occupé par un chevalier
            {
                if (!chargement) // Si on charge une partie on ne place pas les espions
                {
                    if (chevalier == nbrandomblanc && couleur == BLANC) // On place l'espion si c'est le bon moment
                    {
                        plateau[nx][ny].type = ESPION;
                        position_espion[0] = nx;
                        position_espion[1] = ny;
                    }
                    else if (chevalier == nbrandomnoir && couleur == NOIR) // On place l'espion si c'est le bon moment
                    {
                        plateau[nx][ny].type = ESPION;
                        position_espion[0] = nx;
                        position_espion[1] = ny;
                    }
                    else
                    {
                        plateau[nx][ny].type = CHEVALIER; // On place un chevalier
                    }
                }
                else
                {
                    plateau[nx][ny].type = CHEVALIER; // On place un chevalier
                }
                plateau[nx][ny].couleur = couleur;
                chevalier--; // On décrémente le nombre de chevalier à placer
            }
        }
    }
}

void placer_chevalier(int t, Pion plateau[t][t], int chevalier, int position_espion_blanc[2], int position_espion_noir[2], int chargement)
/*
    Place les chevaliers sur le plateau
    args:
        t : la taille du plateau
        plateau : le plateau de jeu
        chevalier : le nombre de chevalier à placer
        position_espion_blanc : la position de l'espion blanc
        position_espion_noir : la position de l'espion noir
        chargement : si on charge une partie ce qui permet de ne pas placer les espions

    return:
        void

    effet de bord:
        place les chevaliers sur le plateau
*/
{
    placer_chevalier_autour_chateau(t, plateau, 0, 0, chevalier, directions_haut, BLANC, position_espion_blanc, chargement);      // On place les chevaliers autour du chateau en haut à gauche
    placer_chevalier_autour_chateau(t, plateau, t - 1, t - 1, chevalier, directions_bas, NOIR, position_espion_noir, chargement); // On place les chevaliers autour du chateau en bas à droite
}

void placer_espions(int t, Pion plateau[t][t], int position_espion_blanc[2], int position_espion_noir[2])
/*
    Place les espions sur le plateau
    args:
        t : la taille du plateau
        plateau : le plateau de jeu
        position_espion_blanc : la position de l'espion blanc
        position_espion_noir : la position de l'espion noir

    return:
        void

    effet de bord:
        place les espions sur le plateau
*/
{
    plateau[position_espion_blanc[1]][position_espion_blanc[0]].type = ESPION; // On place l'espion blanc
    plateau[position_espion_blanc[1]][position_espion_blanc[0]].couleur = BLANC;

    plateau[position_espion_noir[1]][position_espion_noir[0]].type = ESPION; // On place l'espion noir
    plateau[position_espion_noir[1]][position_espion_noir[0]].couleur = NOIR;
}

void afficher_plateau(int t, Pion plateau[t][t], Couleur tour, int debug)
/*
    Affiche le plateau de jeu
    args:
        t : la taille du plateau
        plateau : le plateau de jeu
        tour : le tour du joueur
        debug : si le mode debug est activé ou non qui permet d'afficher les espions des deux joueurs

    return:
        void

    effet de bord:
        affiche le plateau de jeu
*/
{
    for (int i = 0; i < t; i++)
    {
        for (int a = 0; a < 2.2 * t; a++) // On affiche les lignes horizontales
        {
            printf("-");
        }
        printf("\n");
        for (int j = 0; j < t; j++) // On affiche les lignes verticales
        {
            printf("|");
            if (plateau[i][j].type == -1) // Si la case est vide, affiche un espace
            {
                printf(" ");
            }
            else if (plateau[i][j].type == CHEVALIER)
            {
                printf(plateau[i][j].couleur == BLANC ? "b" : "n"); // Si la case est un chevalier, affiche 'b' si il est blanc et 'n' si il est noir
            }
            else if (plateau[i][j].type == ESPION) // Si la case est un espion
            {
                if (debug == 0)
                {
                    // Afficher les espions des deux joueurs si debg est vrai
                    printf(plateau[i][j].couleur == BLANC ? "e" : "E");
                }
                else
                {
                    // Afficher l'espion du joueur à qui c'est le tour si debug est faux
                    if (plateau[i][j].couleur == tour)
                    {
                        printf(plateau[i][j].couleur == BLANC ? "e" : "E");
                    }
                    else
                    {
                        printf(plateau[i][j].couleur == BLANC ? "b" : "n");
                    }
                }
            }
            else if (plateau[i][j].type == CHATEAU)
            {
                printf(plateau[i][j].couleur == BLANC ? "B" : "N"); // Si la case est un chateau, affiche 'B' si il est blanc et 'N' si il est noir
            }
        }
        printf("|"); // On affiche les lignes verticales
        printf("\n");
    }
    for (int a = 0; a < 2.2 * T; a++) // On affiche les lignes horizontales
    {
        printf("-");
    }
    printf("\n");
}

void afficher_coup_possible(int taille, int coup_possible[taille][2])
/*
    Affiche les coups possibles
    args:
        taille : la taille du tableau des coups possibles
        coup_possible : le tableau des coups possibles

    return:
        void

    effet de bord:
        affiche les coups possibles
*/
{
    for (int i = 0; i < taille; i++)
    {
        if (coup_possible[i][0] != -1 || coup_possible[i][1] != -1)
        {
            printf("(%d,%d) ", coup_possible[i][0], coup_possible[i][1]);
        }
    }
}

void vider_buffer()
/*
    Vide le buffer
    args:
        void

    return:
        void

    effet de bord:
        vide le buffer
*/
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // On vide le buffer
}

void charger_partie(const char *nom_fichier, int t, Pion plateau[t][t], Couleur tour[2], int *nb_coups, Coup coups[100], Couleur *gagnant, int position_espion_blanc[2], int position_espion_noir[2])
/*
    Charge une partie depuis un fichier
    args:
        nom_fichier : le nom du fichier à charger
        t : la taille du plateau
        plateau : le plateau de jeu
        tour : la liste des tours
        nb_coups : le nombre de coups joués
        coups : les coups joués
        gagnant : le gagnant de la partie
        position_espion_blanc : la position de l'espion blanc
        position_espion_noir : la position de l'espion noir

    return:
        void

    effet de bord:
        charge une partie depuis un fichier
*/
{
    FILE *fichier = fopen(nom_fichier, "r"); // On ouvre le fichier en lecture seule
    if (fichier == NULL)                     // Si le fichier n'existe pas
    {
        printf("Erreur\n");
        return;
    }

    fscanf(fichier, "B %d %d\n", &position_espion_blanc[0], &position_espion_blanc[1]); // On lit la position de l'espion blanc
    fscanf(fichier, "N %d %d\n", &position_espion_noir[0], &position_espion_noir[1]);   // On lit la position de l'espion noir

    char joueur_actuel;
    fscanf(fichier, "%c\n", &joueur_actuel);         // On lit le joueur actuel
    tour[0] = (joueur_actuel == 'B') ? BLANC : NOIR; // On met à jour le tour des joueurs
    tour[1] = (joueur_actuel == 'B') ? NOIR : BLANC; // On met à jour le tour des joueurs

    *nb_coups = 0;
    while (fscanf(fichier, "%c %d %d -> %d %d\n", &coups[*nb_coups].type, &coups[*nb_coups].x_depart, &coups[*nb_coups].y_depart, &coups[*nb_coups].x_arrive, &coups[*nb_coups].y_arrive) == 5) // On lit les coups si le formatage correspond
    {
        (*nb_coups)++; // On incrémente le nombre de coups
    }

    char joueur_gagnant;
    fscanf(fichier, "%c\n", &joueur_gagnant); // On lit le joueur gagnant
    if (joueur_gagnant == 'B')                // On met à jour le gagnant
    {
        *gagnant = BLANC;
    }
    else if (joueur_gagnant == 'N') // On met à jour le gagnant aussi
    {
        *gagnant = NOIR;
    }
    else // On met à jour le gagnant aussi et là
    {
        *gagnant = -1;
    }

    fclose(fichier); // On ferme le fichier toujours comme de bon programmeur civilisé

    generer_plateau(t, plateau);                                                                         // On génère le plateau
    placer_chateau(t, plateau);                                                                          // On place les chateaux
    placer_chevalier(t, plateau, calculer_chevalier(t), position_espion_blanc, position_espion_noir, 1); // On place les chevaliers
    placer_espions(t, plateau, position_espion_blanc, position_espion_noir);                             // On place les espions

    for (int i = 0; i < *nb_coups; i++)
    {
        Coup coup = coups[i]; // On récupère le coup
        if (coup.type == 'D') // si le coup est un déplacement
        {
            int actionpossible[T * 4][2];
            deplacement_possible(coup.x_depart, coup.y_depart, t, plateau, actionpossible);
            deplacer_pion(coup.x_depart, coup.y_depart, t, plateau, actionpossible, coup.x_arrive, coup.y_arrive);
        }
        else if (coup.type == 'I') // si le coup est une interrogation
        {
            int interrogationpossible[4][2];
            interrogation_possible(coup.x_depart, coup.y_depart, t, plateau, interrogationpossible, tour[0]);
            interroger_pion(coup.x_depart, coup.y_depart, t, plateau, interrogationpossible, coup.x_arrive, coup.y_arrive, tour[0]);
        }
    }
}

int main(int argc, char *argv[]) // On commence le main avec argc qui correpond au nombre d'arguments et argv qui correspond au arguments
{
    int opt;                // On initialise opt
    char *save_file = NULL; // On initialise save_file
    char *load_file = NULL; // On initialise load_file

    while ((opt = getopt(argc, argv, "c:s:d")) != -1) // On parcourt les arguments
    {
        switch (opt) // On switch sur les arguments
        {
        case 's':               // Si l'argument est s
            save_file = optarg; // On met l'argument dans save_file
            break;
        case 'c':               // Si l'argument est c
            load_file = optarg; // On met l'argument dans load_file
            break;
        case 'd':      // Si l'argument est d
            DEBUG = 0; // On met le mode debug à 0
            break;
        default:                // Si l'argument est invalide
            afficher_aide();    // On affiche l'aide
            exit(EXIT_FAILURE); // et on quitte le programme
        }
    }

    if (DEBUG == 0) // Si le mode debug est activé
    {
        printf("DEBUG MODE\n"); // On affiche le mode debug
    }
    else // Si le mode debug n'est pas activé
    {
        printf("NORMAL MODE\n"); // On affiche le mode normal
    }

    Couleur tour[2] = {BLANC, NOIR}; // On initialise les tours
    if (rand() % 2 == 0)             // On choisit aléatoirement le joueur qui commence
    {
        changement_tour(2, tour); // On change le tour
    }

    int chevalier;                // On initialise le nombre de chevalier
    Pion plateau[T][T];           // On initialise le plateau
    int position_espion_blanc[2]; // On initialise la position de l'espion blanc
    int position_espion_noir[2];  // et on initialise la position de l'espion noir comme ca pas de jaloux
    int nb_coups = 0;             // On initialise le nombre de coups
    Coup coups[100];              // On initialise les coups
    Couleur gagnant = -1;         // On initialise le gagnant

    if (load_file) // Si on charge une partie
    {
        printf("Chargement de la partie depuis %s\n", load_file);
        charger_partie(load_file, T, plateau, tour, &nb_coups, coups, &gagnant, position_espion_blanc, position_espion_noir); // On charge la partie
    }

    else // Si on ne charge pas de partie
    {
        chevalier = calculer_chevalier(T);                                                       // On calcule le nombre de chevalier
        generer_plateau(T, plateau);                                                             // On génère le plateau
        placer_chateau(T, plateau);                                                              // On place les chateaux
        placer_chevalier(T, plateau, chevalier, position_espion_blanc, position_espion_noir, 0); // On place les chevaliers
    }

    printf("Bienvenue dans Incognito \n");
    afficher_plateau(T, plateau, tour[0], DEBUG); // On affiche le plateau

    while (victoire(T, plateau) == 1) // Tant qu'il n'y a pas de victoire
    {
        int pionselectionne[2] = {0, 0}; // On remets tout à 0
        int pion_arrive[2] = {0, 0};
        int actionpossible[T * 4][2];
        int interrogationpossible[4][2];
        char moov[1];
        printf("\n Joueur %s, quelle pion voulez vous selectionner (Format : X Y ) : ", couleur_to_string(tour[0]));
        scanf("%d", &pionselectionne[0]); // On demande au joueur de selectionner un pion
        scanf("%d", &pionselectionne[1]);
        vider_buffer(); // On vide le buffer
        printf("La piece selectionnee est (%d,%d)\n", pionselectionne[0], pionselectionne[1]);
        int type_pion = plateau[pionselectionne[1]][pionselectionne[0]].type;                // On récupère le type du pion
        Couleur couleur_pion = plateau[pionselectionne[1]][pionselectionne[0]].couleur;      // On récupère la couleur du pion
        while ((type_pion != CHEVALIER && type_pion != ESPION) || (couleur_pion != tour[0])) // Tant que le pion n'est pas un chevalier ou un espion ou que le pion n'est pas de la couleur du joueur
        {
            afficher_plateau(T, plateau, tour[0], DEBUG);
            printf("Entree invalide veuillez recommencez \n (Soit ce n'est pas un pion de votre couleur, soit l'entree est hors du plateau)");
            printf("\n Joueur %s, quelle pion voulez vous selectionner (Format : X Y ) : ", couleur_to_string(tour[0]));
            vider_buffer();
            scanf("%d", &pionselectionne[0]); // On redemande au joueur de selectionner un pion tout le monde a le droit à une seconde chance
            scanf("%d", &pionselectionne[1]);
            int type_pion = plateau[pionselectionne[1]][pionselectionne[0]].type;
            Couleur couleur_pion = plateau[pionselectionne[1]][pionselectionne[0]].couleur;
        }
        deplacement_possible(pionselectionne[0], pionselectionne[1], T, plateau, actionpossible);                   // On récupère les déplacements possibles
        interrogation_possible(pionselectionne[0], pionselectionne[1], T, plateau, interrogationpossible, tour[0]); // On récupère les interrogations possibles
        printf("\n Les coups possible sont : \n");
        afficher_coup_possible(T * 4, actionpossible); // On affiche les coups possibles
        printf("\n Les interrogation possible sont : \n");
        afficher_coup_possible(4, interrogationpossible); // On affiche les interrogations possibles
        printf("\n");
        printf("Choisissez entre deplacement, interrogation et sauvegarde (d, i ou s) : "); // On demande au joueur de choisir entre deplacement, interrogation et sauvegarde
        scanf("%c", &moov[0]);
        while (moov[0] != 'd' && moov[0] != 'i' && moov[0] != 's') // Tant que le mouvement n'est pas valide
        {
            printf("Entree invalide veuillez recommencez (d, i ou s) :  ");
            vider_buffer();
            scanf("%c", &moov[0]); // On redemande au joueur de choisir entre deplacement, interrogation et sauvegarde encore une deuxième chance
        }
        vider_buffer();
        if (moov[0] == 'd') // Si le joueur a choisi de se déplacer
        {
            printf("Choisissez votre deplacement (Format : X Y ) : ");
            scanf("%d", &pion_arrive[0]); // On demande au joueur de choisir où il veut se déplacer
            scanf("%d", &pion_arrive[1]);
            while (deplacer_pion(pionselectionne[0], pionselectionne[1], T, plateau, actionpossible, pion_arrive[0], pion_arrive[1])) // Tant que le déplacement n'est pas possible
            {
                printf("Deplacement impossible\n");
                printf("Choisissez votre deplacement (Format : X Y ) : ");
                vider_buffer();
                scanf("%d", &pion_arrive[0]); // Deuxième chance pour le joueur de choisir où il veut se déplacer
                scanf("%d", &pion_arrive[1]);
            }
            coups[nb_coups++] = (Coup){'D', pionselectionne[0], pionselectionne[1], pion_arrive[0], pion_arrive[1]}; // On ajoute le coup joué
            changement_tour(2, tour);                                                                                // On change de tour
        }
        else if (moov[0] == 'i') // Si le joueur a choisi d'interroger
        {
            printf("Choisissez le pion que vous souhaitez interroger (Format : X Y ) : "); // On demande au joueur de choisir le pion à interroger
            scanf("%d", &pion_arrive[0]);
            scanf("%d", &pion_arrive[1]);
            while (interroger_pion(pionselectionne[0], pionselectionne[1], T, plateau, interrogationpossible, pion_arrive[0], pion_arrive[1], tour[0]) == 2) // Tant que l'interrogation n'est pas possible
            {
                printf("interrogation impossible\n");
                printf("Choisissez le pion que vous souhaitez interroger (Format : X Y ) : ");
                vider_buffer();
                scanf("%d", &pion_arrive[0]); // Deuxième chance pour le joueur de choisir le pion à interroger
                scanf("%d", &pion_arrive[1]);
            }
            coups[nb_coups++] = (Coup){'I', pionselectionne[0], pionselectionne[1], pion_arrive[0], pion_arrive[1]}; // On ajoute le coup joué
            changement_tour(2, tour);                                                                                // On change de tour
        }
        else if (moov[0] == 's') // Si le joueur a choisi de sauvegarder
        {
            if (save_file) // Si un fichier de sauvegarde a été spécifié
            {
                sauvegarder_partie(save_file, T, plateau, tour, nb_coups, coups, gagnant, position_espion_blanc, position_espion_noir); // On sauvegarde la partie là bas
                printf("Partie sauvegardée\n");
            }
            else // Si aucun fichier de sauvegarde n'a été spécifié
            {
                sauvegarder_partie("sauvegarde.inco", T, plateau, tour, nb_coups, coups, gagnant, position_espion_blanc, position_espion_noir); // On sauvegarde la partie dans sauvegarde.inco il fallait pas être indécis
                printf("Partie sauvegardée dans sauvegarde.inco car aucun fichier de sauvegarde n'a été spécifié\n");
            }
            return 0;
        }
        afficher_plateau(T, plateau, tour[0], DEBUG); // On affiche le plateau
    }

    if (save_file) // Si un fichier de sauvegarde a été spécifié
    {
        sauvegarder_partie(save_file, T, plateau, tour, nb_coups, coups, gagnant, position_espion_blanc, position_espion_noir); // On sauvegarde la partie là bas en fin de partie
        printf("Partie sauvegardée\n");
    }
    return 0;
}