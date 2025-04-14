#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Vehicul{
int id;
float capacitate;
char* sofer;            
char* marca;            
int anFabricatie;
double* consum;         
long timpFunctionare;
} Vehicul;

typedef struct Nod {
	Vehicul info;
	struct Nod* next;
} Nod;

typedef struct Hashtable
{
	int dimensiune;
	Nod** vector;
} Hashtable;

Vehicul creareVehicul(int id, float capacitate, const char* sofer, const char* marca, int an, double consum, long timp) {
    Vehicul v;
    v.id = id;
    v.capacitate = capacitate;

    v.sofer = (char*)malloc(strlen(sofer) + 1);
    strcpy(v.sofer, sofer);

    v.marca = (char*)malloc(strlen(marca) + 1);
    strcpy(v.marca, marca);

    v.anFabricatie = an;

    v.consum = (double*)malloc(sizeof(double));
    *(v.consum) = consum;

    v.timpFunctionare = timp;

    return v;
}

void put(Nod** coada, Vehicul v) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = v;
    nou->next = NULL;

    if (*coada == NULL) {
        *coada = nou;
    }
    else {
        Nod* temp = *coada;
        while (temp->next)
            temp = temp->next;
        temp->next = nou;
    }
}

void afisareCoada(Nod* coada) {
    while (coada) {
        printf("ID: %d | Capacitate: %.2f | Sofer: %s | Marca: %s | An: %d | Consum: %.2lf | Timp: %ld\n",
            coada->info.id,
            coada->info.capacitate,
            coada->info.sofer,
            coada->info.marca,
            coada->info.anFabricatie,
            *(coada->info.consum),
            coada->info.timpFunctionare);
        coada = coada->next;
    }
}

void elibereazaCoada(Nod** coada) {
    while (*coada) {
        Nod* temp = *coada;
        free(temp->info.sofer);
        free(temp->info.marca);
        free(temp->info.consum);
        *coada = (*coada)->next;
        free(temp);
    }
}

void citesteFisier(const char* numeFisier, Nod** coada) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare!");
        return;
    }

    int id, an;
    float capacitate;
    char sofer[100], marca[100];
    double consum;
    long timp;

    while (fscanf(f, "%d %f %s %s %d %lf %ld", &id, &capacitate, sofer, marca, &an, &consum, &timp) == 7) {
        Vehicul v = creareVehicul(id, capacitate, sofer, marca, an, consum, timp);
        put(coada, v);
    }

    fclose(f);
}

Vehicul* ultimulVehiculCuCapacitate(Nod* coada, float prag) {
    Vehicul* rezultat = NULL;
    while (coada) {
        if (coada->info.capacitate > prag) {
            rezultat = &coada->info;
        }
        coada = coada->next;
    }
    return rezultat;
}

int hashFunction(const char* marca, int dim) {
    int suma = 0;
    for (int i = 0; marca[i]; i++) {
        suma += marca[i];
    }
    return suma % dim;
}

Hashtable* initializareHashtable(int dim) {
    Hashtable* ht = (Hashtable*)malloc(sizeof(Hashtable));
    ht->dimensiune = dim;
    ht->vector = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < dim; i++) {
        ht->vector[i] = NULL;
    }
    return ht;
}

Vehicul copieVehicul(Vehicul original) {
    Vehicul v;
    v.id = original.id;
    v.capacitate = original.capacitate;

    v.sofer = (char*)malloc(strlen(original.sofer) + 1);
    strcpy(v.sofer, original.sofer);

    v.marca = (char*)malloc(strlen(original.marca) + 1);
    strcpy(v.marca, original.marca);

    v.anFabricatie = original.anFabricatie;

    v.consum = (double*)malloc(sizeof(double));
    *(v.consum) = *(original.consum);

    v.timpFunctionare = original.timpFunctionare;

    return v;
}

void inserareInHashtable(Hashtable* ht, Vehicul v) {
    int poz = hashFunction(v.marca, ht->dimensiune);
    Vehicul copie = copieVehicul(v);
    put(&ht->vector[poz], copie);
}

Nod** cautaDupaCheie(Hashtable* ht, const char* marca, int* nrRezultate) {
    int poz = hashFunction(marca, ht->dimensiune);
    Nod* temp = ht->vector[poz];

    int count = 0;
    for (Nod* p = temp; p != NULL; p = p->next) {
        count++;
    }

    Nod** vector = (Nod**)malloc(sizeof(Nod*) * count);
    int index = 0;
    for (Nod* p = temp; p != NULL; p = p->next) {
        vector[index++] = p;
    }

    *nrRezultate = count;
    return vector;
}

void afisareHashtable(Hashtable* ht) {
    for (int i = 0; i < ht->dimensiune; i++) {
        printf("Pozitie %d:\n", i);
        Nod* temp = ht->vector[i];
        while (temp) {
            printf(" -> ID: %d | Marca: %s | Sofer: %s\n", temp->info.id, temp->info.marca, temp->info.sofer);
            temp = temp->next;
        }
    }
}


void stergeDupaCriteriu(Hashtable* ht, long pragTimp) {
    for (int i = 0; i < ht->dimensiune; i++) {
        Nod* current = ht->vector[i];
        Nod* prev = NULL;

        while (current != NULL) {
            if (current->info.timpFunctionare < pragTimp) {
                Nod* deSters = current;

                if (prev == NULL) {
                    ht->vector[i] = current->next;
                    current = ht->vector[i];
                }
                else {
                    prev->next = current->next;
                    current = current->next;
                }

                free(deSters->info.sofer);
                free(deSters->info.marca);
                free(deSters->info.consum);
                free(deSters);
            }
            else {
                prev = current;
                current = current->next;
            }
        }
    }
}



void afisareVectorNoduri(Nod** vector, int dim) {
    for (int i = 0; i < dim; i++) {
        printf("Vector[%d]: ID: %d | Marca: %s | Sofer: %s\n", i,
            vector[i]->info.id, vector[i]->info.marca, vector[i]->info.sofer);
    }
}



void main() {
    Nod* coada = NULL;
    citesteFisier("Fisier.txt", &coada);
    afisareCoada(coada);

    Vehicul* vGasit = ultimulVehiculCuCapacitate(coada, 3.5);
    if (vGasit) {
        printf("\nUltimul vehicul cu capacitate > 3.5: %s, %.2f\n", vGasit->marca, vGasit->capacitate);
    }

    Hashtable* ht = initializareHashtable(5);
    Nod* temp = coada;
    while (temp) {
        inserareInHashtable(ht, temp->info);
        temp = temp->next;
    }

    printf("\nTabela de dispersie:\n");
    afisareHashtable(ht);

    int nr = 0;
    Nod** vector = cautaDupaCheie(ht, "Dacia", &nr);
    printf("\nVector cu elementele de pe cheia 'Dacia':\n");
    afisareVectorNoduri(vector, nr);
    free(vector);

    stergeDupaCriteriu(ht, 10000);
    printf("\nTabela dupa stergere vehicule cu timpFunctionare < 10000:\n");
    afisareHashtable(ht);

    elibereazaCoada(&coada);
    
    for (int i = 0; i < ht->dimensiune; i++) {
        elibereazaCoada(&ht->vector[i]);
    }
    free(ht->vector);
    free(ht);
}

//autoevaluare: 10