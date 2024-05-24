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


void insert_node(binary_heap_pq_t *heap, const char *element, int weight);
void delete_node(binary_heap_pq_t *heap, heap_pq_node_t **previous, heap_pq_node_t **current);
void remove_top_node(binary_heap_pq_t *heap, heap_pq_node_t **topNode);
void destroy_heap_priority_queue(binary_heap_pq_t *heap);


void print_heap(binary_heap_pq_t *heap);




#endif