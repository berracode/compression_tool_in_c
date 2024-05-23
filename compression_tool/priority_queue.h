#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct HuffData {
    const char *element;
    int weight;
} HuffData;

typedef struct HuffNode {
    struct HuffData *data;
    struct HuffNode *next;
} HuffNode;



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
typedef struct PriorityQueue {
    HuffNode *head;
    int size;
} PriorityQueue;

PriorityQueue *create_priority_queue();
int is_empty(PriorityQueue *queue);

void insert_node(PriorityQueue *queue, const char *element, int weight);
void delete_node(PriorityQueue *queue, HuffNode **previous, HuffNode **current);
void remove_top_node(PriorityQueue *queue, HuffNode **topNode);
void destroy_priority_queue(PriorityQueue *queue);

void print_huffnode(HuffNode *node);
void print_queue(PriorityQueue *queue);



#endif

