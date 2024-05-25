#ifndef HEAP_PRIORITY_QUEUE_H
#define HEAP_PRIORITY_QUEUE_H

#ifndef huff_data_t
#define HUFF_DATA_T huff_data_t
typedef struct huff_data_t {
    const char *element;
    int weight;
} huff_data_t;
#endif

typedef struct tree_node_t {
    struct huff_data_t *data;
    struct tree_node_t *left;
    struct tree_node_t *right;
} tree_node_t;

typedef struct heap_pq_node_t {
    struct tree_node_t *tree_node;
    struct heap_pq_node_t *next;
} heap_pq_node_t;


/*
binary_heap_pq_t
+---------------------+
| head                | ----> +--------------------+
|                     |       | heap_pq_node_t     |
| size                |       +--------------------+
+---------------------+       | tree_node          | ----> +------------------+
                              | next               |       | tree_node_t      |
                              +--------------------+       +------------------+
                                                      /    | data             |
                                                     /     | left             | ----> +------------------+
                                                    /      | right            |       | tree_node_t      |
                                                   /       +------------------+       +------------------+
                                                  /                                   | data             |
                                                 /                                    | left             |
                                                /                                     | right            |
                                               /                                      +------------------+
                                              /
                                             /
                                            /
                                           /
                              +--------------------+
                              | heap_pq_node_t     |
                              +--------------------+
                              | tree_node          | ----> +------------------+
                              | next               |       | tree_node_t      |
                              +--------------------+       +------------------+
                                                           | data             |
                                                           | left             | ----> +------------------+
                                                           | right            |       | tree_node_t      |
                                                           +------------------+       +------------------+
                                                                                      | data             |
                                                                                      | left             |
                                                                                      | right            |
                                                                                      +------------------+


*/
/**
 * binary heap priority queue
*/
typedef struct binary_heap_pq_t{
    heap_pq_node_t *head;
    int size;
} binary_heap_pq_t;


binary_heap_pq_t* create_binary_heap_pq();
int is_empty(binary_heap_pq_t *heap);

heap_pq_node_t* create_new_node(const char *element, int weight);
void insert_node(binary_heap_pq_t *heap, const char *element, int weight);
void insert_heap_pq_node(binary_heap_pq_t *heap, heap_pq_node_t *new_node);
void delete_node(binary_heap_pq_t *heap, heap_pq_node_t **previous, heap_pq_node_t **current);
void remove_top_node(binary_heap_pq_t *heap, heap_pq_node_t **topNode);
void destroy_heap_priority_queue(binary_heap_pq_t *heap);

void free_heap_pq(binary_heap_pq_t *heap);
void free_tree(tree_node_t *node);

void print_heap_pq_node(heap_pq_node_t *node);
void print_heap(binary_heap_pq_t *heap);
void build_prefix_code_table(binary_heap_pq_t *heap);

void traverse_list_and_trees(binary_heap_pq_t *heap, void (*traverse_func)(tree_node_t *));
void print_heap_post_order(tree_node_t *root);



#endif