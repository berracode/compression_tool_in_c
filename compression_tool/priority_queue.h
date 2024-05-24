#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct huff_data_t {
    const char *element;
    int weight;
} huff_data_t;

typedef struct huff_node_t {
    struct huff_data_t *data;
    struct huff_node_t *next;
} huff_node_t;



/*
+------------------+
| PriorityQueue    |
+------------------+
| head: ---------->|
|    +------------------+    +------------------+    +------------------+
|    | HuffNode         |    | HuffNode         |    | HuffNode         |
|    +------------------+    +------------------+    +------------------+
|    | data: ---------->-----| data: ---------->-----| data: ----------> NULL
|    |    +------------------+|    +------------------+|    +------------------+
|    |    | HuffData         ||    | HuffData         ||    | HuffData         |
|    |    +------------------+|    +------------------+|    +------------------+
|    |    | element: "..."   ||    | element: "..."   ||    | element: "..."   |
|    |    | weight: ...      ||    | weight: ...      ||    | weight: ...      |
|    |    +------------------+|    +------------------+|    +------------------+
|    |                      ||                      ||                      |
|    | next: -------------->||    | next: ---------->||    | next: ----------> NULL
|    |                      ||                      ||                      |
|    +----------------------++----------------------++----------------------+
|
| size: ...
+------------------+
*/
typedef struct priority_queue_t {
    huff_node_t *head;
    int size;
} priority_queue_t;

priority_queue_t *create_priority_queue();
int is_empty(priority_queue_t *queue);

void insert_node(priority_queue_t *queue, const char *element, int weight);
void delete_node(priority_queue_t *queue, huff_node_t **previous, huff_node_t **current);
void remove_top_node(priority_queue_t *queue, huff_node_t **topNode);
void destroy_priority_queue(priority_queue_t *queue);

void print_huffnode(huff_node_t *node);
void print_queue(priority_queue_t *queue);



#endif

