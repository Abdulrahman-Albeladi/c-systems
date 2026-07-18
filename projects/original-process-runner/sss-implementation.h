#ifndef SSS_IMPLEMENTATION_H
#define SSS_IMPLEMENTATION_H

/*
 * Command lists used by the secure secret-sharing implementation.
 * Each array contains command arguments, with the corresponding count
 * stored in comp_num or test_num.
 */
typedef struct commands {
  char **compile_arr;
  char **test_arr;
  int comp_num;
  int test_num;
} Commands;

#endif /* SSS_IMPLEMENTATION_H */
