#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 7575
#define MAX_BUFFER_SIZE 1024
#define FILE_PATH "./server_storage/file_in_server2" // Ruta al archivo binario que quieres enviar


int main() {
    FILE *file;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};
    char *message = "Hola cliente... te vamos a mandar un archivo:\n";

    file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Crear el socket del servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configurar la estructura sockaddr_in
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Asociar el socket a la dirección y al puerto especificados
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Escuchar por conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Aceptar la conexión entrante
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Enviar el contenido del archivo al cliente
    send(new_socket, message, strlen(message), 0);
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, file);
        printf("\n##### LEYENDO BLOQUE #####\n");

        //printf("se leyeron %d bytes\n", (int) bytes_read);
        if (bytes_read > 0) {
            printf("enviando: \n%.*s", (int)bytes_read, buffer); // Imprimir el buffer completo
            send(new_socket, buffer, bytes_read, 0); // Enviar el buffer al cliente
        }
    }


    printf("\nArchivo enviado al cliente.\n");

    fclose(file);
    close(new_socket);
    close(server_fd);
    return 0;
}
