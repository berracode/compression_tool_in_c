#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 7575
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Crear el socket del cliente
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    // Configurar la estructura sockaddr_in del servidor
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convertir dirección IP a binario
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    // Leer el mensaje de bienvenida del servidor
    read(client_socket, buffer, MAX_BUFFER_SIZE);
    printf("%s\n", buffer);

    FILE *received_file = fopen("./client_storage/received_file.txt", "w+"); //w es para escribir y el + para que lo cree si no existe
    if (received_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_received;
    while ((bytes_received = read(client_socket, buffer, MAX_BUFFER_SIZE)) > 0) {
        printf("He recibido: \n %.*s\n", (int)bytes_received, buffer);
        fwrite(buffer, 1, bytes_received, received_file);
    }

    fclose(received_file);
    close(client_socket);

    return 0;
}
