#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <omp.h>

#include "../include/sort.h"
#include "../include/edgelist.h"

// Order edges by id of a source vertex, 
// using the Counting Sort
// Complexity: O(E + V)
void countSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {

    
    int i;
    int key;
    int pos;

    // auxiliary arrays, allocated at the start up of the program
    int *vertex_cnt = (int*)malloc(numVertices*sizeof(int)); // needed for Counting Sort

    for(i = 0; i < numVertices; ++i) {
        vertex_cnt[i] = 0;
    }

    // count occurrence of key: id of a source vertex
    for(i = 0; i < numEdges; ++i) {
        key = edges[i].src;
        vertex_cnt[key]++;
    }

    // transform to cumulative sum
    for(i = 1; i < numVertices; ++i) {
        vertex_cnt[i] += vertex_cnt[i - 1];
    }

    // fill-in the sorted array of edges
    for(i = numEdges - 1; i >= 0; --i) {
        key = edges[i].src;
        pos = vertex_cnt[key] - 1;
        edges_sorted[pos] = edges[i];
        vertex_cnt[key]--;
    }


    free(vertex_cnt);

}



void radixSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges) {
    //edges_sorted is sorted array
    //edges is array given
    //numVertices is largest number of digits
    //numEdges is number of elements in array
    //Each key in A[1..n] is a d-digit integer
    //Digits are numbered 1 to d from right to left
    //Finding number of digits, this is based on numVertices
    int digits;
    int temp;
    int i;
    int j;
    int k;
    //prettyPrint("Edge array Before radix sorting: { ", edges, numEdges);

    if (numVertices < 100000) {
        if (numVertices < 100) {
            if (numVertices < 10) { digits = 1; }
            else { digits = 2; }
        } else {
            if (numVertices < 1000) { digits = 3; }
            else {
                if (numVertices < 10000) { digits = 4; }
                else { digits = 5; }
            }
        }
    } else {
        if (numVertices < 10000000) {
            if (numVertices < 1000000) { digits = 6; }
            else { digits = 7; }
        } else {
            if (numVertices < 100000000) { digits = 8; }
            else {
                if (numVertices < 1000000000) { digits = 9; }
                else { digits = 10; }
            }
        }
    }
    //for 1 to #digits
    for (j = 1; j <= digits; j++) {
        //key, it is number at digit place j, that's why it's size 10, 0->9
        int count[10] = {0};

        //Store the count of 'keys' in count
#pragma omp parallel
{
        #pragma omp default(none) shared(numEdges, j, edges, count) private(i, temp) num_threads(16)
        for (i = 0; i < numEdges; i++) {
            //count[j-th digit of edges array]
            //temp = edges[i].src;
            //temp /= (int)pow(10.0, (double) j-1);
            //temp %= 10;
            //count[temp]++;
            count[(edges[i].src / (int)pow(10.0, (double) j-1)) % 10]++;
        }
}


        for (k = 1; k < 10; k++) {
            count[k] += count[k-1];
        }

        //Build new resulting array by checking
        //new position of A[i] from count[k]
#pragma omp parallel //start of parallel region
{
        #pragma omp parallel for default(none) shared(numEdges, j, edges, edges_sorted, count) private(i, temp) num_threads(16)
        for (i = numEdges - 1; i >= 0; i--) {
            temp = edges[i].src;
            temp /= (int)pow(10.0, (double) j-1);
            temp %= 10;
            edges_sorted[count[temp] - 1] = edges[i];
            count[temp]--;
        }
}

#pragma omp parallel
{
        #pragma omp default(none) shared(numEdges, edges, edges_sorted) private(i) num_threads(16)
        for (i = 0; i < numEdges; i++) {
            edges[i] = edges_sorted[i];
        }
}
    }
    //prettyPrint("Edge array after radix sorting : { ", edges_sorted, numEdges);
}

void prettyPrint(char * textDisp, struct Edge * edges_sorted, int numEdges) {
    printf(textDisp);
    for (int i = 0; i < numEdges; i++) {
        printf(" %d ", edges_sorted[i].src);
    }
    printf("} \n ");
}