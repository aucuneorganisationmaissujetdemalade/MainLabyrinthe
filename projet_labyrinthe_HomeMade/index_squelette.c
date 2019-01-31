#include "graphics.h"

#define nbFantomes 1

#define NB_CASE_X 30
#define NB_CASE_Y 20

#define TAILLE_ECRAN_X 900
#define TAILLE_ECRAN_Y 600

typedef struct
{
	char labyrinthe[NB_CASE_X][NB_CASE_Y];
	POINT joueur;
	POINT fantomes[nbFantomes];
} LABYRINTHE;

LABYRINTHE init_labyrinthe(LABYRINTHE L)
{
	return L;
}

void affiche_labyrinthe(LABYRINTHE L)
{
	
}

BOOL deplacement_possible(POINT directionJoueur, LABYRINTHE L)
{
	return false;
}

LABYRINTHE deplace_joueur(POINT directionJoueur, LABYRINTHE L)
{
	return L;
}

BOOL jeu_fini()
{
	return true;
}

int main() 
{
	LABYRINTHE L; 
	POINT directionJoueur;
	
	init_graphics(TAILLE_ECRAN_X,TAILLE_ECRAN_Y);
	fill_screen(black);
	affiche_auto_off();
	
	L = init_labyrinthe(L);
	affiche_labyrinthe(L);
	while(!jeu_fini(L))
	{
		directionJoueur = get_arrow();
		L = deplace_joueur(directionJoueur, L);
		affiche_labyrinthe(L);
	}
	affiche_all();
	wait_escape();
	exit(0);
}
