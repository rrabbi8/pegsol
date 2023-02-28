#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"


void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;	
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
 * Md Reaj Uddin Rabbi, 955374
 * mrabbi@student.unimelb.edu.au, October 2020
*/
node_t* applyAction(node_t* n, position_s* selected_peg, move_t action ){

	//points to its parent
    node_t* new_node = n;
	
	//update state with the chosen action
    execute_move_t(&(new_node->state), &(new_node->state.cursor), action);
	
	//update depth 
	new_node->depth += 1;

	//update action used to create new state
	new_node->move = action;

	return new_node;

}

/**
 * Solution to Peg Solitaire path as per algorithm description in the handout
 * Md Reaj Uddin Rabbi, 955374
 * mrabbi@student.unimelb.edu.au, October 2020
 */

void find_solution( state_t* init_state  ){
    
    //to keep track of the expanded nodes
	int size_of_array = SIZE;
	node_t **track_nodes = (node_t **)malloc(sizeof(node_t *)*size_of_array);
	int count = 0;

	HashTable table;

	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup( &table, sizeof(int8_t) * SIZE * SIZE, sizeof(int8_t) * SIZE * SIZE, 16769023);

	// Initialize Stack
	initialize_stack();

	//Add the initial node
	node_t* n = create_init_node( init_state );
	
    //push intitial node in the stack
	stack_push(n);

	//initial number of pegs in the board
	int remaining_pegs = num_pegs(&(n->state));

	while ( stack_top_idx >= 0){ //if stack is empty then stack index is -1

		if (count == size_of_array){
			size_of_array *= 2;
			track_nodes = (node_t **)realloc(track_nodes, sizeof(node_t *) * size_of_array);
		}
		//expand top node from stack
		n = stack_top(); stack_pop();
		track_nodes[count++] = n;
		expanded_nodes++;

		//check if a better solution is found
		if (num_pegs(&(n->state)) < remaining_pegs){
			save_solution(n);
			remaining_pegs = num_pegs(&(n->state));
		}

		for (int8_t x = 0; x<SIZE; x++){
			for (int8_t y = 0; y<SIZE; y++) {
				
				//create a child node
				node_t * new_node = create_init_node(&(n->state));
				new_node->parent = n;
				new_node->depth = n->depth;
				new_node->state.cursor.x = x;
				new_node->state.cursor.y = y;
				
				//update the child node if possible move can be applied
				if (can_apply(&(new_node->state), &(new_node->state.cursor), right)){
					new_node = applyAction(new_node, &(new_node->state.cursor), right);
					generated_nodes++;

				} else if (can_apply(&(new_node->state), &(new_node->state.cursor), left)){
					new_node = applyAction(new_node, &(new_node->state.cursor), left);
					generated_nodes++;

				} else if (can_apply(&(new_node->state), &(new_node->state.cursor), up)){
					new_node = applyAction(new_node, &(new_node->state.cursor), up);
					generated_nodes++;

				} else if (can_apply(&(new_node->state), &(new_node->state.cursor), down)){
					new_node = applyAction(new_node, &(new_node->state.cursor), down);
					generated_nodes++;
				} else {						//if move cannot be applied then this is a duplicate state
					free(new_node);
					continue;
				}
				
				//pegsol is solved
				if (won(&(new_node->state))){
					save_solution(new_node);
					remaining_pegs = num_pegs(&(new_node->state));
					
					free_memory(track_nodes, count);
					free(new_node);
					free_stack();
					ht_destroy(&table);
					return; 
				}

				//check if current node state already in hash table 
				if (!ht_contains(&table, &(new_node->state))){
					ht_insert(&table, &(new_node->state), &(new_node->state));
					stack_push(new_node);
				}
			}
		}

		//budget exhausted
		if (expanded_nodes >= budget){

			free_memory(track_nodes, count);
			free_stack();
			ht_destroy(&table);
			return;
		}
	}
}

/**
 * Free the provided array
 * Md Reaj Uddin Rabbi, 955374
 * mrabbi@student.unimelb.edu.au, October 2020
*/

void free_memory(node_t **expanded_nodes, int n){
	for (int i=0; i<n; i++){
		free(expanded_nodes[i]);
	}
	free(expanded_nodes);
}

