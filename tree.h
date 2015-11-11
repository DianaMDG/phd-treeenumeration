/*Data type of the elements of the adjacency matrix*/
/*typedef uint16_t ele_t;*/
typedef char ele_t;


/******************************************************************/
/******************   FUNCTION Declaration   **********************/
/******************************************************************/


/*Recursive function that generates the Prüfer sequences that will generate the trees*/
void generate_seq(int spaces, int *generated);

/*Iterative function that generates the Prüfer sequences that will generate the trees WITH THE SUPER NODE*/
void generate_seq_iteratively(int *generated);

/*Function that generates trees from Prüfer sequences */
void generate_tree(int *seq);


#ifndef matriz
/*************************************************************************
*                    Functions for the Adjacency List
*************************************************************************/

/*Function used to generate combinations of connections to super node, recursively*/
void unfold_SN_list_recursive(int index);

/*Function used to generate combinations of connections to super node*/
static void unfold_SN_list(void);

/*Function that generates the representation for a given tree*/
void generate_graph_list(void);

/*Function that prints the adjacency list*/
void print_list(void);

/*Function that clears an adjacency list BUT THE (0,1) and (0,2) edges*/
void clear_list(void);

/*Function that checks if the tree of a given representation is its minnimum spanning tree*/
void apply_prim_list(void);


#else
/*************************************************************************
*                    Functions for the Adjacency Matrix
*************************************************************************/

/*Function used to generate combinations of connections to super node for building the adjacency matrix*/
void recursive(int degree, int next_edge, int *edges);

/*Function that generates the neutral network graph from the tree*/
void generate_graph(void);

/*Function that prints the adjacency matrix of a given tree*/
void print_adj(void);

/*Function that clears an adjacency matrix */
void clear_adj(void);
#endif

