//
// Created by moraski on 18/10/2021.
//

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "quicksort.h"

void Troca(int *v, int i, int j) {
    if (v == NULL) return;
    int aux;
    aux = v[i];
    v[i] = v[j];
    v[j] = aux;
}

int Partition(int *v, int p, int r) {
    int x, i;
    x = v[r];
    i = p - 1;
    for (int j = p; j < r; j++) {
        if (v[j] <= x) {
            i++;
            Troca(v, i, j);
        }
    }
    Troca(v, i + 1, r);
    return i + 1;
}

int RandomPartition(int *v, int p, int r) {
    int x, i;
    Troca(v, (p+r)/2, r);
    x = v[r];
    i = p - 1;
    for (int j = p; j < r; j++) {
        if (v[j] <= x) {
            i++;
            Troca(v, i, j);
        }
    }
    Troca(v, i + 1, r);
    return i + 1;
}

int MedPartition(int *v, int p, int r) {
    int x;
    int i, j;
    x = v[r];
    i = p - 1;
    for (j = p; j < r; j++) {
        if (v[j] <= x) {
            i++;
            Troca(v, i, j);
        }
    }
    Troca(v, i + 1, r);
    return i + 1;
}


void QuickSort(int *v, int e, int d) {
    int q;
    if (e < d) {
        q = Partition(v, e, d);
        QuickSort(v, e, q - 1);
        QuickSort(v, q + 1, d);
    }
}

void RandomQuickSort(int *v, int e, int d) {
    int q;
    if (e < d) {
        q = RandomPartition(v, e, d);
        RandomQuickSort(v, e, q - 1);
        RandomQuickSort(v, q + 1, d);
    }
}

void MedQuickSort(int *v, int e, int d) {
    int q;
    if (e < d) {
        q = MedPartition(v, e, d);
        MedQuickSort(v, e, q - 1);
        MedQuickSort(v, q + 1, d);
    }
}