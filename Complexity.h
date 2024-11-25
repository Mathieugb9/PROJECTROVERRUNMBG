#ifndef COMPLEXITY_H
#define COMPLEXITY_H

static long operation_count = 0;

/**
 * Increment the global operation counter.
 * @param count Number of operations to increment.
 */
void increment_operations(long count);

/**
 * Get the current operation count.
 * @return The total number of operations.
 */
long get_operation_count();

/**
 * Reset the operation counter.
 */
void reset_operation_count();

#endif // COMPLEXITY_H
