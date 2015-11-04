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



/*Function that prints the adjacency list*/
void print_list(void);

/*Function that clears an adjacency list BUT THE (0,1) and (0,2) edges*/
void clear_list(void);

/*Function that generates the neutral network graph from the tree*/
void generate_graph(void);

/*Function that verifies if the sequance is valid or not*/
/*void check_seq(int *seq);*/

/*Function that attributes zeros to the nodes, in a depth search*/
void check_node(int node);

/*Function used to delete repeated in adjacency list*/
void verify_node(int node);

/*Function used to generate combinations of connections to super node*/
void recursive_list(int index);

