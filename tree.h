/************************************************************************
*
*                              DEFINEs
*
************************************************************************/

/*List of defines*/
/*#define NUMBERS       when defined: counts number of sequences, representations and cuts*/
/*#define TOFILE        when defined: prints sequences, representations and cuts to files*/
/*#define GRAPH         when defined: generates the representation associated with each sequence*/
/*#define PRIM          when defined: checks if the tree is canonical using the Prim algorithm*/
/*#define CONSTANTS     when defined: checks if the representation is minimal regarding constants*/
/*#define ITERATIVE     when defined: generates the sequences and unfolds the super node iteratively. WARNING! Is slower!*/
/*#define TEST          when defined: runs the code for a single test Prüfer sequence*/

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
    #ifndef GRAPH
        #define GRAPH  1
    #endif
    #pragma message "Verifying if is minimal regarding constants"
#endif

#ifdef TEST
    #pragma message "running the program for a single Prüfer sequence"
#endif

/************************************************************************
*
*                        FUNCTION Declaration
*
************************************************************************/

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

/*Function that applies cuts to a given representation according to user defines*/
void verify_representation();

/*Function that prints the adjacency list*/
void print_list();

/*Function that clears the adjacency for the next Prüfer sequence*/
void clear_list();


