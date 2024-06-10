#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ktmem.h"
#include "priority_queue.h"

priority_queue_t *create_priority_queue() {
    priority_queue_t *queue = (priority_queue_t *)ktmalloc(sizeof(priority_queue_t));
    if (!queue) {
        return NULL;
    }
    queue->head = NULL;
    queue->size = 0;

    return queue;
}

void print_huffnode(huff_node_t *node) {
    if (node != NULL && node->data != NULL) {
        printf("Element: %s, Weight: %d\n", node->data->element, node->data->weight);
    }
}

int is_empty(priority_queue_t *queue) {
  return queue->size == 0;
}

/**
 * @brief create a new huff_node_t
 * 
 * @param element 
 * @param weight is the priority
 * @return HuffNode* 
 */
huff_node_t* new_node(const char *element, int weight){
    huff_node_t *temp = (huff_node_t*)ktmalloc(sizeof(huff_node_t));
    huff_data_t *data = (huff_data_t*)ktmalloc(sizeof(huff_data_t));

    data->element = element;
    data->weight = weight;
    temp->data = data;
    temp->next = NULL;

    return temp;
}

void insert_node(priority_queue_t *queue, const char *element, int weight) {

    huff_node_t *newNode = new_node(element, weight);
    //printf("newNode ");
    //print_huffnode(newNode);

    if (is_empty(queue)) {
        queue->head = newNode;
    } else {
        huff_node_t *current = queue->head;
        huff_node_t *previous = NULL;
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

void remove_top_node(priority_queue_t *queue, huff_node_t **topNode) {
    //printf("q size %d\n", queue->size);
    if (is_empty(queue)) {
        return;
    }
    *topNode = queue->head;
    queue->head = queue->head->next;
    queue->size--;

}

void delete_node(priority_queue_t *queue, huff_node_t **previous, huff_node_t **current){
    if (current && (*current)->next){
        (*previous)->next = (*current)->next;
    } else {
        (*previous)->next = NULL;
    }
    ktfree((*current)->data);
    ktfree(*current);
    queue->size--;
}

void destroy_priority_queue(priority_queue_t *queue) {
    huff_node_t *current = queue->head;
    while (current) {
        huff_node_t *next = current->next;
        ktfree(current);
        current = next;
    }
    ktfree(queue);
}

void print_queue(priority_queue_t *queue) {
    huff_node_t *current = queue->head;
    while (current != NULL) {
        printf("# Element: %s - %lu, Weight: %d\n", current->data->element,strlen(current->data->element), current->data->weight);
        current = current->next;
    }
}