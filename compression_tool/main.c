#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help(char *argv[]){
    printf("\n");
    printf("usage: %s <COMMAND> FILE_PATH\n\n", argv[0]);
    printf("Commands:\n");
    printf("    compress        compress data\n");
    printf("    decompress      decompress data\n");
    printf("    help\n\n");

    printf("Options:\n\n");
    printf("    -h, --help      Print help\n\n");
    printf("    -v, --version   Print version\n");

}

void compress_file(char *file_path){
    char line[1024];
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        printf("Error al abrir el archivo de entrada: %s\n", file_path);
        return;
    }

    // Leer línea por línea hasta el final del archivo
    while (fgets(line, sizeof(line), file)) {
        // Imprimir la línea leída en la consola
        printf("%s", line);
    }

    // Cerrar el archivo
    fclose(file);

}



int main(int argc, char *argv[]) {

    if (argc < 3) {
        if(argc ==2 && (strcmp(argv[1], "-v")==0 ||strcmp(argv[1], "--version")==0)){
            printf("%s version 0.0.1, build xxx\n", argv[0]);
            return 1;
        }
        help(argv);
        return 1;
    }

    char *command = argv[1];
    char *file_path = argv[2];

    if(strcmp(command, "compress")==0){
        compress_file(file_path);
    } else if (strcmp(command, "decompress")==0){
        printf("Not implemented yet\n");
    } else {
        help(argv);
    }

}

