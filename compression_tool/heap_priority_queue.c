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

void free_tree(tree_node_t *node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);
    if (node->data != NULL) {
        if(node->data->element !=NULL) {
            printf("Tree: %s\n", node->data->element);
            ktfree((void *)node->data->element);
        }
        ktfree(node->data);

    }
    ktfree(node); //mismo node
}

void free_heap_pq(binary_heap_pq_t *heap) {
    heap_pq_node_t *current = heap->head;
    while (current != NULL) {
        heap_pq_node_t *next = current->next;
        free_tree(current->tree_node);
        ktfree(current);
        current = next;
    }
    ktfree(heap);
}

void traverse_list_and_trees(binary_heap_pq_t *heap, void (*traverse_func)(tree_node_t *)) {
    heap_pq_node_t *current = heap->head;
    while (current != NULL) {
        printf("Traversing a tree in the list:\n");
        traverse_func(current->tree_node);
        current = current->next;
    }
}

void print_heap_post_order(tree_node_t *root) {
    if (root != NULL) {
        print_heap_post_order(root->left);
        print_heap_post_order(root->right);
        if(root->data->element!=NULL){
            printf("Element: %s, Weight: %d\n", root->data->element, root->data->weight);
        }
    }
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

int* increase(int *counter){
    *counter = *counter + 1;
    return counter;
}

void visit_tree_node_post_order(tree_node_t *node, int *edges, int *count_left, int *count_right, int is_left, char *code) {
    printf("#######################################\n");
    if (node == NULL) {
        printf("path nullo para la %s\n", (is_left?"izquierda":"derecha"));

        if(is_left){
            *count_left = *count_left -1;

        } else {
            *count_right = *count_right -1;
        }
        return;
    }
    printf("entra a %s con peso %d: en la %s\n", node->data->element, node->data->weight, (is_left?"izquierda":"derecha"));
    printf("code: %s\n", code);
    printf("CL: %d\n", *count_left);
    printf("CR: %d\n", *count_right);

    *edges = *count_left+(*count_right);
    printf("edges: %d\n", *edges);
    if(*edges > 0){
        //reservar memoria
        int len = *edges;
        if(code == NULL || len == 1){
            code = (char*)ktmalloc(len + 1);
        } else {
            printf("QQQQQQQQQ AMPLIANDO VECTOR code: %s | %lu\n", code, strlen(code));
            char *new_code = (char*)ktmalloc(len + 1);
            strcpy(new_code, code);
            code = (char*)ktmalloc(len + 1);
            strcpy(code, new_code);
            printf("new code sin modificar: %s\n", new_code);
            //ktfree(code);
            //code = new_code;
        }

        if(is_left){
            code[len-1] = '0';
        }else{
            code[len-1] = '1';
        }

        printf("vector %s de %lu BYTES\n",code, strlen(code));
    }

    visit_tree_node_post_order(node->left, edges, increase(count_left), count_right, 1, code);
    printf("Antes de ir a la derecha: %s\n", code);
    visit_tree_node_post_order(node->right, edges, count_left, increase(count_right), 0, code);
    if (node->data != NULL) {
        if(node->data->element !=NULL) {
            //*edges = *edges +1;
            printf("sum: %d\n", *edges);
            printf("hoja: %s a la %s | code: %s\n", node->data->element, (is_left?"izquierda":"derecha"), code);
            //ktfree((void *)node->data->element);
            //code = NULL;
            *edges = 0;
            if(*count_left>=1){
                *count_left = *count_left -1;
            }
            if(*count_right>=1 && !is_left){
                *count_right = *count_right -1;
            }
            printf("FINAL CODE EN HOJA: %s\n", code);
            printf("HOJA CL: %d\n", *count_left);
            printf("HOJA CR: %d\n", *count_right);
            printf("--------------------\n\n");

        }
        //ktfree(node->data);

    }
    //ktfree(node); //mismo node
}

void build_prefix_code_table(binary_heap_pq_t *heap){
    /*
    recorrer hasta las hojas
    si es un nodo a la izquierda ponerle 0
    */
   heap_pq_node_t *head = heap->head;
   int i = 0, count_left = 0, count_right = 0;
   char *code;
   visit_tree_node_post_order(head->tree_node, &i, &count_left, &count_right, 1, code);
   ktfree(code);
}