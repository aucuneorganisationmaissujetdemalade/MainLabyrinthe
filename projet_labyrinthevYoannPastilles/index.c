#include "graphics.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define nbFantomes 1
#define nbPastilles 4

#define NB_CASE_X 50
#define NB_CASE_Y 50

#define TAILLE_ECRAN_X 600
#define TAILLE_ECRAN_Y 600

#define TAILLE_CASE_X (TAILLE_ECRAN_X/NB_CASE_X)
#define TAILLE_CASE_Y (TAILLE_ECRAN_Y/NB_CASE_Y)

typedef struct{ 
	char cases[NB_CASE_X][NB_CASE_Y];
	POINT joueur, sortie;
	POINT pastilles[nbPastilles];
	POINT fantomes[nbFantomes]; 
	int NombreCaseX, NombreCaseY, compteurPastillesCollectees;
	BOOL sortieOuverte;
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


int taille_perso(){
	if(TAILLE_CASE_X<TAILLE_CASE_Y){
		return(TAILLE_CASE_X/5);
	}
	else{
		return(TAILLE_CASE_Y/5);
	}
}

int taille_pastille(){
	if(TAILLE_CASE_X<TAILLE_CASE_Y){
		return(TAILLE_CASE_X/3);
	}
	else{
		return(TAILLE_CASE_Y/3);
	}
}

BOOL clic_edit(POINT P){
	if(P.x > (3*TAILLE_ECRAN_X)/10 && P.x < (7*TAILLE_ECRAN_X)/10 && P.y > (6*TAILLE_ECRAN_Y)/10 && P.y < (8*TAILLE_ECRAN_Y)/10){
		return true;
	}
	return false;
}

BOOL clic_play(POINT P){
	if(P.x > (3*TAILLE_ECRAN_X)/10 && P.x < (7*TAILLE_ECRAN_X)/10 && P.y > (2*TAILLE_ECRAN_Y)/10 && P.y < (4*TAILLE_ECRAN_Y)/10)
	{
		return true;
	}
	return false;
}

BOOL clic_edit_fini(POINT P){
	if(P.x > (3*TAILLE_ECRAN_X)/10 && P.x < (7*TAILLE_ECRAN_X)/10 && P.y > 0 && P.y < TAILLE_CASE_Y ){
		   return true;
	}
	return false;
}

void affiche_menu_principal(){
	POINT bg_edit_button, hd_edit_button, bg_play_button, hd_play_button, bg_edit_text, bg_play_text;
	
	bg_edit_button.x = (3*TAILLE_ECRAN_X)/10;
	bg_edit_button.y = (6*TAILLE_ECRAN_Y)/10;
	
	hd_edit_button.x = (7*TAILLE_ECRAN_X)/10;
	hd_edit_button.y = (8*TAILLE_ECRAN_Y)/10;
	
	draw_fill_rectangle(bg_edit_button, hd_edit_button, bleu);
	
	bg_play_button.x = (3*TAILLE_ECRAN_X)/10;
	bg_play_button.y = (2*TAILLE_ECRAN_Y)/10;
	
	hd_play_button.x = (7*TAILLE_ECRAN_X)/10;
	hd_play_button.y = (4*TAILLE_ECRAN_Y)/10;
	
	draw_fill_rectangle(bg_play_button, hd_play_button, citronvert);
	
	bg_edit_text.x = bg_edit_button.x + (2*TAILLE_ECRAN_X)/30;
	bg_edit_text.y = bg_edit_button.y + (4*TAILLE_ECRAN_Y)/30;
	aff_pol("Ouvrir l'editeur", 20, bg_edit_text,white);
	
	bg_play_text.x = bg_play_button.x + (2*TAILLE_ECRAN_X)/30;
	bg_play_text.y = bg_play_button.y + (4*TAILLE_ECRAN_Y)/30;
	aff_pol("Nouvelle Partie", 20, bg_play_text,white);
	
	affiche_all();
}

void sauvegarde_labyrinthe(LABYRINTHE L){
	int WX,WY;
	char FlNm[100];
	char* extension = ".lab";
	char* FlNmEx;
	
	printf("Nommez votre labyrinthe afin qu'il soit enregistré avec au maximum 100 caractères(si un autre labyrinthe du même nom existe, il sera supprimé)\n Nom de votre labyrinthe :");
	scanf("%s",FlNm);
	FlNmEx = strcat(FlNm,extension);
	printf("%s\n",FlNmEx);
	
	FILE* fichier = NULL;
	fichier = fopen(FlNmEx,"w");
	
	if(fichier != NULL){
		fprintf(fichier,"%d ; %d", L.NombreCaseX, L.NombreCaseY);
		fprintf(fichier,"\n\n");
		
		for(WY = 0; WY < L.NombreCaseY ; WY ++){
			for(WX = 0; WX < L.NombreCaseX; WX ++){
				fprintf(fichier,"%c",L.cases[WX][L.NombreCaseY - WY - 1]);
			}
			fprintf(fichier,"\n");
		}
	}
	else{
		printf("Impossible d'ouvrir ou de créer votre fichier");
	}
	fclose(fichier);
}

LABYRINTHE lecture_fichier(LABYRINTHE L){
	int RX,RY;
	
	FILE* fichier = NULL;
	fichier = fopen("UI.lab","r");
	if(fichier != NULL){
		fscanf(fichier,"%d ; %d\n\n", &L.NombreCaseX, &L.NombreCaseY);
		
		for(RY = 0; RY < L.NombreCaseY ; RY ++){
			for(RX = 0; RX < L.NombreCaseX; RX ++){
				fprintf(fichier,"%c",L.cases[RX][L.NombreCaseY - RY - 1]);
			}
		fscanf(fichier,"\n");
		}
	}
	else{
		printf("Impossible d'ouvrir ou de créer votre fichier");
	}
	fclose(fichier);
	return L;
}

void affiche_bouton_edit_fini(){
	POINT bg_play_button, hd_play_button, bg_play_text;
	bg_play_button.x = (3*TAILLE_ECRAN_X)/10;
	bg_play_button.y = 0;
	
	hd_play_button.x = (7*TAILLE_ECRAN_X)/10;
	hd_play_button.y = TAILLE_CASE_Y;
	
	draw_fill_rectangle(bg_play_button, hd_play_button, citronvert);
	
	bg_play_text.x = bg_play_button.x + (5*TAILLE_ECRAN_Y)/30;
	bg_play_text.y = 12;
	aff_pol("Valider", 10, bg_play_text,black);
	
	affiche_all();
}

LABYRINTHE edit_labyrinthe(LABYRINTHE L, POINT clic){
	clic = pixel2piece(clic);
	BOOL dejaUnePastille;
	int i;
	dejaUnePastille = false;
	for(i=0; i < nbPastilles; i++){
		if(clic.x == pixel2piece(L.pastilles[i]).x && clic.y == pixel2piece(L.pastilles[i]).y)
		{
			dejaUnePastille = true;
		}
	}
	if(clic.x == pixel2piece(L.joueur).x && clic.y == pixel2piece(L.joueur).y){
		printf("\n \n cMORT \n \n");
	}
	else if(dejaUnePastille == true)
	{
		printf("\n \n cMORT \n \n");
	}	
	else if(clic.x >= NB_CASE_X - 1 || clic.y >= NB_CASE_Y - 1 || clic.x < 1 || clic.y < 1){
		printf("\n \n cMORT \n \n");
	}
	else if(L.cases[clic.x][clic.y] == 'm'){
		L.cases[clic.x][clic.y] = 'v';
	}
	else if(L.cases[clic.x][clic.y] == 'v'){
		L.cases[clic.x][clic.y] = 'm';
	}
	return L;
}

BOOL positionPossible(LABYRINTHE L, POINT clic){
	clic = pixel2piece(clic);
	if(clic.x >= NB_CASE_X - 1 || clic.y >= NB_CASE_Y - 1 || clic.x < 1 || clic.y < 1){
		printf("\n \n cMORT \n \n");
		return false;
	}
	return true;
}
LABYRINTHE edit_joueur_position(LABYRINTHE L,POINT clic){
	clic = pixel2piece(clic);
	L.joueur.x = clic.x; 
	L.joueur.y = clic.y;
	L.joueur = piece2pixel(L.joueur);
	return L;
}

LABYRINTHE edit_pastilles_positions(LABYRINTHE L,POINT clic, int numero){
	clic = pixel2piece(clic);
	L.pastilles[numero].x = clic.x; 
	L.pastilles[numero].y = clic.y;
	L.pastilles[numero] = piece2pixel(L.pastilles[numero]);
	return L;
}

LABYRINTHE menu_main_edit(LABYRINTHE L){ 
	int compteurPastilles;
	POINT clic;
	BOOL jouer, editFini, joueurPlaced, pastillesPlaced;
	compteurPastilles = 0;
	pastillesPlaced = false;
	joueurPlaced = false;
	editFini = false;
	jouer = false;
	affiche_menu_principal();
	while(!jouer){
		clic = wait_clic();
		if(clic_edit(clic)){
			printf("\n \n caEdit \n \n");
			while(!pastillesPlaced){
				affiche_labyrinthe_edit(L);
				printf("\n \n Faites un clic la ou vous souhaitez que la pastille apparaisse \n \n");
				clic = wait_clic();	
				if(positionPossible(L, clic)){
					L = edit_pastilles_positions(L, clic, compteurPastilles);
					compteurPastilles++;	
				}
				if(compteurPastilles >= nbPastilles)
				{
					pastillesPlaced = true;
				}
			}
			while(!joueurPlaced){
				affiche_labyrinthe_edit(L);
				printf("\n \n Faites un clic la ou vous souhaitez que le joueur apparaisse \n \n");
				clic = wait_clic();	
				if(positionPossible(L, clic)){
					L = edit_joueur_position(L, clic);	
					joueurPlaced = true;
				}
			}
			printf("\n \n Positionnez les murs \n \n");
			while(!editFini){
				affiche_labyrinthe_edit(L);
				affiche_bouton_edit_fini();
				clic = wait_clic();
				L = edit_labyrinthe(L, clic);
				if(clic_edit_fini(clic)){
					sauvegarde_labyrinthe(L);
					editFini = true;
				}
			}
			jouer = true;	
		} 
		else if(clic_play(clic))
		{
			printf("\n \n caJoue \n \n");
			L = lecture_fichier(L);
			jouer = true;
		}
	}
	return L;
}

void affiche_joueur(LABYRINTHE L){
	int TaillePersonnage;
	COULEUR CouleurPersonnage;
	
	TaillePersonnage = taille_perso();
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
	L.sortieOuverte = false;
	L.sortie.x = 1; 
	L.sortie.y = NB_CASE_Y - 1;
	L.compteurPastillesCollectees = 0;
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
	L.cases[L.sortie.x][L.sortie.y] = 'm';
	L.NombreCaseX = NB_CASE_X;
	L.NombreCaseY = NB_CASE_Y;
	L.joueur.x = TAILLE_ECRAN_X - ((TAILLE_CASE_X*3)/2);
	L.joueur.y = (TAILLE_CASE_Y*3)/2;
	return L;
}

LABYRINTHE ouvrirSortie(LABYRINTHE L)
{
	L.sortieOuverte = true;
	L.cases[L.sortie.x][L.sortie.y] = 'v';
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

void affiche_pastilles_edit(LABYRINTHE L){
	int TaillePastilles, i;
	COULEUR CouleurPastilles;
	
	TaillePastilles = taille_pastille();
	CouleurPastilles = citronvert;
	for(i=0; i<nbPastilles; i++)
	{
		draw_fill_circle(L.pastilles[i],TaillePastilles,CouleurPastilles);
	}
}

void affiche_pastilles_jeu(LABYRINTHE L){
	int TaillePastilles;
	COULEUR CouleurPastilles;
	
	TaillePastilles = taille_pastille();
	CouleurPastilles = citronvert;
	draw_fill_circle(L.pastilles[L.compteurPastillesCollectees],TaillePastilles,CouleurPastilles);
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
	affiche_pastilles_edit(L);
	affiche_joueur(L);
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
	affiche_pastilles_jeu(L);
	if(L.sortieOuverte == true){
		affiche_sortie(L);
	}
	else{
		affiche_mur(L.sortie);
	}
	affiche_joueur(L);
	affiche_all();
}

BOOL deplacement_possible(POINT directionJoueur, LABYRINTHE L){
	POINT CaseTest,Cible;
	int TestMultiDir,TaillePersonnage;
	TaillePersonnage = taille_perso();
	POINT DirectionTest[8] = { {(TaillePersonnage+1),0},{0,(TaillePersonnage+1)},{(-TaillePersonnage-1),0},{0,(-TaillePersonnage-1)},{(TaillePersonnage+1),(TaillePersonnage+1)},{(-TaillePersonnage-1),(TaillePersonnage+1)},{(TaillePersonnage+1),(-TaillePersonnage-1)},{(-TaillePersonnage-1),(-TaillePersonnage-1)} };
	
	for(TestMultiDir = 0; TestMultiDir < 8;TestMultiDir ++){
		Cible = DirectionTest[TestMultiDir];
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

void fps(){
	int Delay;
	Delay = (TAILLE_ECRAN_X+TAILLE_ECRAN_Y)/200;
	SDL_Delay(Delay);
}
BOOL pastillesCollectees(LABYRINTHE L){
	if(L.compteurPastillesCollectees == nbPastilles){
		return true;
	}
	return false;
}

BOOL pastilleCollectee(LABYRINTHE L){
	if(pixel2piece(L.joueur).x == pixel2piece(L.pastilles[L.compteurPastillesCollectees]).x && 
	pixel2piece(L.joueur).y == pixel2piece(L.pastilles[L.compteurPastillesCollectees]).y){
		return true;
	}
	return false;
}

int main(){
	LABYRINTHE L; 
	POINT directionJoueur;
	
	init_graphics(TAILLE_ECRAN_X,TAILLE_ECRAN_Y);
	fill_screen(0x765154);
	affiche_auto_off();
	
	L = init_labyrinthe(L);
	L = menu_main_edit(L);
	
	affiche_labyrinthe_jeu(L);
	while(!jeu_fini(L)){
		fps();
		fill_screen(0x765154);
		directionJoueur = get_arrow();
		L = deplace_joueur(directionJoueur, L);
		if(pastilleCollectee(L)){
			L.compteurPastillesCollectees++;
		}
		if(pastillesCollectees(L)){
			L = ouvrirSortie(L);
		}
		affiche_labyrinthe_jeu(L);
	}
	affiche_all();
	wait_escape();
	exit(0);
}
