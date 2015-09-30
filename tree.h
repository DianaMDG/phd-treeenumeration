
/*typedef uint16_t ele_t;*/
typedef char ele_t;

/*Indexed array*/
struct node {
    int value;
    struct node *children[N-1];
};

/******************************************************************/
/******************   FUNCTION Declaration   **********************/
/******************************************************************/


/*Recursive function that generates the Prüfer sequences that will generate the trees*/
void generate_seq(int spaces, int *generated);

/*Function that generates trees from Prüfer sequences */
void generate_tree(int *seq);

/*Function that prints the adjacency matrix of a given tree*/
inline void print_adj(ele_t **lines)

/*Function that clears an adjacency matrix -> sets to zeros */
inline void clear_adj(ele_t adjacency[]);

