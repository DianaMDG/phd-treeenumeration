/*Data type of the elements of the adjacency matrix*/
/*typedef uint16_t ele_t;*/
typedef char ele_t;

/*Indexed array -- NOT YET USED*/
/*struct node {
    int value;
    struct node *children[N-1];
};*/

/******************************************************************/
/******************   FUNCTION Declaration   **********************/
/******************************************************************/


/*Recursive function that generates the Prüfer sequences that will generate the trees*/
void generate_seq(int spaces, int *generated);

/*Function that generates trees from Prüfer sequences */
void generate_tree(int *seq);

/*Function that prints the adjacency matrix of a given tree*/
 void print_adj();

/*Function that clears an adjacency matrix -> sets to zeros */
 void clear_adj();

/*Function that generates the neutral network given by the tree*/
void generate_graph();

