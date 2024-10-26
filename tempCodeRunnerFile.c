int deplacer_pion(int x_depart, int y_depart, int t, Pion plateau[t][t], int actionpossible[T * 4][2], int x_arrive, int y_arrive)
{
    int i = 0;
    while (actionpossible[i][0] != -1 && actionpossible[i][1] != -1)
    {
        if (actionpossible[i][0] == x_arrive && actionpossible[i][1] == y_arrive)
        {
            if(plateau[y_arrive][x_arrive].type == CHATEAU && plateau[y_depart][x_depart].type == CHEVALIER) {
                plateau[y_depart][x_depart].type = -1;
                plateau[y_depart][x_depart].couleur = -1;
                return 0;
            }
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