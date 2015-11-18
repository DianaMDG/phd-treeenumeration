
/*List of defines*/
/*#define NUMBERS     0        when defined : counts number of sequences, representations and cuts*/
/*#define TOFILE      0        when defined : prints sequences, representations and cuts to files*/
/*#define GRAPH       1        when defined : generates the representation associated with each sequence*/
/*#define PRIM        0        when defined : checks if the tree is canonical using the Prim algorithm*/
/*#define CONSTANTS   0        when defined : checks if the representation is minimal regarding constants*/
/*#define ITERATIVE   0        when defined : generates the sequences and unfolds the super node iteratively. WARNING! Is slower!*/

#ifdef NUMBERS
    #pragma message "counting sequences, representations and cuts"
#endif

#ifdef TOFILE
    #pragma message "printing to files"
#endif

#ifdef GRAPH
    #pragma message "generating representations"
#endif

#ifdef PRIM
    #ifndef GRAPH
        #define GRAPH  1
    #endif
    #pragma message "calculating minimum spanning tree using Prim algorithm"
#endif

#ifdef CONSTANTS
    #ifndef PRIM
        #define PRIM  1
    #endif
    #ifndef GRAPH
        #define GRAPH  1
    #endif
    #pragma message "Verifying if is minimal regarding constants"
#endif


/******************************************************************/
/******************   FUNCTION Declaration   **********************/
/******************************************************************/


void generate_seq(int spaces, int *generated);
/*Recursive function that generates the Prüfer sequences that will generate the trees*/

void generate_tree(int *seq);
/*Function that generates trees from Prüfer sequences */


/*************************************************************************
*                    Functions for the Adjacency List
*************************************************************************/

void unfold_SN_list_recursive(int index);
/*Function used to generate combinations of connections to super node, recursively*/

void generate_graph_list(void);
/*Function that generates the representation for a given tree*/

void print_list(void);
/*Function that prints the adjacency list*/

void clear_list(void);
/*Function that clears an adjacency list BUT THE (0,1) and (0,2) edges*/

void apply_prim_list(void);
/*Function that checks if the tree of a given representation is its minnimum spanning tree*/

