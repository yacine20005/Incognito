#ifndef TYPES_H
#define TYPES_H

#define T 5

typedef enum _couleur
{
    BLANC,
    NOIR
} Couleur;

typedef enum _type
{
    CHEVALIER,
    ESPION,
    CHATEAU
} Type;

typedef struct _pion
{
    Type type; //chevalier ou espion?
    Couleur couleur;
} Pion;

typedef struct
{
    int x, y;
} Case, Direction;

typedef struct _mouvement
{
    Case depart;
    Case arrive;
} Mouvement;

typedef struct _jeu
{
    Pion * plateau[T][T];
    Couleur joueur;
} Jeu;

#endif