/************************************************************************
*
*                        FUNCTION Declaration
*
************************************************************************/

/*Function that runs the program*/
void program();

/*Recursive function that generates the Prüfer sequences that will generate the trees with the 1-0-2 super node*/
void generate_seq(int spaces, int *generated);

/*Function that generates trees from Prüfer sequences with the 1-0-2 super node*/
void generate_tree(int *seq);

/*************************************************************************
*                    Functions for the Adjacency List
*************************************************************************/

/*Recursive function used to generate combinations of connections to super node*/
void unfold_SN_list(int index);

/*Function that generates the zero representation for a given tree*/
void generate_graph();

/*Function that applies cuts to the representation according to specified by the user*/
void verify_representation();

/*Function that prints the adjacency list*/
void print_list();

/*Function that clears the adjacency for the next Prüfer sequence*/
void clear_list();


/************************************************************************
*                  Functions for the Iterative Case
*************************************************************************/
#ifdef ITERATIVE

/*Iterative function that generates the Prüfer sequences that will generate the trees with the 1-0-2 super node*/
void generate_seq_iterative(int *generated);

/*Iterative function used to generate combinations of connections to super node*/
static void unfold_SN_list_iterative();

#endif

