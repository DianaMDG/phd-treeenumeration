/******************************************************************/
/******************   FUNCTION Declaration   **********************/
/******************************************************************/


void generate_seq(int spaces, int *generated);
/*Recursive function that generates the Prüfer sequences that will generate the trees*/

void generate_seq_iteratively(int *generated);
/*Iterative function that generates the Prüfer sequences that will generate the trees WITH THE SUPER NODE*/

void generate_tree(int *seq);
/*Function that generates trees from Prüfer sequences */


/*************************************************************************
*                    Functions for the Adjacency List
*************************************************************************/

void unfold_SN_list_recursive(int index);
/*Function used to generate combinations of connections to super node, recursively*/

static void unfold_SN_list(void);
/*Function used to generate combinations of connections to super node*/

void generate_graph_list(void);
/*Function that generates the representation for a given tree*/

void print_list(void);
/*Function that prints the adjacency list*/

void clear_list(void);
/*Function that clears an adjacency list BUT THE (0,1) and (0,2) edges*/

void apply_prim_list(void);
/*Function that checks if the tree of a given representation is its minnimum spanning tree*/

