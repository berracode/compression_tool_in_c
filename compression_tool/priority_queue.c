#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

PriorityQueue *create_priority_queue() {
    PriorityQueue *queue = malloc(sizeof(PriorityQueue));
    if (!queue) {
        return NULL;
    }
    queue->head = NULL;
    queue->size = 0;

    return queue;
}

void print_huffnode(HuffNode *node) {
    if (node != NULL && node->data != NULL) {
        printf("/////////// Element: %c, Weight: %d\n", node->data->element, node->data->weight);
    }
}

int is_empty(PriorityQueue *queue) {
  return queue->size == 0;
}

/**
 * @brief create a new HuffNode
 * 
 * @param element 
 * @param weight is the priority
 * @return HuffNode* 
 */
HuffNode* new_node(char element, int weight){
    HuffNode* temp = (HuffNode*)malloc(sizeof(HuffNode));
    HuffData* data = (HuffData*)malloc(sizeof(HuffData));

    data->element = element;
    data->weight = weight;
    temp->data = data;
    temp->next = NULL;

    return temp;
}

void insert_node(PriorityQueue *queue, char element, int weight) {

    HuffNode *newNode = new_node(element, weight);
    //printf("newNode ");
    //print_huffnode(newNode);

    if (is_empty(queue)) {
        queue->head = newNode;
    } else {
        HuffNode *current = queue->head;
        HuffNode *previous = NULL;
        //printf("current inser ");
        //print_huffnode(current);
        while (current && current->data->weight < newNode->data->weight) {
            previous = current;
            current = current->next;
        }

        if (previous == NULL) {
            newNode->next = queue->head;
            queue->head = newNode;
        } else {
            previous->next = newNode;
            newNode->next = current;
        }

    }

    queue->size++;
}

void remove_top_node(PriorityQueue *queue, HuffNode **topNode) {
    //printf("q size %d\n", queue->size);
    if (is_empty(queue)) {
        return;
    }
    *topNode = queue->head;
    queue->head = queue->head->next;
    queue->size--;

}

void delete_node(PriorityQueue *queue, HuffNode **previous, HuffNode **current){
    if (current && (*current)->next){
        (*previous)->next = (*current)->next;
    } else {
        (*previous)->next = NULL;
    }
    free(*current);
    queue->size--;
}

void destroy_priority_queue(PriorityQueue *queue) {
    HuffNode *current = queue->head;
    while (current) {
        HuffNode *next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}

void print_queue(PriorityQueue *queue) {
    HuffNode *current = queue->head;
    while (current != NULL) {
        printf("# Element: %c, Weight: %d\n", current->data->element, current->data->weight);
        current = current->next;
    }
}