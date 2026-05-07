#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>

#define N 10000

typedef struct Drone {
    int id;
    float x, y, z;
} Drone;

// Distance au carré (évite sqrt inutile)
float distance_sq(const Drone *a, const Drone *b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float dz = a->z - b->z;
    return dx*dx + dy*dy + dz*dz;
}

// Fusion de deux sous-tableaux triés selon x
void merge_by_x(Drone *arr, int left, int mid, int right, Drone *temp) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if ((arr + i)->x <= (arr + j)->x)
            *(temp + k++) = *(arr + i++);
        else
            *(temp + k++) = *(arr + j++);
    }
    while (i <= mid) *(temp + k++) = *(arr + i++);
    while (j <= right) *(temp + k++) = *(arr + j++);
    for (i = left; i <= right; ++i)
        *(arr + i) = *(temp + i);
}

// Tri fusion récursif
void merge_sort(Drone *arr, int left, int right, Drone *temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid, temp);
        merge_sort(arr, mid + 1, right, temp);
        merge_by_x(arr, left, mid, right, temp);
    }
}

// Tri public par x
void sort_drones_by_x(Drone *drones, int n) {
    Drone *temp = (Drone*) malloc(n * sizeof(Drone));
    if (!temp) return;
    merge_sort(drones, 0, n - 1, temp);
    free(temp);
}

// Recherche de la paire la plus proche dans le tableau trié
void find_closest_pair(const Drone *drones, int n, int *id1, int *id2, float *dist) {
    float min_sq = FLT_MAX;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            float dx = (drones + j)->x - (drones + i)->x;
            if (dx * dx >= min_sq) break;   // élagage par X
            float d2 = distance_sq(drones + i, drones + j);
            if (d2 < min_sq) {
                min_sq = d2;
                *id1 = (drones + i)->id;
                *id2 = (drones + j)->id;
            }
        }
    }
    *dist = sqrtf(min_sq);
}

int main() {
    // Allocation d'un bloc contigu 
    Drone *drones = (Drone*) malloc(N * sizeof(Drone));
    if (!drones) return 1;

    // Initialisation
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; ++i) {
        (drones + i)->id = i + 1;
        (drones + i)->x = (float)rand() / RAND_MAX * 1000.0f;
        (drones + i)->y = (float)rand() / RAND_MAX * 1000.0f;
        (drones + i)->z = (float)rand() / RAND_MAX * 1000.0f;
    }

    // Tri par x
    sort_drones_by_x(drones, N);

    int idA, idB;
    float distance;
    find_closest_pair(drones, N, &idA, &idB, &distance);

    printf("Drones les plus proches : %d et %d\n", idA, idB);
    printf("Distance = %f\n", distance);

    free(drones);
    return 0;
}
