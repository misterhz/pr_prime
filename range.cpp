#include "range.h"

int get_range_start(int total_count, int count, int start, int end) {
    int range = end - start;
    return range / (total_count + .0f) * count + start;
}

int get_range_end(int total_count, int count, int start, int end) {
    return count == total_count - 1 ? end : get_range_start(total_count, count + 1, start, end);
}