#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int id;
    float x, y, z;
} Drone;

// Distance euclidienne
float distance(Drone *a, Drone *b) {
    return sqrt(
        (a->x - b->x)*(a->x - b->x) +
        (a->y - b->y)*(a->y - b->y) +
        (a->z - b->z)*(a->z - b->z)
    );
}

// Swap avec pointeurs
void swap(Drone *a, Drone *b) {
    Drone temp = *a;
    *a = *b;
    *b = temp;
}

// Tri rapide (QuickSort) basé sur x
void quickSort(Drone *base, int left, int right) {
    if (left >= right) return;

    float pivot = (base + right)->x;
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if ((base + j)->x < pivot) {
            i++;
            swap(base + i, base + j);
        }
    }

    swap(base + i + 1, base + right);

    quickSort(base, left, i);
    quickSort(base, i + 2, right);
}

// Version naïve (petits cas)
float bruteForce(Drone *drones, int n, Drone **d1, Drone **d2) {
    float min = 1e9;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            float d = distance(drones + i, drones + j);
            if (d < min) {
                min = d;
                *d1 = drones + i;
                *d2 = drones + j;
            }
        }
    }
    return min;
}

// Fonction principale Divide & Conquer
float closestUtil(Drone *drones, int n, Drone **d1, Drone **d2) {
    if (n <= 3)
        return bruteForce(drones, n, d1, d2);

    int mid = n / 2;
    Drone *midDrone = drones + mid;

    Drone *dl1, *dl2, *dr1, *dr2;

    float dl = closestUtil(drones, mid, &dl1, &dl2);
    float dr = closestUtil(drones + mid, n - mid, &dr1, &dr2);

    float d = dl;
    *d1 = dl1;
    *d2 = dl2;

    if (dr < d) {
        d = dr;
        *d1 = dr1;
        *d2 = dr2;
    }

    // Zone critique (strip)
    Drone *strip = (Drone *)malloc(n * sizeof(Drone));
    int j = 0;

    for (int i = 0; i < n; i++) {
        if (fabs((drones + i)->x - midDrone->x) < d) {
            *(strip + j) = *(drones + i);
            j++;
        }
    }

    for (int i = 0; i < j; i++) {
        for (int k = i + 1; k < j && k < i + 7; k++) {
            float dist = distance(strip + i, strip + k);
            if (dist < d) {
                d = dist;
                *d1 = strip + i;
                *d2 = strip + k;
            }
        }
    }

    free(strip);
    return d;
}

// Fonction principale
int main() {
    int n = 10000;

    Drone *essaim = (Drone *)malloc(n * sizeof(Drone));

    // Initialisation
    for (int i = 0; i < n; i++) {
        (essaim + i)->id = i;
        (essaim + i)->x = rand() % 1000;
        (essaim + i)->y = rand() % 1000;
        (essaim + i)->z = rand() % 1000;
    }

    quickSort(essaim, 0, n - 1);

    Drone *d1, *d2;
    float minDist = closestUtil(essaim, n, &d1, &d2);

    printf("Distance minimale: %f\n", minDist);
    printf("Drone 1 ID: %d\n", d1->id);
    printf("Drone 2 ID: %d\n", d2->id);

    free(essaim);
    return 0;
}
