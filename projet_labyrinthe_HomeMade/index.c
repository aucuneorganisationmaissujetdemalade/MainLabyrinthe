#include "graphics.h"

#define nbFantomes 1

#define NB_CASE_X 50
#define NB_CASE_Y 50

#define TAILLE_ECRAN_X 500
#define TAILLE_ECRAN_Y 500

#define TAILLE_CASE_X (TAILLE_ECRAN_X/NB_CASE_X)
#define TAILLE_CASE_Y (TAILLE_ECRAN_Y/NB_CASE_Y)

typedef struct{
	char cases[NB_CASE_X][NB_CASE_Y];
	POINT joueur, sortie;
	POINT fantomes[nbFantomes];
} LABYRINTHE;

POINT piece2pixel(POINT piece){
	piece.x = TAILLE_CASE_X*piece.x;
	piece.y = TAILLE_CASE_Y*piece.y;
	return piece;
}

POINT pixel2piece(POINT pixel){
	pixel.x = pixel.x/TAILLE_CASE_X;
	pixel.y = pixel.y/TAILLE_CASE_Y;
	return pixel;
}

int TaillePerso(){
	if(TAILLE_CASE_X<TAILLE_CASE_Y){
		return(TAILLE_CASE_X/4);
	}
	else{
		return(TAILLE_CASE_Y/4);
	}
	
}

void affiche_joueur(LABYRINTHE L){
	int TaillePersonnage;
	COULEUR CouleurPersonnage;
	
	TaillePersonnage = TaillePerso();
	CouleurPersonnage = 0xFFF400;
	draw_fill_circle(L.joueur,TaillePersonnage,CouleurPersonnage);
}

LABYRINTHE init_labyrinthe(LABYRINTHE L){
	int TestX, TestY;
	for(TestX = 0; TestX<NB_CASE_X; TestX++){
		for(TestY = 0; TestY<NB_CASE_Y; TestY++){
			if(TestX == 0 || TestX == NB_CASE_X - 1 || TestY == 0 || TestY == NB_CASE_Y - 1){
				L.cases[TestX][TestY] = 'm';
			}
			else{					
				L.cases[TestX][TestY] = 'v';
			}
		}
	}
	L.joueur.x = TAILLE_ECRAN_X - ((TAILLE_CASE_X*3)/2);
	L.joueur.y = (TAILLE_CASE_Y*3)/2;
	L.sortie.x = 1; 
	L.sortie.y = NB_CASE_Y - 2;
	return L;
}

void affiche_mur(POINT M){
	POINT Mur1,Mur2;
	COULEUR CouleurMur;
	CouleurMur = 0x3F5151;
	
	Mur1 = piece2pixel(M);
	Mur2.x = Mur1.x + TAILLE_CASE_X;
	Mur2.y = Mur1.y + TAILLE_CASE_Y;
	draw_fill_rectangle(Mur1,Mur2,CouleurMur);
}

void affiche_sortie(LABYRINTHE L){
	POINT Sortie1,Sortie2;
	COULEUR CouleurSortie;
	CouleurSortie = 0x834A20;
	
	Sortie1 = piece2pixel(L.sortie);
	Sortie1.x = Sortie1.x + 1;
	Sortie2.x = Sortie1.x + TAILLE_CASE_X;   Sortie2.y = Sortie1.y + TAILLE_CASE_Y - 1;
	
	draw_fill_rectangle(Sortie1,Sortie2,CouleurSortie);
}

void affiche_labyrinthe(LABYRINTHE L){
	int TestX, TestY;
	for(TestX = 0; TestX<NB_CASE_X; TestX++){
		for(TestY = 0; TestY<NB_CASE_Y; TestY++){
			if(L.cases[TestX][TestY] == 'm'){
				POINT TEMP = {TestX,TestY};
				affiche_mur(TEMP);
			}
			else{			
				
			}
		}
	}
	affiche_sortie(L);
	affiche_joueur(L);
	affiche_all();
}

BOOL deplacement_possible(POINT directionJoueur, LABYRINTHE L){
	POINT CaseTest,Cible;
	int TestMultiDir;
	
	for(TestMultiDir = 0; TestMultiDir < 4;TestMultiDir ++){
		if(TestMultiDir == 0){
			Cible.x = TaillePerso(); Cible.y = 0;
		}
		if(TestMultiDir == 1){
			Cible.x = -TaillePerso(); Cible.y = 0;
		}
		if(TestMultiDir == 2){
			Cible.x = 0; Cible.y = TaillePerso();
		}
		if(TestMultiDir == 3){
			Cible.x = 0; Cible.y = -TaillePerso();
		}
		CaseTest.x = directionJoueur.x + L.joueur.x + Cible.x;			
		CaseTest.y = directionJoueur.y + L.joueur.y + Cible.y;
		CaseTest = pixel2piece(CaseTest);
		if(L.cases[CaseTest.x][CaseTest.y] == 'm'){
			return false;
		}
	}
	return true;
}

LABYRINTHE deplace_joueur(POINT directionJoueur, LABYRINTHE L){
	if(deplacement_possible(directionJoueur,L)){
		L.joueur.x = L.joueur.x + directionJoueur.x;
		L.joueur.y = L.joueur.y + directionJoueur.y;
	}
	return L;
}

BOOL jeu_fini(LABYRINTHE L){
	if(pixel2piece(L.joueur).x == L.sortie.x &&  pixel2piece(L.joueur).y == L.sortie.y){ 
		return true;
	}
	else{
		return false;
	}
}

int main() 
{
	LABYRINTHE L; 
	POINT directionJoueur;
	
	init_graphics(TAILLE_ECRAN_X,TAILLE_ECRAN_Y);
	fill_screen(0x765154);
	affiche_auto_off();
	
	L = init_labyrinthe(L);
	affiche_labyrinthe(L);
	while(!jeu_fini(L)){
		SDL_Delay(4);
		fill_screen(0x765154);
		directionJoueur = get_arrow();
		L = deplace_joueur(directionJoueur, L);
		affiche_labyrinthe(L);
	}
	affiche_all();
	wait_escape();
	exit(0);
}
