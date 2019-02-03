#include "graphics.h"

#define nbFantomes 1

#define NB_CASE_X 20
#define NB_CASE_Y 20

#define TAILLE_ECRAN_X 600
#define TAILLE_ECRAN_Y 600

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

BOOL clicEdit(POINT P){
	if(P.x > (TAILLE_ECRAN_X/NB_CASE_X)*5 && P.x < TAILLE_ECRAN_X -(TAILLE_ECRAN_X/NB_CASE_X)*5 
				&& 
	   P.y > (TAILLE_ECRAN_Y/NB_CASE_Y)*13 && P.y < TAILLE_ECRAN_Y -(TAILLE_ECRAN_Y/NB_CASE_Y)*3)
	{
		return true;
	}
	return false;
}
BOOL clicPlay(POINT P){
	if(P.x > (TAILLE_ECRAN_X/NB_CASE_X)*5 && P.x < TAILLE_ECRAN_X -(TAILLE_ECRAN_X/NB_CASE_X)*5 
				&& 
	   P.y > (TAILLE_ECRAN_Y/NB_CASE_Y)*3 && P.y < TAILLE_ECRAN_Y -(TAILLE_ECRAN_Y/NB_CASE_Y)*13)
	{
		return true;
	}
	return false;
}

BOOL clicEditFini(POINT P){
	if(P.x > (TAILLE_ECRAN_X/NB_CASE_X)*7 && P.x < TAILLE_ECRAN_X -(TAILLE_ECRAN_X/NB_CASE_X)*7
				&&
	   P.y > 0 && P.y < TAILLE_ECRAN_Y -(TAILLE_ECRAN_Y/NB_CASE_Y)*19 ){
		   return true;
	}
	return false;
}

void AfficheMenuPrincipal(){
	POINT bg_edit_button, hd_edit_button, bg_play_button, hd_play_button, bg_edit_text, bg_play_text;
	
	bg_edit_button.x = (TAILLE_ECRAN_X/NB_CASE_X)*5;
	bg_edit_button.y = (TAILLE_ECRAN_Y/NB_CASE_Y)*13;
	
	hd_edit_button.x = TAILLE_ECRAN_X -(TAILLE_ECRAN_X/NB_CASE_X)*5;
	hd_edit_button.y = TAILLE_ECRAN_Y -(TAILLE_ECRAN_Y/NB_CASE_Y)*3;
	
	draw_fill_rectangle(bg_edit_button, hd_edit_button, bleu);
	
	bg_play_button.x = (TAILLE_ECRAN_X/NB_CASE_X)*5;
	bg_play_button.y = (TAILLE_ECRAN_Y/NB_CASE_Y)*3;
	
	hd_play_button.x = TAILLE_ECRAN_X -(TAILLE_ECRAN_X/NB_CASE_X)*5;
	hd_play_button.y = TAILLE_ECRAN_Y -(TAILLE_ECRAN_Y/NB_CASE_Y)*13;
	
	draw_fill_rectangle(bg_play_button, hd_play_button, citronvert);
	
	bg_edit_text.x = bg_edit_button.x + (TAILLE_ECRAN_X/NB_CASE_X)*1.1;
	bg_edit_text.y = bg_edit_button.y + (TAILLE_ECRAN_Y/NB_CASE_Y)*2.7;
	aff_pol("Ouvrir l'editeur", 20, bg_edit_text,white);
	
	bg_play_text.x = bg_play_button.x + (TAILLE_ECRAN_X/NB_CASE_X)*1.2;
	bg_play_text.y = bg_play_button.y + (TAILLE_ECRAN_Y/NB_CASE_Y)*2.7;
	aff_pol("Nouvelle Partie", 20, bg_play_text,white);
	
	affiche_all();
}

void affiche_bouton_edit_fini(){
	POINT bg_play_button, hd_play_button, bg_play_text;
	bg_play_button.x = (TAILLE_ECRAN_X/NB_CASE_X)*7;
	bg_play_button.y = 0;
	
	hd_play_button.x = TAILLE_ECRAN_X -(TAILLE_ECRAN_X/NB_CASE_X)*7;
	hd_play_button.y = TAILLE_ECRAN_Y -(TAILLE_ECRAN_Y/NB_CASE_Y)*19;
	
	draw_fill_rectangle(bg_play_button, hd_play_button, citronvert);
	
	bg_play_text.x = bg_play_button.x + (TAILLE_ECRAN_X/NB_CASE_X)*1.9;
	bg_play_text.y = 22;
	aff_pol("Valider", 15, bg_play_text,white);
	
	affiche_all();
}

LABYRINTHE edit_labyrinthe(LABYRINTHE L, POINT clic){
	clic = pixel2piece(clic);
	if(clic.x >= NB_CASE_X - 1 || clic.y >= NB_CASE_Y - 1
	|| clic.x < 1 || clic.y < 1)
	{
		printf("\n \n cMORT \n \n");
	}
	else if(L.cases[clic.x][clic.y] == 'm')
	{
		L.cases[clic.x][clic.y] = 'v';
	}
	else if(L.cases[clic.x][clic.y] == 'v')
	{
		L.cases[clic.x][clic.y] = 'm';
	}
	return L;
}

LABYRINTHE MenuMainEdit(LABYRINTHE L){ 
	POINT clic;
	BOOL jouer, editFini;
	editFini = false;
	jouer = false;
	AfficheMenuPrincipal();
	while(!jouer)
	{
		clic = wait_clic();
		if(clicEdit(clic))
		{
			printf("\n \n caEdit \n \n");
			while(!editFini)
			{
				affiche_labyrinthe_edit(L);
				affiche_bouton_edit_fini();
				clic = wait_clic();
				L = edit_labyrinthe(L, clic);
				if(clicEditFini(clic)){
					editFini = true;
				}
			}
			jouer = true;	
		} 
		else if(clicPlay(clic))
		{
			printf("\n \n caJoue \n \n");
			jouer = true;
		}
	}
	return L;
}

void affiche_joueur(LABYRINTHE L){
	int TaillePersonnage;
	COULEUR CouleurPersonnage;
	
	TaillePersonnage = TaillePerso();
	CouleurPersonnage = 0xFFF400;
	draw_fill_circle(L.joueur,TaillePersonnage,CouleurPersonnage);
	POINT HitBox, HitBox2;
	HitBox.x = L.joueur.x - TaillePersonnage - 1; 
	HitBox.y = L.joueur.y - TaillePersonnage - 1; 
	
	HitBox2.x = L.joueur.x + TaillePersonnage + 1; 
	HitBox2.y = L.joueur.y + TaillePersonnage + 1; 
	
	draw_rectangle(HitBox,HitBox2,blanc);
}

LABYRINTHE init_labyrinthe(LABYRINTHE L){
	int InitX, InitY;
	L.sortie.x = 1; 
	L.sortie.y = NB_CASE_Y - 1;
	for(InitX = 0; InitX<NB_CASE_X; InitX++){
		for(InitY = 0; InitY<NB_CASE_Y; InitY++){
			if(InitX == 0 || InitX == NB_CASE_X - 1 || InitY == 0 || InitY == NB_CASE_Y - 1){
				L.cases[InitX][InitY] = 'm';
			}
			else{					
				L.cases[InitX][InitY] = 'v';
			}
			if(InitX == L.sortie.x && InitY == L.sortie.y)
			{
				L.cases[InitX][InitY] = 'v';
			}
		}
	}
	L.joueur.x = TAILLE_ECRAN_X - ((TAILLE_CASE_X*3)/2);
	L.joueur.y = (TAILLE_CASE_Y*3)/2;
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

void affiche_labyrinthe_edit(LABYRINTHE L){
	fill_screen(0x765154);
	int TestX, TestY;
	for(TestX = 0; TestX<NB_CASE_X; TestX++){
		for(TestY = 0; TestY<NB_CASE_Y; TestY++){
			if(L.cases[TestX][TestY] == 'm'){
				POINT Mur = {TestX,TestY};
				affiche_mur(Mur);
			}
		}
	}
	affiche_sortie(L);
	affiche_all();
}

void affiche_labyrinthe_jeu(LABYRINTHE L){
	int TestX, TestY;
	for(TestX = 0; TestX<NB_CASE_X; TestX++){
		for(TestY = 0; TestY<NB_CASE_Y; TestY++){
			if(L.cases[TestX][TestY] == 'm'){
				POINT Mur = {TestX,TestY};
				affiche_mur(Mur);
			}
		}
	}
	affiche_sortie(L);
	affiche_joueur(L);
	affiche_all();
}

BOOL deplacement_possible(POINT directionJoueur, LABYRINTHE L){
	POINT PointTest,Direction;
	int TestMultiDir;
	
	for(TestMultiDir = 0; TestMultiDir < 4;TestMultiDir ++){
		if(TestMultiDir == 0){
			Direction.x = TaillePerso()+1; Direction.y = 0;
		}
		if(TestMultiDir == 1){
			Direction.x = -TaillePerso()-1; Direction.y = 0;
		}
		if(TestMultiDir == 2){
			Direction.x = 0; Direction.y = TaillePerso()+1;
		}
		if(TestMultiDir == 3){
			Direction.x = 0; Direction.y = -TaillePerso()-1;
		}
		PointTest.x = directionJoueur.x + L.joueur.x + Direction.x;			
		PointTest.y = directionJoueur.y + L.joueur.y + Direction.y;
		PointTest = pixel2piece(PointTest);
		if(L.cases[PointTest.x][PointTest.y] == 'm'){
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

void fps(){
	int Delay;
	Delay = (TAILLE_ECRAN_X+TAILLE_ECRAN_Y)/200;
	SDL_Delay(Delay);
}

int main(){
	LABYRINTHE L; 
	POINT directionJoueur;
	
	init_graphics(TAILLE_ECRAN_X,TAILLE_ECRAN_Y);
	fill_screen(0x765154);
	affiche_auto_off();
	
	L = init_labyrinthe(L);
	L = MenuMainEdit(L);
	
	affiche_labyrinthe_jeu(L);
	while(!jeu_fini(L)){
		fps();
		fill_screen(0x765154);
		directionJoueur = get_arrow();
		L = deplace_joueur(directionJoueur, L);
		affiche_labyrinthe_jeu(L);
	}
	affiche_all();
	wait_escape();
	exit(0);
}
