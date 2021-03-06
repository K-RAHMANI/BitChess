#define MAX 1
#define MIN -1
#define INFINI 10000
#include "definition.h"

void copier( struct configuration *c1, struct configuration *c2 ) /// Copie la configuration c1 dans c2
{
	int i, j;

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			c2->board[i][j] = c1->board[i][j];
	for(i=0;i<4;i++)
        c2->roque[i] = c1->roque[i];
	c2->turn = c1->turn;
	c2->val = c1->val;
}

int egal(char c1[8][8], char c2[8][8] ) /// Teste si les conf c1 et c2 sont �gales
{
	int i, j;

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			if (c1[i][j] != c2[i][j]) return 0;
	return 1;
}

int feuille( struct configuration conf, int *cout ) /// Teste si conf repr�sente une fin de partie et retourne dans 'cout' la valeur associ�e
{
	*cout = 0;
    int i=0,j=0,pasroiN=0,pasroiB=0;

    for (i=0;i<8;i++){
        for (j=0;j<8;j++){
            if(conf.board[i][j]== 'R'){
               pasroiB=1;
            }
        }
    }
	// Si victoire pour les Blancs cout = +10000
	for (i=0;i<8;i++){
        for (j=0;j<8;j++){
            if(conf.board[i][j]== -'R'){
               pasroiN=1;
            }
        }
    }
    if(!pasroiB) {
        *cout = -10000;
        return 1;
    }
    if(!pasroiN) {
        *cout = +10000;
        return 1;
    }
	// Sinon ce n'est pas une config feuille
	return 0;
}

int minmax_ab( struct configuration conf, int mode, int niv, int alpha, int beta ) /// MinMax avec elagage alpha-beta
{
 	int n, i, score, score2;
 	struct configuration T[100];

   	if ( feuille(conf, &score) )
	    return score;

   	if ( niv == 0 )
		return evaluation( conf );

   	if ( mode == MAX ) {

	   n=cout_possible(T, conf, MAX);
	   score = alpha;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = minmax_ab( T[i], MIN, niv-1, score, beta );
		if (score2 > score) score = score2;
		if (score > beta) {
   	      		return beta;
	    	}
	   }
	}
	else  { // mode == MIN
	   n=cout_possible(T, conf, MIN);
	   score = beta;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = minmax_ab( T[i], MAX, niv-1, alpha, score );
		if (score2 < score) score = score2;
		if (score < alpha) {
   	      		return alpha;
	    	}
	   }
	}
        if ( score == +INFINI ) score = +10000;
        if ( score == -INFINI ) score = -10000;
	return score;
}

int minmax( struct configuration conf, int mode, int niv) /// MinMax sans elagage alpha-beta
{
 	int n, i, score, score2;
 	struct configuration T[100];
   	if ( feuille(conf, &score) )
		return score;
   	if ( niv == 0 )
		return evaluation( conf );

    if ( mode == MAX ) {
            score = -INFINI;
   	}else{
            score = +INFINI;
    }
    if ( mode == MAX ) {
	   n = cout_possible( T, conf, MAX);
	   for ( i=0; i<n; i++ ) {
        score2 = minmax( T[i], MIN, niv-1);
		if (score2 > score) score = score2;
	   }
	}
	else  {
	   n=cout_possible(T, conf, MIN);
	   for ( i=0; i<n; i++ ) {
   	    	score2 = minmax( T[i], MAX, niv-1);
		if (score2 < score) score = score2;
	   }
	}
    if ( score == +INFINI ) score = +10000;
    if ( score == -INFINI ) score = -10000;
	return score;
}

void getCMD(configuration board1, configuration board2, char *cmd[5]) /// Conclure la comande du mouvement � partire du d�placement �ffectu� par la machine
{
    int i, j, x, y;
    bool get = false;

    if((board1.board[0][0] == 'T') && (board1.board[0][4] == 'R') && (board2.board[0][0] == 0) && (board2.board[0][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '1';
        cmd[2] = 'c';
        cmd[3] = '1';
        get = true;
    }
    if((board1.board[7][0] == -'T') && (board1.board[7][4] == -'R') && (board2.board[7][0] == 0) && (board2.board[7][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '8';
        cmd[2] = 'c';
        cmd[3] = '8';
        get = true;
    }
    if((board1.board[0][7] == 'T') && (board1.board[0][4] == 'R') && (board2.board[0][7] == 0) && (board2.board[0][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '1';
        cmd[2] = 'g';
        cmd[3] = '1';
        get = true;
    }
    if((board1.board[7][7] == -'T') && (board1.board[7][4] == -'R') && (board2.board[7][7] == 0) && (board2.board[7][4] == 0))
    {
        cmd[0] = 'e';
        cmd[1] = '8';
        cmd[2] = 'g';
        cmd[3] = '8';
        get = true;
    }
    if(!get)
    {
        for(i = 0 ; i <= 7 ; i++)
        {
            for(j = 0 ; j <= 7 ; j++)
            {
                if((board1.board[i][j] != 0) && (board2.board[i][j] == 0))
                {
                    x = i;
                    y = j;
                    cmd[1] = i + '1';
                    cmd[0] = j + 'a';
                }
            }
        }
        for(i = 0 ; i <= 7 ; i++)
        {
            for(j = 0 ; j <= 7 ; j++)
            {
                if((board1.board[i][j] != board2.board[i][j]) && ((x != i) || (y != j)))
                {
                    cmd[3] = i + '1';
                    cmd[2] = j + 'a';
                }
            }
        }
    }
}
