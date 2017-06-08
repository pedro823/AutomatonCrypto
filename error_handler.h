#ifndef __ERROR_HANDLER__
#define __ERROR_HANDLER__

#include <stdlib.h>

/* Allocates memory, kills program if insuficient memory. */
void* emalloc(size_t size);

/* Like printf, prints error message and kills program. */
void kill(const char* expression, ...);

/* On error message, will print program name */
void set_program_name(const char* program_name);

/* Add function to stack of functions to print on error. */
void add_to_stack(const char* function_name);

/* Pops stack. Will not print popped function on error. */
void pop_stack();

#endif
