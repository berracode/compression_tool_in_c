#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct HuffData {
    char element;
    int weight;
} HuffData;

typedef struct HuffNode {
    struct HuffData *data;
    struct HuffNode *next;
} HuffNode;

typedef struct PriorityQueue {
    HuffNode *head;
    int size;
} PriorityQueue;

PriorityQueue *create_priority_queue();
int is_empty(PriorityQueue *queue);

void insert_node(PriorityQueue *queue, char element, int weight);
void remove_top_node(PriorityQueue *queue, HuffNode **topNode);
void destroy_priority_queue(PriorityQueue *queue);


#endif

