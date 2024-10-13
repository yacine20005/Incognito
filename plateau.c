#include <stdio.h> //Tjr là tjr présent
#include "types.h" //Flm de mettre les types ici donc je les ai mis dans un autre fichier
#include <time.h> 
#include <stdlib.h.>
#include <string.h>

#define T 5 //taille du plateau TxT

int directions_haut[3][2] = {{0, 1}, {1, 0}, {1, 1}}; // Les directions où il est possible de placer les chevaliers pour le château en haut à gauche

int directions_bas[3][2] = {{0, -1}, {-1, 0}, {-1, -1}}; // même chose pour le château en bas à droite


int changementdetour(int t, char *tour[t]) {
    char *tmp = tour[0];
    *tour[0] = *tour[1];
    *tour[1] = *tmp;
}

char *couleur_to_string(Couleur c) {
    switch(c) {
        case BLANC: return "Blanc";
        case NOIR: return "Noir";
        default: return "inconnue";
    }
}

void possibilite(int x, int y, int t, Pion plateau[t][t], char actionpossible[3][3], char * couleur) {
    /* prend en paramètre les coordonnées du pion que l'on veut checker */
    int directionpossible[8][2] = {{-1,-1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

    // Initialiser actionpossible avec des cases vides (ou autre valeur par défaut)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            actionpossible[i][j] = 'Y';
        }
    }
    
    // Le pion de référence est au centre du tableau 3x3
    actionpossible[1][1] = 'X'; // Par exemple 'X' pour indiquer le pion lui-même

    // Parcourir les directions autour du pion
    for(int i = 0; i < 8; i++) {
        int newx = x + directionpossible[i][0];
        int newy = y + directionpossible[i][1];

        // Vérification des limites du plateau
        if (newx >= 0 && newx < t && newy >= 0 && newy < t) {
            int relativeX = 1 + directionpossible[i][0]; // Calcul de l'index relatif pour actionpossible
            int relativeY = 1 + directionpossible[i][1];

            // Gérer les différents types de cases du plateau
            switch(plateau[newy][newx].type) {
                case CHATEAU:
                    actionpossible[relativeY][relativeX] = 'C'; 
                    break;
                case ESPION:
                case CHEVALIER:
                     if(couleur_to_string(plateau[newx][newy].couleur)==couleur) {
                        actionpossible[relativeY][relativeX] = 'F'; // Ami
                    } else {
                        actionpossible[relativeY][relativeX] = 'I'; // Ennemi
                    }
                    break;
                default:
                    actionpossible[relativeY][relativeX] = 'D'; 
                    break;
            }
        }
    }
}


int victoire(int t, Pion plateau[t][t], int nbtour){
    /*
    Condition de victoire, parcoure tout le plateau et verifie s'il y a 2 espion ou pas, 
    s'il y en a qu'un ou 0 la partie est fini donc on renvoie False, sinon True
    
    */
    if(nbtour > 2)
        return 0;
    int nbespion = 0;
     for (int y = 0; y < t; y++) {
        for (int x = 0; x < t; x++) {
            if(plateau[y][x].type==ESPION)
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
    int nbrandomblanc = rand() %chevalier + 1;
    int nbrandomnoir = rand() %chevalier + 1;
    printf("\n \n \n %d, %d \n \n \n", nbrandomblanc, nbrandomnoir);
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
                printf("Chevalier place en (%d), (%d) et il reste %d chevalier en boucle numero :%d \n", nx, ny, chevalier, i);
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
        printf("%d \n", i);
    }
    printf("\n");
}

void afficher_couppossible(char couppossible[3][3]){
    for(int i = 0; i<3; i++) {
        printf("\n");
        for(int j = 0; j<3; j++) {
            printf("%c",couppossible[i][j]);
    }
    }
}

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Lire et ignorer tous les caractères jusqu'à la fin de la ligne
}



int main()
{
    int t = T;
    int nbtour = 0;
    char *tour[2] = {"Noir","Blanc"};
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
    placer_chateau(T, plateau);
    placer_chevalier(T, plateau, chevalier);
    printf("Bienvenue dans Incognito \n");
    afficher_plateau(T, plateau);
    printf("%d \n", victoire(T, plateau, nbtour));
    while(victoire(T, plateau, nbtour) == 1) {
        int pionselectionne[2];
        char actionpossible[3][3]; // ordre [En Haut a gauche, haut mid, haut droite, mid gauche, mid mid, mid droite, bas gauche, bas mid, droite mid]
        printf("\n Joueur %s, quelle pion voulez vous selectionner (Format : X Y ) : ", tour[0]);
        scanf("%d", &pionselectionne[0]);
        scanf("%d", &pionselectionne[1]);
        printf("La piece selectionnee est (%d,%d)", pionselectionne[0], pionselectionne[1]);
        while((pionselectionne[0] < 0 || pionselectionne[0] > T) || (pionselectionne[1] < 0 || pionselectionne[1] > T) || (plateau[pionselectionne[1]][pionselectionne[0]].type != (CHEVALIER || ESPION) && couleur_to_string(plateau[pionselectionne[1]][0].couleur)==tour[0])){
            printf("Entree invalide veuillez recommencez \n (Soit ce n'est pas un pion de votre couleur, soit l'entrée est hors du plateau)");
            printf("\n Joueur %s, quelle pion voulez vous selectionner (Format : X Y ) : ", tour[0]);
            vider_buffer(); // temporairement c'est chagpt qui l'as fait prcq la boucle infinie elle clc
            //fflush(stdin) ; le prof avait dit de use ca mais ca marchais pas.
            scanf("%d", &pionselectionne[0]);
            scanf("%d", &pionselectionne[1]);
        }
        possibilite(pionselectionne[0], pionselectionne[1],T, plateau, actionpossible,tour[0]);
        printf("\n Les coups possible sont : \n");
        afficher_couppossible(actionpossible);
        nbtour +=1;
        changementdetour(2, tour);
    }
    return 0;
}


