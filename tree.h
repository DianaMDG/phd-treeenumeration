/*Data type of the elements of the adjacency matrix*/
/*typedef uint16_t ele_t;*/
typedef char ele_t;

/*#define matriz      0*/



/******************************************************************/
/******************   FUNCTION Declaration   **********************/
/******************************************************************/


/*Recursive function that generates the Prüfer sequences that will generate the trees*/
void generate_seq(int spaces, int *generated);

/*Function that generates trees from Prüfer sequences */
void generate_tree(int *seq);

#ifdef matriz

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

#else
/*************************************************************************
*                    Functions for the Adjacency List
*************************************************************************/

/*Function used to delete repeated in adjacency list*/
void verify_node(int node);

/*Function used to generate combinations of connections to super node*/
void recursive_list(int index);

/*Function that generates the calculates the representation for a given tree*/
void generate_graph_list(void);

/*Function that attributes zeros to the nodes, in a depth search*/
void check_node(int node);

/*Function that prints the adjacency list*/
void print_list(void);

/*Function that clears an adjacency list BUT THE (0,1) and (0,2) edges*/
void clear_list(void);

#endif
