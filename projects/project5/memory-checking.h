#ifndef PROJECT5_MEMORY_CHECKING_H
#define PROJECT5_MEMORY_CHECKING_H

/** Configure optional runtime heap diagnostics. */
void setup_memory_checking(void);

/** Check the heap for corruption when supported by the runtime. */
void check_heap(void);

#endif /* PROJECT5_MEMORY_CHECKING_H */
