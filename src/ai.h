#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "utils.h"

void initialize_ai();

void find_solution( state_t* init_state );
void free_memory(node_t **expanded_nodes, int n);

#endif
