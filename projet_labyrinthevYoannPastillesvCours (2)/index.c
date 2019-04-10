#include "graphics.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define TAILLE_ECRAN_X 605
#define TAILLE_ECRAN_Y 600

typedef struct{ 
	int nombreCaseX, nombreCaseY;
	char cases[100][100];
	
	POINT joueur, sortie;
	
	int nombrePastilles;
	POINT pastilles[100];
	int compteurPastillesCollectees;
	
	int nombreFantomes;
	POINT fantomes[50]; 
	POINT directionFantomes[50];
	
	BOOL sortieOuverte, fini, win;
} LABYRINTHE;

POINT piece2pixel(POINT piece, LABYRINTHE L){
	piece.x = (TAILLE_ECRAN_X/L.nombreCaseX)*piece.x;
	piece.y = (TAILLE_ECRAN_Y/L.nombreCaseY)*piece.y;
	return piece;
}

POINT pixel2piece(POINT pixel, LABYRINTHE L){
	pixel.x = pixel.x/(TAILLE_ECRAN_X/L.nombreCaseX);
	pixel.y = pixel.y/(TAILLE_ECRAN_Y/L.nombreCaseY);
	return pixel;
}


int taille_perso(LABYRINTHE L){
	if((TAILLE_ECRAN_X/L.nombreCaseX)<(TAILLE_ECRAN_Y/L.nombreCaseY)){
		return((TAILLE_ECRAN_X/L.nombreCaseX)/5);
	}
	else{
		return((TAILLE_ECRAN_Y/L.nombreCaseY)/5);
	}
}

int taille_pastille(LABYRINTHE L){
	if(L.nombreCaseX>L.nombreCaseY){
		return(((TAILLE_ECRAN_X/L.nombreCaseX)/3));
	}
	else{
		return(((TAILLE_ECRAN_Y/L.nombreCaseY)/3));
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

BOOL clic_edit_fini(POINT P, LABYRINTHE L){
	if(P.x > (3*TAILLE_ECRAN_X)/10 && P.x < (7*TAILLE_ECRAN_X)/10 && P.y > 0 && P.y < (TAILLE_ECRAN_Y/L.nombreCaseY) ){
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

void affiche_victoire(){
	POINT bg_win_text;
	bg_win_text.x = (2*TAILLE_ECRAN_X)/30;
	bg_win_text.y = (4*TAILLE_ECRAN_Y)/30;
	aff_pol("BRAVO SUPER BIEN TU AS REUSSI", 20, bg_win_text,white);
	affiche_all();
}

void affiche_defaite(){
	POINT bg_win_text;
	bg_win_text.x = (2*TAILLE_ECRAN_X)/30;
	bg_win_text.y = (4*TAILLE_ECRAN_Y)/30;
	aff_pol("perdu", 20, bg_win_text,white);
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
		fprintf(fichier,"%d ; %d     %d ; %d\n%d\n", L.nombreCaseX, L.nombreCaseY,L.joueur.x, L.joueur.y, L.nombrePastilles);
		int WPast;
		for(WPast = 0; WPast < L.nombrePastilles; WPast++){
			fprintf(fichier,"%d ; %d\n",L.pastilles[WPast].x,L.pastilles[WPast].y);
		}
		for(WY = 0; WY < L.nombreCaseY ; WY ++){
			for(WX = 0; WX < L.nombreCaseX; WX ++){
				fprintf(fichier,"%c",L.cases[WX][L.nombreCaseY - WY - 1]);
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
		fscanf(fichier,"%d ; %d     %d ; %d\n%d\n", &L.nombreCaseX, &L.nombreCaseY, &L.joueur.x, &L.joueur.y, &L.nombrePastilles);
		int RPast;
		for(RPast = 0; RPast < L.nombrePastilles; RPast++){
			fscanf(fichier,"%d ; %d\n",&L.pastilles[RPast].x, &L.pastilles[RPast].y);
		}
		for(RY = 0; RY < L.nombreCaseY ; RY ++){
			for(RX = 0; RX < L.nombreCaseX; RX ++){
				fscanf(fichier,"%c",&L.cases[RX][L.nombreCaseY - RY - 1]);
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

void affiche_bouton_edit_fini(LABYRINTHE L){
	POINT bg_play_button, hd_play_button, bg_play_text;
	bg_play_button.x = (3*TAILLE_ECRAN_X)/10;
	bg_play_button.y = 0;
	
	hd_play_button.x = (7*TAILLE_ECRAN_X)/10;
	hd_play_button.y = (TAILLE_ECRAN_Y/L.nombreCaseY);
	
	draw_fill_rectangle(bg_play_button, hd_play_button, citronvert);
	
	bg_play_text.x = bg_play_button.x + (5*TAILLE_ECRAN_Y)/30;
	bg_play_text.y = 12;
	aff_pol("Valider", 10, bg_play_text,black);
	
	affiche_all();
}

LABYRINTHE edit_labyrinthe(LABYRINTHE L, POINT clic){
	clic = pixel2piece(clic, L);
	BOOL dejaUnePastille;
	int i;
	dejaUnePastille = false;
	for(i=0; i < L.nombrePastilles; i++){
		if(clic.x == pixel2piece(L.pastilles[i], L).x && clic.y == pixel2piece(L.pastilles[i], L).y)
		{
			dejaUnePastille = true;
		}
	}
	if(clic.x == pixel2piece(L.joueur, L).x && clic.y == pixel2piece(L.joueur, L).y){
		printf("\n \n cMORT \n \n");
	}
	else if(dejaUnePastille == true)
	{
		printf("\n \n cMORT \n \n");
	}	
	else if(clic.x >= L.nombreCaseX - 1 || clic.y >= L.nombreCaseY - 1 || clic.x < 1 || clic.y < 1){
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
	clic = pixel2piece(clic, L);
	if(clic.x >= L.nombreCaseX - 1 || clic.y >= L.nombreCaseY - 1 || clic.x < 1 || clic.y < 1){
		printf("\n \n cMORT \n \n");
		return false;
	}
	return true;
}
LABYRINTHE edit_joueur_position(LABYRINTHE L,POINT clic){
	clic = pixel2piece(clic, L);
	L.joueur.x = clic.x; 
	L.joueur.y = clic.y;
	L.joueur = piece2pixel(L.joueur, L);
	L.joueur.x = L.joueur.x + ( (TAILLE_ECRAN_X/L.nombreCaseX)/2);
	L.joueur.y = L.joueur.y + ( (TAILLE_ECRAN_Y/L.nombreCaseY)/2);
	return L;
}

LABYRINTHE edit_pastilles_positions(LABYRINTHE L,POINT clic, int numero){
	clic = pixel2piece(clic, L);
	L.pastilles[numero].x = clic.x; 
	L.pastilles[numero].y = clic.y;
	L.pastilles[numero] = piece2pixel(L.pastilles[numero], L);
	return L;
}

LABYRINTHE edit_fantome_directions(LABYRINTHE L){
	int compteurFantomes;
	
	for(compteurFantomes=0; compteurFantomes < L.nombreFantomes;compteurFantomes++){		
		L.directionFantomes[compteurFantomes].x = 0;
		L.directionFantomes[compteurFantomes].y = 0;
	}
	
	return L;
}

LABYRINTHE edit_fantome_position(LABYRINTHE L){
	int compteurFantomes, aleat_int_x, aleat_int_y;
	POINT CaseTest;
	BOOL pasDObstacle;
	
	for(compteurFantomes=0; compteurFantomes<L.nombreFantomes;compteurFantomes++){		
		pasDObstacle = false;
		
		while(!pasDObstacle){
			aleat_int_x = 2 + alea_int(L.nombreCaseX - 4);
			aleat_int_y = 2 + alea_int(L.nombreCaseY - 4);
			CaseTest.x = aleat_int_x;
			CaseTest.y = aleat_int_y;
			if(L.cases[aleat_int_x][aleat_int_y] == 'v' && (CaseTest.x != pixel2piece(L.joueur, L).x && CaseTest.y != pixel2piece(L.joueur, L).y)){
				pasDObstacle = true;
			}
		}		
		L.fantomes[compteurFantomes].x = aleat_int_x;
		L.fantomes[compteurFantomes].y = aleat_int_y;
		L.fantomes[compteurFantomes] = piece2pixel(L.fantomes[compteurFantomes], L);
		L.fantomes[compteurFantomes].x = L.fantomes[compteurFantomes].x + ( (TAILLE_ECRAN_X/L.nombreCaseX)/2);
		L.fantomes[compteurFantomes].y = L.fantomes[compteurFantomes].y + ( (TAILLE_ECRAN_Y/L.nombreCaseY)/2);
	}
	return L;
}

LABYRINTHE menu_main_edit(LABYRINTHE L){ 
	int compteurPastilles, i;
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
			for(i = 0; i < L.nombrePastilles;i++){
				L.pastilles[i].x = -10;
				L.pastilles[i].y = -10;
				L.pastilles[i] = piece2pixel(L.pastilles[i], L);
			}
			printf("\n \n caEdit \n \n");
			while(!pastillesPlaced){
				affiche_labyrinthe_edit(L);
				printf("\n \n Faites un clic la ou vous souhaitez que la pastille apparaisse \n \n");
				clic = wait_clic();
				if(positionPossible(L, clic)){
					L = edit_pastilles_positions(L, clic, compteurPastilles);
					compteurPastilles++;	
				}
				if(compteurPastilles >= L.nombrePastilles)
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
				affiche_bouton_edit_fini(L);
				clic = wait_clic();
				L = edit_labyrinthe(L, clic);
				if(clic_edit_fini(clic, L)){
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
	
	TaillePersonnage = taille_perso(L);
	CouleurPersonnage = 0xFFF400;
	draw_fill_circle(L.joueur,TaillePersonnage,CouleurPersonnage);
	POINT HitBox, HitBox2;
	HitBox.x = L.joueur.x - TaillePersonnage - 1; 
	HitBox.y = L.joueur.y - TaillePersonnage - 1; 
	
	HitBox2.x = L.joueur.x + TaillePersonnage + 1; 
	HitBox2.y = L.joueur.y + TaillePersonnage + 1; 
	
	draw_rectangle(HitBox,HitBox2,blanc);
}

void affiche_fantomes(LABYRINTHE L){
	int TailleFantome, i;
	COULEUR CouleurFantome;
	POINT HitBox, HitBox2;
	
	TailleFantome = taille_perso(L);
	CouleurFantome = blue;
	
	for(i=0;i<L.nombreFantomes;i++){
		draw_fill_circle(L.fantomes[i],TailleFantome,CouleurFantome);
		
		HitBox.x = L.fantomes[i].x - TailleFantome - 1; 
		HitBox.y = L.fantomes[i].y - TailleFantome - 1; 
		
		HitBox2.x = L.fantomes[i].x + TailleFantome + 1; 
		HitBox2.y = L.fantomes[i].y + TailleFantome + 1; 
		
		draw_rectangle(HitBox,HitBox2,blanc);
	}
}

LABYRINTHE init_labyrinthe(LABYRINTHE L){
	int InitX, InitY, aleat_int_x, aleat_int_y, i;
	BOOL pasDeMur;
	L.fini = false;
	L.win = false;
	aleat_int_x = 0;
	aleat_int_y = 0;
	
	L.nombreCaseX = 50;
	L.nombreCaseY = 50;
	
	L.nombreFantomes = 50;
	L.nombrePastilles = 10;
	
	L.sortieOuverte = false;
	
	L.sortie.x = 1; 
	L.sortie.y = L.nombreCaseY - 1;
	
	L.compteurPastillesCollectees = 0;
	for(InitX = 0; InitX<L.nombreCaseX; InitX++){
		for(InitY = 0; InitY<L.nombreCaseY; InitY++){
			if(InitX == 0 || InitX == L.nombreCaseX - 1 || InitY == 0 || InitY == L.nombreCaseY - 1){
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
	for(i=0; i<L.nombrePastilles; i++){
		pasDeMur = false;
		while(!pasDeMur){
			aleat_int_x = alea_int(L.nombreCaseX - 1);
			aleat_int_y = alea_int(L.nombreCaseY - 1);
			if(L.cases[aleat_int_x][aleat_int_y] == 'v'){
				pasDeMur = true;
			}
		}
		L.pastilles[i]. x = aleat_int_x;
		L.pastilles[i]. y = aleat_int_y;
		L.pastilles[i] = piece2pixel(L.pastilles[i], L);
	}
	L.cases[L.sortie.x][L.sortie.y] = 'm';
	
	L.joueur.x = TAILLE_ECRAN_X - (((TAILLE_ECRAN_X/L.nombreCaseX)*3)/2);
	L.joueur.y = ((TAILLE_ECRAN_Y/L.nombreCaseY)*3)/2;
	
	L.pastilles[L.nombrePastilles].x = 10000;
	L.pastilles[L.nombrePastilles].y = 10000;
	
	return L;
}

LABYRINTHE ouvrirSortie(LABYRINTHE L)
{
	L.sortieOuverte = true;
	L.cases[L.sortie.x][L.sortie.y] = 'v';
	return L;
}

void affiche_mur(POINT M, LABYRINTHE L){
	POINT Mur1,Mur2;
	COULEUR CouleurMur;
	CouleurMur = 0x3F5151;
	
	Mur1 = piece2pixel(M, L);
	Mur2.x = Mur1.x + (TAILLE_ECRAN_X/L.nombreCaseX);
	Mur2.y = Mur1.y + (TAILLE_ECRAN_Y/L.nombreCaseY);
	draw_fill_rectangle(Mur1,Mur2,CouleurMur);
}

void affiche_sortie(LABYRINTHE L){
	POINT Sortie1,Sortie2;
	COULEUR CouleurSortie;
	CouleurSortie = 0x834A20;
	
	Sortie1 = piece2pixel(L.sortie, L);
	Sortie1.x = Sortie1.x + 1;
	Sortie2.x = Sortie1.x + (TAILLE_ECRAN_X/L.nombreCaseX);   Sortie2.y = Sortie1.y + (TAILLE_ECRAN_Y/L.nombreCaseY) - 1;
	
	draw_fill_rectangle(Sortie1,Sortie2,CouleurSortie);
}

void affiche_pastilles_edit(LABYRINTHE L){
	int TaillePastilles, i;
	POINT Pastilles;
	COULEUR CouleurPastilles;
	
	TaillePastilles = taille_pastille(L);
	CouleurPastilles = citronvert;
	
	for(i=0; i<L.nombrePastilles; i++){
		Pastilles.x = L.pastilles[i].x + ((TAILLE_ECRAN_X / L.nombreCaseX)/2);
		Pastilles.y = L.pastilles[i].y + ((TAILLE_ECRAN_Y / L.nombreCaseY)/2);
		draw_fill_circle(Pastilles,TaillePastilles,CouleurPastilles);
	}
}

void affiche_pastilles_jeu(LABYRINTHE L){
	int TaillePastilles;
	POINT Pastille;
	
	COULEUR CouleurPastilles;
	
	TaillePastilles = taille_pastille(L);
	CouleurPastilles = citronvert;
	
	Pastille.x = L.pastilles[L.compteurPastillesCollectees].x + ( (TAILLE_ECRAN_X/L.nombreCaseX)/2);
	Pastille.y = L.pastilles[L.compteurPastillesCollectees].y + ( (TAILLE_ECRAN_Y/L.nombreCaseY)/2);
	
	draw_fill_circle(Pastille,TaillePastilles,CouleurPastilles);
}

void affiche_labyrinthe_edit(LABYRINTHE L){
	fill_screen(0x765154);
	int TestX, TestY;
	for(TestX = 0; TestX<L.nombreCaseX; TestX++){
		for(TestY = 0; TestY<L.nombreCaseY; TestY++){
			if(L.cases[TestX][TestY] == 'm'){
				POINT Mur = {TestX,TestY};
				affiche_mur(Mur, L);
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
	for(TestX = 0; TestX<L.nombreCaseX; TestX++){
		for(TestY = 0; TestY<L.nombreCaseY; TestY++){
			if(L.cases[TestX][TestY] == 'm'){
				POINT Mur = {TestX,TestY};
				affiche_mur(Mur, L);
			}
		}
	}
	affiche_pastilles_jeu(L);
	affiche_fantomes(L);
	if(L.sortieOuverte == true){
		affiche_sortie(L);
	}
	else{
		affiche_mur(L.sortie, L);
	}
	affiche_joueur(L);
	affiche_all();
}

BOOL deplacement_possible(POINT directionJoueur, LABYRINTHE L){
	POINT CaseTest,Cible;
	int TestMultiDir,TaillePersonnage;
	TaillePersonnage = taille_perso(L);
	POINT DirectionTest[8] = { {(TaillePersonnage+1),0},{0,(TaillePersonnage+1)},{(-TaillePersonnage-1),0},{0,(-TaillePersonnage-1)},{(TaillePersonnage+1),(TaillePersonnage+1)},{(-TaillePersonnage-1),(TaillePersonnage+1)},{(TaillePersonnage+1),(-TaillePersonnage-1)},{(-TaillePersonnage-1),(-TaillePersonnage-1)} };
	
	for(TestMultiDir = 0; TestMultiDir < 8;TestMultiDir ++){
		Cible = DirectionTest[TestMultiDir];
		CaseTest.x = directionJoueur.x + L.joueur.x + Cible.x;			
		CaseTest.y = directionJoueur.y + L.joueur.y + Cible.y;
		CaseTest = pixel2piece(CaseTest, L);
		if(L.cases[CaseTest.x][CaseTest.y] == 'm'){
			return false;
		}
	}
	return true;
}

BOOL deplacement_possible_fantome(POINT directionJoueur, POINT fantome, LABYRINTHE L, int fantomeNumero){
	POINT CaseTest,Cible;
	int TestMultiDir,TaillePersonnage;
	TaillePersonnage = taille_perso(L);
	POINT DirectionTest[8] = { {(TaillePersonnage+1),0},{0,(TaillePersonnage+1)},{(-TaillePersonnage-1),0},{0,(-TaillePersonnage-1)},{(TaillePersonnage+1),(TaillePersonnage+1)},{(-TaillePersonnage-1),(TaillePersonnage+1)},{(TaillePersonnage+1),(-TaillePersonnage-1)},{(-TaillePersonnage-1),(-TaillePersonnage-1)} };
	
	for(TestMultiDir = 0; TestMultiDir < 8;TestMultiDir ++){
		Cible = DirectionTest[TestMultiDir];
		CaseTest.x = directionJoueur.x + fantome.x + Cible.x;			
		CaseTest.y = directionJoueur.y + fantome.y + Cible.y;
		CaseTest = pixel2piece(CaseTest, L);
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

LABYRINTHE deplace_fantomes(LABYRINTHE L){
	int i;
	BOOL deplaced;
	POINT DirectionTest[4] = { {1,0},{0,1},{-1,0},{0,-1} };
	for(i=0; i<L.nombreFantomes;i++){
		deplaced = false;
		while(!deplaced){
			if(L.directionFantomes[i].x == 0 && L.directionFantomes[i].y == 0){
				L.directionFantomes[i] = DirectionTest[alea_int(4)];
			}
			else if(deplacement_possible_fantome(L.directionFantomes[i], L.fantomes[i], L, i)){
				L.fantomes[i].x = L.fantomes[i].x + L.directionFantomes[i].x;
				L.fantomes[i].y = L.fantomes[i].y + L.directionFantomes[i].y;
				deplaced = true;
			}
			else{
				L.directionFantomes[i] = DirectionTest[alea_int(4)];
			}
		}
	}
	return L;	
}

LABYRINTHE jeu_fini(LABYRINTHE L){
	int i, j, intervalleXPlus, intervalleXMoins, intervalleYPlus, intervalleYMoins, TaillePerso;
	TaillePerso = taille_perso(L);
	
	
	POINT DirectionTest[8] = { {(TaillePerso+1),0},{0,(TaillePerso+1)},{(-TaillePerso-1),0},{0,(-TaillePerso-1)},{(TaillePerso+1),(TaillePerso+1)},{(-TaillePerso-1),(TaillePerso+1)},{(TaillePerso+1),(-TaillePerso-1)},{(-TaillePerso-1),(-TaillePerso-1)} };
	
	if(pixel2piece(L.joueur, L).x == L.sortie.x &&  pixel2piece(L.joueur, L).y == L.sortie.y){
		L.fini = true;
		L.win = true;
		return L;
	}
	else{
		for(i=0; i < L.nombreFantomes; i++){
			for(j=0; j < 8; j++){
				intervalleXPlus = L.fantomes[i].x + TaillePerso;
				intervalleXMoins = L.fantomes[i].x - TaillePerso;
				intervalleYPlus = L.fantomes[i].y + TaillePerso;
				intervalleYMoins = L.fantomes[i].y - TaillePerso;
				if(L.joueur.x + DirectionTest[j].x >= intervalleXMoins && L.joueur.x + DirectionTest[j].x <= intervalleXPlus && L.joueur.y + DirectionTest[j].y >= intervalleYMoins && L.joueur.y + DirectionTest[j].y <= intervalleYPlus){
					L.fini = true;
					L.win = false;
					return L;
				}
				else{
					L.fini = false;
					L.win = false;
				}
			}
		}
	}
	return L;
}


void fps(){
	int Delay;
	Delay = (TAILLE_ECRAN_X+TAILLE_ECRAN_Y)/200;
	SDL_Delay(Delay);
}

BOOL toutesPastillesCollectees(LABYRINTHE L){
	if(L.compteurPastillesCollectees == L.nombrePastilles){
		return true;
	}
	return false;
}

BOOL pastilleCollectee(LABYRINTHE L){
	int j, intervalleXPlus, intervalleXMoins, intervalleYPlus, intervalleYMoins, TaillePerso;
	
	TaillePerso = taille_perso(L);
	
	POINT DirectionTest[8] = { {(TaillePerso+1),0},{0,(TaillePerso+1)},{(-TaillePerso-1),0},{0,(-TaillePerso-1)},{(TaillePerso+1),(TaillePerso+1)},{(-TaillePerso-1),(TaillePerso+1)},{(TaillePerso+1),(-TaillePerso-1)},{(-TaillePerso-1),(-TaillePerso-1)} };

	for(j=0; j < 8; j++){
		intervalleXPlus = L.pastilles[L.compteurPastillesCollectees].x + TaillePerso + ( (TAILLE_ECRAN_X/L.nombreCaseX)/2);
		intervalleXMoins = L.pastilles[L.compteurPastillesCollectees].x - TaillePerso + ( (TAILLE_ECRAN_X/L.nombreCaseX)/2);
		intervalleYPlus = L.pastilles[L.compteurPastillesCollectees].y + TaillePerso + ( (TAILLE_ECRAN_Y/L.nombreCaseY)/2);
		intervalleYMoins = L.pastilles[L.compteurPastillesCollectees].y - TaillePerso + ( (TAILLE_ECRAN_Y/L.nombreCaseY)/2);
		if(L.joueur.x + DirectionTest[j].x >= intervalleXMoins && L.joueur.x + DirectionTest[j].x <= intervalleXPlus && L.joueur.y + DirectionTest[j].y >= intervalleYMoins && L.joueur.y + DirectionTest[j].y <= intervalleYPlus){
			return true;
		}
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
	L = edit_fantome_position(L);
	L = edit_fantome_directions(L);
	
	affiche_labyrinthe_jeu(L);
	while(!L.fini){
		fps();
		fill_screen(0x765154);
		directionJoueur = get_arrow();
		L = deplace_joueur(directionJoueur, L);
		L = deplace_fantomes(L);
		if(pastilleCollectee(L)){
			L.compteurPastillesCollectees++;
		}
		if(toutesPastillesCollectees(L)){
			L = ouvrirSortie(L);
		}
		affiche_labyrinthe_jeu(L);
		L = jeu_fini(L);
	}
	if(L.win){
		affiche_victoire();
	}
	else{
		affiche_defaite();
	}
	affiche_all();
	wait_escape();
	exit(0);
}
