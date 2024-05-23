#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "priority_queue.h"

void help(char *argv){
    printf("\n");
    printf("usage: %s <COMMAND> FILE_PATH\n\n", argv);
    printf("Commands:\n");
    printf("    compress        compress data\n");
    printf("    decompress      decompress data\n");
    printf("    help\n\n");

    printf("Options:\n\n");
    printf("    -h, --help      Print help\n");
    printf("    -v, --version   Print version\n");

}


long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    printf("size file: %ld\n", size);
    fseek(file, 0, SEEK_SET);
    fclose(file);
    return size;
}

size_t choose_buffer_size(long file_size) {

    if (file_size > 512 && file_size <= 1024) {
        return 1024;
    } else if (file_size > 1024 && file_size <= 10240) { // 1024 * 10
        return 2048;
    } else if (file_size > 10240){
        return 6144; // 1024 * 6
    } else {
         return file_size;
    }
}

void read_file_with_buffer(const char *filename) {
    long file_size = get_file_size(filename);
    size_t buffer_size = choose_buffer_size(file_size);
    printf("buffer_size: %ld\n", buffer_size);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        perror("Can't set memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t bytes_read;
    PriorityQueue *queue = create_priority_queue();


    while ((bytes_read = fread(buffer, 1, buffer_size, file)) > 0) {
        printf("Bytes readed: %ld\n", bytes_read);
        // Procesar el buffer aquí
        /*
        1. iterar sobre el buffer caracter a caracter
        2. si la cola esta vacia, agregar el caractere
        2.1 si no esta vacia, validar (iterar la cola) si el caracter que estoy
        probando ya esta en la cola
        2.1.1 si no esta, agregarlo, 
        2.1.2 si ya esta, sumarle uno al valor que diga el peso
        3. repetir
        */
        for (int i = 0; i < bytes_read; i++){
            if(is_empty(queue)){
                insert_node(queue, buffer[i], 1);
            } else {

                if (!iscntrl(buffer[i])){
                    //printf("CHAR = %d\n", buffer[i]);
                    HuffNode *current = queue->head;
                    HuffNode *previuos = NULL;

                    short is_present = 0;
                    while (current && !is_present) {
                        char element = current->data->element;
                        int weight = current->data->weight;
                        
                        
                           // printf("element %c, bufferi %c,  %d\n", element, buffer[i], (tolower(element) == tolower(buffer[i])) || element == buffer[i]);
                        
                        if((tolower(element) == tolower(buffer[i])) || element == buffer[i]) {
                            is_present = 1;
                            weight = weight + 1;

                            /*
                            si esta presente, debo obtener el valor del peso
                            2. eliminarlo, y volverlo a insertar pero con el nuevo valor del peso. 
                            */
                            if(previuos == NULL) { //is the first node
                                HuffNode *topNode;
                                remove_top_node(queue, &topNode);
                                insert_node(queue, element, weight);
                                free(topNode);

                            } else {
                                //printf("previuos while ");
                                //print_huffnode(previuos);
                                //printf("current while ");
                                //print_huffnode(current);
                                delete_node(queue, &previuos, &current);
                                insert_node(queue, element, weight);
                                //free(current);
                            }
                            break;
                        }
                        previuos = current;
                        current = current->next;
                    }

                    if(!is_present){
                        insert_node(queue, buffer[i], 1);
                    }
                    //printf("------S_------\n");
                    //print_queue(queue);
                    //printf("------E_------\n");


                }

            }
        }
    }

    print_queue(queue);

    HuffNode *topNode;
    printf("---------------inciia destrucción---------------------\n");
    
    while (!is_empty(queue)) {
        remove_top_node(queue, &topNode);
        //printf("aqui para: \n");
        //printf("%c: %d\n", topNode->data->element, topNode->data->weight);
        //printf("aqui para: \n");

        free(topNode);
    }
    printf("Destruccion\n");

    destroy_priority_queue(queue);

    free(buffer);
    fclose(file);
}

void compress_file(char *file_path){

    read_file_with_buffer(file_path);

}



int main(int argc, char *argv[]) {

    if (argc < 3) {
        if(argc ==2 && (strcmp(argv[1], "-v")==0 ||strcmp(argv[1], "--version")==0)){
            printf("%s version 0.0.1, build xxx\n", argv[0]);
            return 1;
        }
        help(argv[0]);
        return 1;
    }

    char *command = argv[1];
    char *file_path = argv[2];

    if(strcmp(command, "compress")==0){
        compress_file(file_path);
    } else if (strcmp(command, "decompress")==0){
        printf("Not implemented yet\n");
    } else {
        help(argv[0]);
    }

}

