#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ktmem.h"
#include "heap_priority_queue.h"

binary_heap_pq_t* create_binary_heap_pq(){
    binary_heap_pq_t *heap = (binary_heap_pq_t *)ktmalloc(sizeof(binary_heap_pq_t));
    if (!heap) {
        return NULL;
    }
    heap->head = NULL;
    heap->size = 0;

    return heap;

}

heap_pq_node_t* create_new_node(const char *element, int weight){
    heap_pq_node_t *temp = (heap_pq_node_t*)ktmalloc(sizeof(heap_pq_node_t));
    tree_node_t *tree_node = (tree_node_t*)ktmalloc(sizeof(tree_node_t));
    huff_data_t *data = (huff_data_t*)ktmalloc(sizeof(huff_data_t));

    data->element = element;
    data->weight = weight;
    tree_node->data = data;
    tree_node->left = NULL;
    tree_node->right = NULL;
    temp->tree_node = tree_node;
    temp->next = NULL;

    return temp;
}

int is_empty(binary_heap_pq_t *heap){
    return heap->size == 0;
}

void print_heap_pq_node(heap_pq_node_t *node) {
    if (node != NULL && node->tree_node != NULL && node->tree_node->data != NULL) {
        printf("#       Element: %s | Weight: %d\n", node->tree_node->data->element, node->tree_node->data->weight);
        if(node->tree_node->left != NULL){
            printf("# LF    Element: %s | Weight: %d\n", node->tree_node->left->data->element, node->tree_node->left->data->weight);
        }
        if(node->tree_node->right){
            printf("# RT    Element: %s | Weight: %d\n", node->tree_node->right->data->element, node->tree_node->right->data->weight);
        }

    }
}


void insert_heap_pq_node(binary_heap_pq_t *heap, heap_pq_node_t *new_node){
    if(is_empty(heap)){
        heap->head = new_node;
    } else {
        printf("aqui 4\n");

        heap_pq_node_t *current = heap->head;
        heap_pq_node_t *previous = NULL;
        print_heap_pq_node(current);

        while (current && current->tree_node->data->weight < new_node->tree_node->data->weight){
            previous = current;
            current = current->next;
        }
        
         if (previous == NULL) {
            printf("aqui P == NULL\n");

            new_node->next = heap->head;
            printf("aqui 4\n");

            heap->head = new_node;
                    printf("aqui 5\n");

        } else {
            printf("aqui P NOT NULL\n");

            previous->next = new_node;
            new_node->next = current;
        }

    }

    heap->size++;
    printf("aqui 6\n");

}

void insert_node(binary_heap_pq_t *heap, const char *element, int weight){
    heap_pq_node_t *new_node = create_new_node(element, weight);
    if(is_empty(heap)){
        heap->head = new_node;
    } else {
        heap_pq_node_t *current = heap->head;
        heap_pq_node_t *previous = NULL;

        while (current && current->tree_node->data->weight < new_node->tree_node->data->weight){
            previous = current;
            current = current->next;
        }
        
         if (previous == NULL) {
            new_node->next = heap->head;
            heap->head = new_node;
        } else {
            previous->next = new_node;
            new_node->next = current;
        }

    }

    heap->size++;
}

void delete_node(binary_heap_pq_t *heap, heap_pq_node_t **previous, heap_pq_node_t **current){
     if (current && (*current)->next){
        (*previous)->next = (*current)->next;
    } else {
        (*previous)->next = NULL;
    }
    ktfree((*current)->tree_node->data);
    ktfree((*current)->tree_node);
    ktfree(*current);
    heap->size--;
}

void remove_top_node(binary_heap_pq_t *heap, heap_pq_node_t **topNode){
    if (is_empty(heap)) {
        return;
    }
    *topNode = heap->head;
    heap->head = heap->head->next;
    heap->size--;
}

void destroy_heap_priority_queue(binary_heap_pq_t *heap){
    heap_pq_node_t *current = heap->head;
    printf("en la linea de arriba da el seg fault\n");
    while (current) {
        printf("en el while\n");

        heap_pq_node_t *next = current->next;
        printf("en el while 2\n");
        printf("addr wh : %p\n", current);

        ktfree(current->tree_node->data);
                printf("en el while 3\n");

        ktfree(current->tree_node);
                printf("en el while 4\n");

        ktfree(current);
        current = next;
    }
    printf("no llega aqui\n");
    ktfree(heap);
}


void print_heap(binary_heap_pq_t *heap){
    heap_pq_node_t *current = heap->head;
    printf("------------------PRINT FULL HEAP\n");
    while (current != NULL) {
        print_heap_pq_node(current);
        //printf("$ Element: %s - bytes: %lu | Weight: %d\n", current->tree_node->data->element,strlen(current->tree_node->data->element), current->tree_node->data->weight);
        current = current->next;
    }
}