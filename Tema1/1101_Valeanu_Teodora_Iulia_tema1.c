#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct Item {
	int cod_unic;
	char* denumire;
	float* pret;
	int cantitate;
	double* rating;
	char categorie[30];
	char* descriere;
} Item;

typedef struct Nod 
{
	Item info;
	struct Nod* next;

} Nod;

void afisareItem(Item item)
{
	printf("\nItemul cu codul unic %d se numeste %s, are categoria %s si costa %.2f lei.\n",
			item.cod_unic, item.denumire, item.categorie, *item.pret);
	printf("Cantitatea disponibila este: %d\n", item.cantitate);
	printf("Rating-ul este: %.2f\n", *item.rating);
	printf("Descriere: %s\n", item.descriere);
	
}

void inserareLaSfarsit(Nod** cap, Item item)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = item;
	nou->next = NULL;

	if ((*cap) == NULL)
	{
		*cap = nou;
	}
	else
	{
		Nod* temp = *cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void dezalocareLista(Nod** cap) {
	while (*cap) {
		free((*cap)->info.denumire);
		free((*cap)->info.pret);
		free((*cap)->info.rating);
		free((*cap)->info.descriere);
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
	}
}

void afisareLista(Nod* cap) {
	Nod* temp = cap;
	while (temp != NULL) {
		afisareItem(temp->info);
		temp = temp->next;
	}
}

void stergerePrimulElement(Nod** cap) {
	if (*cap == NULL) {
		printf("Nu se poate sterge primul element.\n");
		return;
	}

	Nod* temp = *cap;
	*cap = (*cap)->next; 
	free(temp->info.denumire);
	free(temp->info.pret);
	free(temp->info.rating);
	free(temp->info.descriere);
	free(temp); 
}

void sortareLista(Nod* cap) {
	if (cap == NULL) return;  

	Nod* i = cap;
	while (i != NULL) {
		Nod* min = i;
		Nod* j = i->next;
		while (j != NULL) {
			if (j->info.cod_unic < min->info.cod_unic) {
				min = j;
			}
			j = j->next;
		}

		if (min != i) {
			Item temp = i->info;
			i->info = min->info;
			min->info = temp;
		}
		i = i->next;
	}
}

void stergereElement(Nod** cap, int id) {
	if (*cap == NULL) {
		printf("Lista este goala.\n");
		return;
	}

	if ((*cap)->info.cod_unic == id) {
		Nod* temp = *cap;
		*cap = (*cap)->next;  
		free(temp->info.denumire);
		free(temp->info.pret);
		free(temp->info.rating);
		free(temp->info.descriere);
		free(temp);  
		printf("Elementul cu codul unic %d a fost sters.\n", id);
		return;
	}

	Nod* temp = *cap;
	while (temp->next != NULL && temp->next->info.cod_unic != id) {
		temp = temp->next;  
	}

	if (temp->next != NULL && temp->next->info.cod_unic == id) {
		Nod* deSters = temp->next;
		temp->next = temp->next->next;  
		free(deSters->info.denumire);
		free(deSters->info.pret);
		free(deSters->info.rating);
		free(deSters->info.descriere);
		free(deSters);  
		printf("Elementul cu codul unic %d a fost sters.\n", id);
	}
	else {
		printf("Elementul cu codul unic %d nu a fost gasit.\n", id);
	}
}

void citireVectorDeVectori(FILE* file, Item*** vectorDeVectori, int** nrElemente, int* nrVectori) {
	fscanf(file, "%d", nrVectori);

	*vectorDeVectori = (Item**)malloc(*nrVectori * sizeof(Item*));
	*nrElemente = (int*)malloc(*nrVectori * sizeof(int));

	for (int i = 0; i < *nrVectori; i++) {
		fscanf(file, "%d", &(*nrElemente)[i]); 

		(*vectorDeVectori)[i] = (Item*)malloc((*nrElemente)[i] * sizeof(Item));

		for (int j = 0; j < (*nrElemente)[i]; j++) {
			Item* item = &(*vectorDeVectori)[i][j];

			fscanf(file, "%d", &item->cod_unic);

			item->denumire = (char*)malloc(100 * sizeof(char));
			fscanf(file, "%s", item->denumire);

			item->pret = (float*)malloc(sizeof(float));
			fscanf(file, "%f", item->pret);

			fscanf(file, "%s", item->categorie);

			fscanf(file, "%d", &item->cantitate);

			item->rating = (double*)malloc(sizeof(double));
			fscanf(file, "%lf", item->rating);

			item->descriere = (char*)malloc(200 * sizeof(char));
			fscanf(file, " %[^\n]s", item->descriere);
		}
	}
}



void afisareVectorDeVectori(Item** vectorDeVectori, int* nrElemente, int nrVectori) {
	for (int i = 0; i < nrVectori; i++) {
		printf("\nGrupul %d:\n", i + 1);
		for (int j = 0; j < nrElemente[i]; j++) {  
			Item item = vectorDeVectori[i][j];
			printf("Cod Unic: %d, Denumire: %s, Pret: %.2f, Categorie: %s, Cantitate: %d, Rating: %.2f, Descriere: %s\n",
				item.cod_unic, item.denumire, *item.pret, item.categorie, item.cantitate, *item.rating, item.descriere);
		}
	}
}


void dezalocareVectorDeVectori(Item*** vectorDeVectori, int** nrElemente, int nrVectori) {
	for (int i = 0; i < nrVectori; i++) {
		for (int j = 0; j < (*nrElemente)[i]; j++) {
			Item* item = &(*vectorDeVectori)[i][j];
			free(item->denumire);
			free(item->pret);
			free(item->rating);
			free(item->descriere);
		}
		free((*vectorDeVectori)[i]);
	}
	free(*vectorDeVectori);
	free(*nrElemente);
}



void main() {
	Nod* cap = NULL;

	Item item1;
	item1.cod_unic = 1;
	item1.denumire = (char*)malloc(20 * sizeof(char));
	strcpy(item1.denumire, "Item 1");
	item1.pret = (float*)malloc(sizeof(float));
	*item1.pret = 100.5f;
	item1.cantitate = 10;
	item1.rating = (double*)malloc(sizeof(double));
	*item1.rating = 4.5;
	strcpy(item1.categorie, "Electronice");
	item1.descriere = (char*)malloc(50 * sizeof(char));
	strcpy(item1.descriere, "Descriere pentru item 1.");

	Item item2;
	item2.cod_unic = 2;
	item2.denumire = (char*)malloc(20 * sizeof(char));
	strcpy(item2.denumire, "Item 2");
	item2.pret = (float*)malloc(sizeof(float));
	*item2.pret = 200.0f;
	item2.cantitate = 5;
	item2.rating = (double*)malloc(sizeof(double));
	*item2.rating = 4.0;
	strcpy(item2.categorie, "Cosmetice");
	item2.descriere = (char*)malloc(50 * sizeof(char));
	strcpy(item2.descriere, "Descriere pentru item 2.");

	Item item3;
	item3.cod_unic = 3;
	item3.denumire = (char*)malloc(20 * sizeof(char));
	strcpy(item3.denumire, "Item 3");
	item3.pret = (float*)malloc(sizeof(float));
	*item3.pret = 150.0f;
	item3.cantitate = 8;
	item3.rating = (double*)malloc(sizeof(double));
	*item3.rating = 4.7;
	strcpy(item3.categorie, "Electrocasnice");
	item3.descriere = (char*)malloc(50 * sizeof(char));
	strcpy(item3.descriere, "Descriere pentru item 3.");

	inserareLaSfarsit(&cap, item1);
	inserareLaSfarsit(&cap, item3);
	inserareLaSfarsit(&cap, item2);

	printf("\nInainte de stergere:\n");
	afisareLista(cap);

	stergerePrimulElement(&cap);

	printf("\nDupa stergerea primului element:\n");
	afisareLista(cap);

	sortareLista(cap);

	printf("\nDupa sortare:\n");
	afisareLista(cap);

	stergereElement(&cap, 2);

	printf("\nAfisare lista dupa stergere:\n");
	afisareLista(cap);

	dezalocareLista(&cap);

	FILE* file = fopen("Items.txt", "r");
	if (!file) {
		printf("Eroare la deschidere.\n");
		return 1;
	}

	int* nrElemente = NULL;
	Item** vectorDeVectori = NULL;
	int nrVectori = 0;

	citireVectorDeVectori(file, &vectorDeVectori, &nrElemente, &nrVectori);
	fclose(file);

	printf("\nAfisare vector de vectori:\n");
	afisareVectorDeVectori(vectorDeVectori, nrElemente, nrVectori);

	dezalocareVectorDeVectori(&vectorDeVectori, &nrElemente, nrVectori);

}
//autoevaluare: 10
