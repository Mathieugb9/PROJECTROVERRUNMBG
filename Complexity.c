#include "Complexity.h"

void increment_operations(long count) {
    operation_count += count;
}

long get_operation_count() {
    return operation_count;
}

void reset_operation_count() {
    operation_count = 0;
}
