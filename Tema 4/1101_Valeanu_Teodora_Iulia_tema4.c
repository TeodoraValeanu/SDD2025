#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

typedef struct SmartDevice {
    int deviceID;
    char* deviceName;
    char* manufacturer;
    int releaseYear;
    float firmwareVersion;
    double* sensorReadings;
    char* ipAddress;
} SmartDevice;

typedef struct Nod {
    SmartDevice info;
    struct Nod* stanga;
    struct Nod* dreapta;
} Nod;

typedef struct MaxHeap {
    SmartDevice* vector;
    int dim;
} MaxHeap;

SmartDevice initSmartDevice(int id, float fwVer, const char* name, const char* manuf,
    int year, double reading, const char* ip) {
    SmartDevice device;
    device.deviceID = id;
    device.firmwareVersion = fwVer;
    device.releaseYear = year;

    device.deviceName = (char*)malloc(strlen(name) + 1);
    strcpy(device.deviceName, name);

    device.manufacturer = (char*)malloc(strlen(manuf) + 1);
    strcpy(device.manufacturer, manuf);

    device.ipAddress = (char*)malloc(strlen(ip) + 1);
    strcpy(device.ipAddress, ip);

    device.sensorReadings = (double*)malloc(sizeof(double));
    *(device.sensorReadings) = reading;

    return device;
}

void afisareSmartDevice(SmartDevice d) {
    printf("Device ID: %d\n", d.deviceID);
    printf("Device Name: %s\n", d.deviceName);
    printf("Manufacturer: %s\n", d.manufacturer);
    printf("Release Year: %d\n", d.releaseYear);
    printf("Firmware Version: %.2f\n", d.firmwareVersion);
    printf("Sensor Reading: %.2lf\n", *(d.sensorReadings));
    printf("IP Address: %s\n\n", d.ipAddress);
}

void inserareBST(Nod** radacina, SmartDevice info) {
    if (*radacina == NULL) {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->info = info;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        *radacina = nou;
    }
    else if (info.deviceID < (*radacina)->info.deviceID) {
        inserareBST(&(*radacina)->stanga, info);
    }
    else {
        inserareBST(&(*radacina)->dreapta, info);
    }
}

void citesteFisier(const char* numeFisier, Nod** radacina) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare la deschiderea fisierului!");
        return;
    }

    int id, year;
    float fwVer;
    double reading;
    char name[100], manuf[100], ip[50];

    while (fscanf(f, "%d %f %s %s %d %lf %s", &id, &fwVer, name, manuf, &year, &reading, ip) == 7) {
        SmartDevice dev = initSmartDevice(id, fwVer, name, manuf, year, reading, ip);
        inserareBST(radacina, dev);
    }

    fclose(f);
}

void parcurgereInordineArbore(Nod* radacina) {
    if (radacina) {
        parcurgereInordineArbore(radacina->stanga);
        afisareSmartDevice(radacina->info);
        parcurgereInordineArbore(radacina->dreapta);
    }
}

int numaraFrunze(Nod* radacina) {
    if (radacina == NULL)
        return 0;
    if (radacina->stanga == NULL && radacina->dreapta == NULL)
        return 1;
    return numaraFrunze(radacina->stanga) + numaraFrunze(radacina->dreapta);
}

int numaraFrunzeSubarboreDrept(Nod* radacina) {
    if (radacina == NULL || radacina->dreapta == NULL)
        return 0;
    return numaraFrunze(radacina->dreapta);
}

void adaugaInVector(SmartDevice* vector, int* dim, Nod* radacina) {
    if (radacina) {
        adaugaInVector(vector, dim, radacina->stanga);

        SmartDevice deviceNou;
        deviceNou.deviceID = radacina->info.deviceID;

        deviceNou.deviceName = (char*)malloc(strlen(radacina->info.deviceName) + 1);
        strcpy(deviceNou.deviceName, radacina->info.deviceName);

        deviceNou.manufacturer = (char*)malloc(strlen(radacina->info.manufacturer) + 1);
        strcpy(deviceNou.manufacturer, radacina->info.manufacturer);

        deviceNou.ipAddress = (char*)malloc(strlen(radacina->info.ipAddress) + 1);
        strcpy(deviceNou.ipAddress, radacina->info.ipAddress);

        deviceNou.releaseYear = radacina->info.releaseYear;
        deviceNou.firmwareVersion = radacina->info.firmwareVersion;

        deviceNou.sensorReadings = (double*)malloc(sizeof(double));
        *(deviceNou.sensorReadings) = *(radacina->info.sensorReadings);

        vector[*dim] = deviceNou;
        (*dim)++;

        adaugaInVector(vector, dim, radacina->dreapta);
    }
}


void filtrareMaxHeap(MaxHeap mHeap, int index) {
    if (mHeap.dim > 0) {
        int pozMax = index;
        int pozSt = 2 * index + 1;
        int pozDr = 2 * index + 2;

        if (pozSt < mHeap.dim && mHeap.vector[pozMax].deviceID < mHeap.vector[pozSt].deviceID) {
            pozMax = pozSt;
        }
        if (pozDr < mHeap.dim && mHeap.vector[pozMax].deviceID < mHeap.vector[pozDr].deviceID) {
            pozMax = pozDr;
        }

        if (pozMax != index) {
            SmartDevice aux = mHeap.vector[index];
            mHeap.vector[index] = mHeap.vector[pozMax];
            mHeap.vector[pozMax] = aux;

            if (pozMax <= mHeap.dim / 2 - 1) {
                filtrareMaxHeap(mHeap, pozMax);
            }
        }
    }
}

void construireHeap(MaxHeap* mHeap) {
    for (int i = mHeap->dim / 2 - 1; i >= 0; i--) {
        filtrareMaxHeap(*mHeap, i);
    }
}

void traversareMaxHeap(MaxHeap m) {
    for (int i = 0; i < m.dim; i++)
    {
        afisareSmartDevice(m.vector[i]);
    }
};

void afisareFrunzeHeap(MaxHeap heap) {
    printf("\nNoduri frunza in MaxHeap:\n");
    for (int i = heap.dim / 2; i < heap.dim; i++) {
        afisareSmartDevice(heap.vector[i]);  
    }
}

void stergeDupaAn(MaxHeap* heap, int pragAn) {
    int i = 0;
    while (i < heap->dim) {
        if (heap->vector[i].releaseYear < pragAn) {
            
            free(heap->vector[i].deviceName);
            free(heap->vector[i].manufacturer);
            free(heap->vector[i].ipAddress);
            free(heap->vector[i].sensorReadings);

           
            heap->vector[i] = heap->vector[heap->dim - 1];
            heap->dim--;

            filtrareMaxHeap(*heap, i);
        }
        else {
            i++;
        }
    }
}


void dezalocare(Nod** radacina) {
    if (*radacina) {
        dezalocare(&(*radacina)->stanga);
        dezalocare(&(*radacina)->dreapta);

        free((*radacina)->info.deviceName);
        free((*radacina)->info.manufacturer);
        free((*radacina)->info.ipAddress);
        free((*radacina)->info.sensorReadings);

        free(*radacina);
        *radacina = NULL;
    }
}

void main() {
    Nod* radacina = NULL;
    citesteFisier("date.txt", &radacina);

    printf("Afisare arbore (inordine):\n\n");
    parcurgereInordineArbore(radacina);

    int nrFrunzeDreapta = numaraFrunzeSubarboreDrept(radacina);
    printf("\nNumarul de frunze din subarborele drept: %d\n", nrFrunzeDreapta);

    MaxHeap heap;
    heap.dim = 0;
    heap.vector = (SmartDevice*)malloc(sizeof(SmartDevice) * 100);

    adaugaInVector(heap.vector, &heap.dim, radacina);
    construireHeap(&heap);

    printf("\nMaxHeap initial:\n");
    traversareMaxHeap(heap);

    afisareFrunzeHeap(heap);

    stergeDupaAn(&heap, 2020);

    printf("\nMaxHeap dupa stergere dispozitive mai vechi de anul 2020:\n");
    traversareMaxHeap(heap);

    
    for (int i = 0; i < heap.dim; i++) {
        free(heap.vector[i].deviceName);
        free(heap.vector[i].manufacturer);
        free(heap.vector[i].ipAddress);
        free(heap.vector[i].sensorReadings);
    }
    free(heap.vector);
    dezalocare(&radacina);
    //autoevaluare: 10
}