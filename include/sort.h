#ifndef SORT_H
#define SORT_H

#include "edgelist.h"
// Order edges by id of a source vertex, 
// using the Counting Sort
// Complexity: O(E + V)
void countSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges);

void radixSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges);

void prettyPrint(char * textDisp, struct Edge * edges_sorted, int numEdges);

#endif