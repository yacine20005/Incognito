int victoire(int t, Pion plateau[t][t])
{
    /*
    Condition de victoire, parcoure tout le plateau et verifie s'il y a 2 espion ou pas,
    s'il y en a qu'un ou 0 la partie est fini donc on renvoie False, sinon True
    */
    int listeespion[2][2];
    int nbespion = 0;
    for (int y = 0; y < t; y++)
    {
        for (int x = 0; x < t; x++)
        {
            if (plateau[y][x].type == ESPION)
                listeespion[nbespion][0] = x;
                listeespion[nbespion][1] = y;
                nbespion += 1;
        }
    }
    if(nbespion < 2)
        printf("\n \n Victoire des %s !\n \n", plateau[listeespion[0][1]][listeespion[0][0]].couleur);
    return nbespion == 2 ? 1 : 0;
}