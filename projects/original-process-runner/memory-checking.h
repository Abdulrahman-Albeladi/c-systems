#ifndef MEMORY_CHECKING_H
#define MEMORY_CHECKING_H

/**
 * Configure heap memory checking facilities for the current process.
 */
void setup_memory_checking(void);

/**
 * Inspect the heap for memory-management errors.
 */
void check_heap(void);

#endif /* MEMORY_CHECKING_H */
