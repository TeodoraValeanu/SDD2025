#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

typedef struct Transport {
    int id;
    float greutate;
    char* tipTransport; 
    char* firma;
    int costuri[3];
    double* distanta;
    long timpLivrare;
} Transport;

typedef struct Nod {
    Transport info;
   struct Nod* prev;
   struct Nod* next;
}Nod;

typedef struct NodSimplu {
    Transport info;
    struct NodSimplu* next;
} NodSimplu;

void afisareTransport(Transport transport) {
    printf("\nTransport ID: %d", transport.id);
    printf("\nGreutate: %.2f kg", transport.greutate);
    printf("\nTip transport: %s", transport.tipTransport);
    printf("\nFirma: %s", transport.firma);
    printf("\nCosturi: %d, %d, %d", transport.costuri[0], transport.costuri[1], transport.costuri[2]);
    printf("\nDistanta: %.2f km", transport.distanta);
    printf("\nTimp livrare: %ld ore\n", transport.timpLivrare);
}

void traversareLD(Nod* cap) {
    while (cap)
    {
        afisareTransport(cap->info);
        cap = cap->next;
    } 
}

Transport citireTransport(FILE* fileStream) {
    
    Transport transport;
    char buffer[50];

   
    fgets(buffer, sizeof(buffer), fileStream);
    transport.id = atoi(buffer);

    
    fgets(buffer, sizeof(buffer), fileStream);
    transport.greutate = atof(buffer);
 
    fgets(buffer, sizeof(buffer), fileStream);
    strtok(buffer, "\n");
    transport.tipTransport = (char*)malloc(strlen(buffer) + 1);
    strcpy(transport.tipTransport, buffer);

    
    fgets(buffer, sizeof(buffer), fileStream);
    strtok(buffer, "\n"); 
    transport.firma = (char*)malloc(strlen(buffer) + 1);
    strcpy(transport.firma, buffer);

    for (int i = 0; i < 3; i++) {
        fgets(buffer, sizeof(buffer), fileStream);
        transport.costuri[i] = atoi(buffer);
    }

    transport.distanta = (double*)malloc(sizeof(double));
    fgets(buffer, sizeof(buffer), fileStream);
    *transport.distanta = atof(buffer);

    fgets(buffer, sizeof(buffer), fileStream);
    transport.timpLivrare = atol(buffer);

    return transport;

}

void inserareInceputLD(Nod** cap, Nod** coada, Transport transport) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = transport;
    nou->prev = NULL;
    nou->next = *cap;

    if ((*cap) != NULL) {
    (*cap)->prev = nou;
    *cap = nou;
    }
    else {
        *cap = *coada = nou;
    }
   
}

Nod* filtrareLista(Nod* cap, float prag) {
    Nod* rezultat = NULL;
    Nod* coada = NULL;
    while (cap) {
        if (cap->info.greutate > prag) {
            inserareInceputLD(&rezultat, &coada, cap->info);
        }
        cap = cap->next;
    }
    return rezultat;
}

void inserareDupaNod(Nod* cap, Transport transport, float prag) {
    while (cap) {
        if (cap->info.greutate > prag) {
            Nod* nou = (Nod*)malloc(sizeof(Nod));
            nou->info = transport;
            nou->next = cap->next;
            nou->prev = cap;

            if (cap->next != NULL) {
                cap->next->prev = nou;
            }

            cap->next = nou;

            if (nou->next == NULL) {
            }

            break;
        }
        cap = cap->next;
    }
}


void dezalocareLD(Nod** cap, Nod** coada) {
    while (*cap) {
        free((*cap)->info.tipTransport);
        free((*cap)->info.firma);
        free((*cap)->info.distanta);
        Nod* temp = *cap;
        *cap = (*cap)->next;
        free(temp);
    }
    *coada = NULL;
}


void traversareListaCirculara(Nod* cap) {
    if (!cap) {
        return;
    }
    Nod* copie = cap;
    do {
        afisareTransport(cap->info);
        cap = cap->next;
    } while (cap != copie);
}

void inserareLaInceputLS(NodSimplu** cap, Transport transport) {
    NodSimplu* nou = (NodSimplu*)malloc(sizeof(NodSimplu));
    nou->info = transport;
    nou->next = *cap;
    *cap = nou;
}

void traversareLS(NodSimplu* cap)
{
    while (cap) {
        afisareTransport(cap->info);
        cap = cap->next;
    }
}

void dezalocareLS(NodSimplu** cap) {
    while ((*cap)) {
        free((*cap)->info.tipTransport);
        free((*cap)->info.firma);
        free((*cap)->info.distanta);
        NodSimplu* temp = *cap;
        *cap = (*cap)->next;
        free(temp);
    }
}

void dezalocareLDCirculara(Nod** cap, Nod** coada) {
    if (*cap == NULL) {
        return; 
    }

    Nod* temp = *cap;
    do {
        if (temp->info.tipTransport != NULL) {
            free(temp->info.tipTransport);
        }
        if (temp->info.firma != NULL) {
            free(temp->info.firma);
        }
        if (temp->info.distanta != NULL) {
            free(temp->info.distanta);
        }

        Nod* aux = temp;
        temp = temp->next;  

        free(aux);
    } while (temp != *cap); 

    *cap = NULL;
    *coada = NULL;
}



void main() {
    FILE* file = fopen("Transport.txt", "r");
    Nod* cap = NULL;
    Nod* coada = NULL;
    while (!feof(file)) {
        Transport t = citireTransport(file);
        if (feof(file)) {
            break;  
        }

        inserareInceputLD(&cap, &coada, t);
    }
    fclose(file);

    printf("___________________________\n");
    printf("Lista dublu inlantuita:\n");
    traversareLD(cap);

    printf("___________________________");
    Nod* listaFiltrata = filtrareLista(cap, 1000);
    printf("\nLista filtrata (greutate > 1000):\n");
    traversareLD(listaFiltrata);

    printf("___________________________\n");
    printf("Adaugare nod dupa primul nod care indeplineste un criteriu:\n");
    Transport nouTransport = { 4, 1200.0, "Terestru", "TNT", { 150, 200, 250 }, 899, 59 };
    inserareDupaNod(listaFiltrata, nouTransport, 1000);
    traversareLD(listaFiltrata);

    printf("___________________________\n");
    printf("Transformare in LDI circulara:\n");
    coada->next = cap;
    cap->prev = coada;
    traversareListaCirculara(cap);  
    dezalocareLDCirculara(&cap, &coada);

    printf("___________________________\n");
    printf("Transformare in LS:\n");
    NodSimplu* capLS = NULL;
    FILE* file1 = fopen("Transport.txt", "r");
    while (!feof(file1)) {
        Transport t1 = citireTransport(file1);
        if (feof(file1)) {
            break;
        }
        inserareLaInceputLS(&capLS, t1);
    } 
    traversareLS(capLS);
    fclose(file1);
  
    dezalocareLS(&capLS);

    //autoevaluare: 10
}