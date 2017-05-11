#ifndef _SHORTEST_PATH_H
#define _SHORTEST_PATH_H

#include "graph.h"

template<class T>
void dijkstra(const graph<T>& gref, const T& orig, const T& end);

#endif