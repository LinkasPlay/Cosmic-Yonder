#include <stdio.h>
#include <stdlib.h>



int * remplirTab(int * tab1, int taille,int indice){
	int * tabVertical = NULL;
	int * test;
	tabVertical = malloc(taille * sizeof(int));
	if (tabVertical == NULL){
		printf("Erreur malloc");
		return -1;
	}
	tab1[indice] = *tabVertical;
	if(indice =! (taille-1)){
		test = remplirTab(tab1,taille,indice+1);
	}
	else{
		return tab1;
	}
}	

void afficheTab(int ** tab1,int taille){
	int i,j;
	for (i==0,i<=taille,i++){
		if(i==taille){
			printf("/n							");
		}
		else{
			for (j==0,j<taille,j++){
				if(j == 0 || j == taille-1 || i == 0 || j == taille-1){
					printf ("🧱 ");
				}
				else if(tab1[i][j] == 1){
					printf ("🧑‍🚀 ");
				}
				else{
					printf ("   ");
				}
			}
		}
	}
}
			

int creeTab(in * b){
	int tailleTab, i;
	int ** tabHorizontal = NULL;
	do{
		printf("Quelle est la taille du tableau ?/n/n");
		scanf("%d",tailleTab);
		if(tailleTab<1){
			printf("/nValeur invalide/n/n");
		}
	}while(tailleTab<1);
	tabHorizontal = malloc(sizeof(int) * tailleTab);
	if (tabHorizontal == NULL){
		printf("Erreur malloc");
		return -1;
	}
	tabHorizontal = remplirTab(tabHorizontal,tailleTab,0);
	b = tailleTab;
	return tabHorizontal;
}
	
	
	

int main(){
	int ** a;
	int * b;
	a = creeTab(b);
	printf("%d et %d",a,b);
	afficheTab(a,b);
	
}