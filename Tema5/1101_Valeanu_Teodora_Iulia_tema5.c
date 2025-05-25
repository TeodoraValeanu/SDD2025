#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<malloc.h>


typedef struct Conferinta {
    int id;
    char* titlu;
    char* organizator;
    int an;
    float buget;
    char* locatie;
    int durataZile;
} Conferinta;

typedef struct NodAVL {
    Conferinta info;
    struct NodAVL* stanga;
    struct NodAVL* dreapta;
    int inaltime;
} NodAVL;

typedef struct NodGraf {
    Conferinta info;
    struct NodGraf** vecini;
    int nrVecini;
    int indexInVector; 
} NodGraf;

typedef struct NodLista {
    NodGraf* info;
    struct NodLista* urmator;
} NodLista;


int maxInt(int a, int b) {
    if (a > b) return a;
    return b;
}

Conferinta initConferinta(int id, const char* titlu, const char* organizator, int an, float buget, const char* locatie, int durata) {
    Conferinta c;
    c.id = id;

    c.titlu = (char*)malloc(strlen(titlu) + 1);
    strcpy(c.titlu, titlu);

    c.organizator = (char*)malloc(strlen(organizator) + 1);
    strcpy(c.organizator, organizator);

    c.locatie = (char*)malloc(strlen(locatie) + 1);
    strcpy(c.locatie, locatie);

    c.an = an;
    c.buget = buget;
    c.durataZile = durata;

    return c;
}

void dezalocareConferinta(Conferinta* c) {
    if (c->titlu) free(c->titlu);
    if (c->organizator) free(c->organizator);
    if (c->locatie) free(c->locatie);

    c->titlu = NULL;
    c->organizator = NULL;
    c->locatie = NULL;
}

void afisareConferinta(Conferinta c) {
    printf("ID: %d | Titlu: %s | Organizator: %s | An: %d | Buget: %.2f | Locatie: %s | Durata: %d zile\n",
        c.id, c.titlu, c.organizator, c.an, c.buget, c.locatie, c.durataZile);
}

int inaltime(NodAVL* nod) {
    if (!nod) return 0;
    return nod->inaltime;
}

int getBalance(NodAVL* nod) {
    if (!nod) return 0;
    return inaltime(nod->stanga) - inaltime(nod->dreapta);
}

NodAVL* rotatieDreapta(NodAVL* y) {
    NodAVL* x = y->stanga;
    NodAVL* T2 = x->dreapta;

    x->dreapta = y;
    y->stanga = T2;

    y->inaltime = maxInt(inaltime(y->stanga), inaltime(y->dreapta)) + 1;
    x->inaltime = maxInt(inaltime(x->stanga), inaltime(x->dreapta)) + 1;

    return x;
}

NodAVL* rotatieStanga(NodAVL* x) {
    NodAVL* y = x->dreapta;
    NodAVL* T2 = y->stanga;

    y->stanga = x;
    x->dreapta = T2;

    x->inaltime = maxInt(inaltime(x->stanga), inaltime(x->dreapta)) + 1;
    y->inaltime = maxInt(inaltime(y->stanga), inaltime(y->dreapta)) + 1;

    return y;
}


NodAVL* inserareAVL(NodAVL* rad, Conferinta c) {
    if (!rad) {
        NodAVL* nou = (NodAVL*)malloc(sizeof(NodAVL));
        nou->info = c;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        nou->inaltime = 1;
        return nou;
    }

    if (c.id < rad->info.id)
        rad->stanga = inserareAVL(rad->stanga, c);
    else if (c.id > rad->info.id)
        rad->dreapta = inserareAVL(rad->dreapta, c);
    else
        return rad;

    rad->inaltime = maxInt(inaltime(rad->stanga), inaltime(rad->dreapta)) + 1;
    int balance = getBalance(rad);

    
    if (balance > 1 && c.id < rad->stanga->info.id)
        return rotatieDreapta(rad);
    if (balance < -1 && c.id > rad->dreapta->info.id)
        return rotatieStanga(rad);
    if (balance > 1 && c.id > rad->stanga->info.id) {
        rad->stanga = rotatieStanga(rad->stanga);
        return rotatieDreapta(rad);
    }
    if (balance < -1 && c.id < rad->dreapta->info.id) {
        rad->dreapta = rotatieDreapta(rad->dreapta);
        return rotatieStanga(rad);
    }

    return rad;
}

void inordine(NodAVL* rad) {
    if (!rad) return;
    inordine(rad->stanga);
    afisareConferinta(rad->info);
    inordine(rad->dreapta);
}

NodAVL* minimNod(NodAVL* nod) {
    NodAVL* curent = nod;
    while (curent && curent->stanga != NULL)
        curent = curent->stanga;
    return curent;
}

NodAVL* stergereAVL(NodAVL* rad, int id) {
    if (!rad) return rad;

    if (id < rad->info.id) {
        rad->stanga = stergereAVL(rad->stanga, id);
    }
    else if (id > rad->info.id) {
        rad->dreapta = stergereAVL(rad->dreapta, id);
    }
    else {
        if (!rad->stanga || !rad->dreapta) {
            NodAVL* temp = rad->stanga ? rad->stanga : rad->dreapta;
            dezalocareConferinta(&rad->info);
            free(rad);
            return temp;
        }
        else {
            NodAVL* temp = minimNod(rad->dreapta);

            free(rad->info.titlu);
            free(rad->info.organizator);
            free(rad->info.locatie);

            rad->info.titlu = (char*)malloc(strlen(temp->info.titlu) + 1);
            strcpy(rad->info.titlu, temp->info.titlu);

            rad->info.organizator = (char*)malloc(strlen(temp->info.organizator) + 1);
            strcpy(rad->info.organizator, temp->info.organizator);

            rad->info.locatie = (char*)malloc(strlen(temp->info.locatie) + 1);
            strcpy(rad->info.locatie, temp->info.locatie);

            rad->info.id = temp->info.id;
            rad->info.an = temp->info.an;
            rad->info.buget = temp->info.buget;
            rad->info.durataZile = temp->info.durataZile;

            rad->dreapta = stergereAVL(rad->dreapta, temp->info.id);
        }
    }

    if (!rad) return rad;

    rad->inaltime = 1 + maxInt(inaltime(rad->stanga), inaltime(rad->dreapta));
    int balance = getBalance(rad);

    if (balance > 1 && getBalance(rad->stanga) >= 0)
        return rotatieDreapta(rad);
    if (balance > 1 && getBalance(rad->stanga) < 0) {
        rad->stanga = rotatieStanga(rad->stanga);
        return rotatieDreapta(rad);
    }
    if (balance < -1 && getBalance(rad->dreapta) <= 0)
        return rotatieStanga(rad);
    if (balance < -1 && getBalance(rad->dreapta) > 0) {
        rad->dreapta = rotatieDreapta(rad->dreapta);
        return rotatieStanga(rad);
    }

    return rad;
}


NodAVL* conversieGrafLaAVL(NodGraf** graf, int nrNoduri) {
    NodAVL* rad = NULL;
    for (int i = 0; i < nrNoduri; i++) {
        rad = inserareAVL(rad, graf[i]->info);
    }
    return rad;
}

float calculMedieBuget(NodGraf** graf, int nrNoduri) {
    if (nrNoduri == 0) return 0;
    float suma = 0;
    for (int i = 0; i < nrNoduri; i++) {
        suma += graf[i]->info.buget;
    }
    return suma / nrNoduri;
}

NodLista* creareListaDurataMaiMareCa2(NodGraf** graf, int nrNoduri) {
    NodLista* cap = NULL;
    NodLista* ultim = NULL;
    for (int i = 0; i < nrNoduri; i++) {
        if (graf[i]->info.durataZile > 2) {
            NodLista* nod = (NodLista*)malloc(sizeof(NodLista));
            nod->info = graf[i];
            nod->urmator = NULL;
            if (!cap) {
                cap = nod;
                ultim = nod;
            }
            else {
                ultim->urmator = nod;
                ultim = nod;
            }
        }
    }
    return cap;
}

void afisareListaNoduri(NodLista* cap) {
    while (cap) {
        afisareConferinta(cap->info->info);
        cap = cap->urmator;
    }
}

void dezalocareLista(NodLista* cap) {
    while (cap) {
        NodLista* temp = cap;
        cap = cap->urmator;
        free(temp);
    }
}

float calculSumaCriterii(NodAVL* rad, float pragBuget, int anCautat) {
    if (!rad) return 0;
    float suma = 0;
    if (rad->info.buget > pragBuget && rad->info.an == anCautat)
        suma += rad->info.buget;
    suma += calculSumaCriterii(rad->stanga, pragBuget, anCautat);
    suma += calculSumaCriterii(rad->dreapta, pragBuget, anCautat);
    return suma;
}

void citireGraf(const char* numeFisier, NodGraf*** graf, int* nrNoduri) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului.\n");
        return;
    }

    fscanf(f, "%d", nrNoduri);
    *graf = (NodGraf**)malloc((*nrNoduri) * sizeof(NodGraf*));

    for (int i = 0; i < *nrNoduri; i++) {
        int id, an, durata;
        float buget;
        char titlu[100], organizator[100], locatie[100];
        fscanf(f, "%d %s %s %d %f %s %d", &id, titlu, organizator, &an, &buget, locatie, &durata);

        (*graf)[i] = (NodGraf*)malloc(sizeof(NodGraf));
        (*graf)[i]->info = initConferinta(id, titlu, organizator, an, buget, locatie, durata);
        (*graf)[i]->vecini = NULL;
        (*graf)[i]->nrVecini = 0;
    }

    int nrMuchii;
    fscanf(f, "%d", &nrMuchii);
    for (int i = 0; i < nrMuchii; i++) {
        int sursa, destinatie;
        fscanf(f, "%d %d", &sursa, &destinatie);

        (*graf)[sursa]->vecini = (NodGraf**)realloc((*graf)[sursa]->vecini, ((*graf)[sursa]->nrVecini + 1) * sizeof(NodGraf*));
        (*graf)[sursa]->vecini[(*graf)[sursa]->nrVecini++] = (*graf)[destinatie];

        (*graf)[destinatie]->vecini = (NodGraf**)realloc((*graf)[destinatie]->vecini, ((*graf)[destinatie]->nrVecini + 1) * sizeof(NodGraf*));
        (*graf)[destinatie]->vecini[(*graf)[destinatie]->nrVecini++] = (*graf)[sursa];
    }

    fclose(f);
}

void afisareGraf(NodGraf** graf, int nrNoduri) {
    for (int i = 0; i < nrNoduri; i++) {
        afisareConferinta(graf[i]->info);
        printf(" Vecini: ");
        for (int j = 0; j < graf[i]->nrVecini; j++) {
            printf("%d ", graf[i]->vecini[j]->info.id);
        }
        printf("\n");
    }
}

void dezalocareGraf(NodGraf** graf, int nrNoduri) {
    if (!graf) return;
    for (int i = 0; i < nrNoduri; i++) {
        dezalocareConferinta(&graf[i]->info); 
        if (graf[i]->vecini) {
            free(graf[i]->vecini); 
        }
        free(graf[i]); 
    }
    free(graf); 
}

void dezalocareAVL(NodAVL* rad) {
    if (!rad) return;
    dezalocareAVL(rad->stanga);
    dezalocareAVL(rad->dreapta);
    dezalocareConferinta(&rad->info);
    free(rad);
}

int cautaIndexDupaID(NodGraf** graf, int nrNoduri, int id) {
    for (int i = 0; i < nrNoduri; i++) {
        if (graf[i]->info.id == id)
            return i;
    }
    return -1;
}

void parcurgereBFS(NodGraf** graf, int nrNoduri) {
    int* vizitat = (int*)calloc(nrNoduri, sizeof(int));
    int* coada = (int*)malloc(nrNoduri * sizeof(int));
    int inceput = 0, sfarsit = 0;

    coada[sfarsit++] = 0;
    vizitat[0] = 1;

    printf("\nParcurgere BFS graf:\n");
    while (inceput < sfarsit) {
        int curent = coada[inceput++];
        afisareConferinta(graf[curent]->info);

        for (int i = 0; i < graf[curent]->nrVecini; i++) {
            int vecinID = graf[curent]->vecini[i]->info.id;
            int indexVecin = cautaIndexDupaID(graf, nrNoduri, vecinID);
            if (indexVecin != -1 && !vizitat[indexVecin]) {
                vizitat[indexVecin] = 1;
                coada[sfarsit++] = indexVecin;
            }
        }
    }

    free(vizitat);
    free(coada);
}


void main() {
    NodGraf** graf = NULL;
    int nrNoduri = 0;

    citireGraf("date.txt", &graf, &nrNoduri);

    printf("Afisare graf:\n");
    afisareGraf(graf, nrNoduri);

    // 3. Medie buget
    float medieBuget = calculMedieBuget(graf, nrNoduri);
    printf("\n3. Media bugetelor: %.2f\n", medieBuget);

    // 4. Conversie graf -> AVL
    NodAVL* rad = conversieGrafLaAVL(graf, nrNoduri);

    // 5. Lista noduri cu durata > 2
    NodLista* listaDurata = creareListaDurataMaiMareCa2(graf, nrNoduri);
    printf("\n5. Lista noduri cu durata > 2 zile:\n");
    afisareListaNoduri(listaDurata);

    // 6. Calcul suma buget pentru conferinte cu buget > 5000 si an = 2023
    float sumaBuget2023 = calculSumaCriterii(rad, 5000.0f, 2023);
    printf("\n6. Suma bugetelor (buget > 5000 si an = 2023): %.2f\n", sumaBuget2023);

    // 7. Parcurgere BFS graf si inordine AVL
    parcurgereBFS(graf, nrNoduri);
    printf("\nParcurgere inordine AVL:\n");
    inordine(rad);

    // 8. Stergere nod AVL cu un criteriu (ex: id = 3)
    printf("8. Stergere nod AVL cu id = 3\n");
    rad = stergereAVL(rad, 3);

    printf("AVL dupa stergere:\n");
    inordine(rad);


    // 9. Eliberare memorie
 
    dezalocareAVL(rad);
    dezalocareLista(listaDurata);

    //autoevaluare: 10
}
