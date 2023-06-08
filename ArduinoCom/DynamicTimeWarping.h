#pragma once
#include <limits>
#include "BoundedPriorityQueue.h"
#include "DataStruct.h"

#define FLOAT_MAX std::numeric_limits<float>::max()
#define CLASS_MAX 2

double DTW_distance(Pattern* s1, Pattern* s2, float max_dist);

size_t knn_class(Pattern* query, std::vector<Pattern*>& dataset, size_t k);


