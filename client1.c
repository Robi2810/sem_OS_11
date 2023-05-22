#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP address> <port>\n", argv[0]);
        exit(1);
    }

    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Создаем сокет клиента №1
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Failed to create client socket");
        exit(1);
    }

    // Настраиваем адрес сервера
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    // Подключаемся к серверу
    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Failed to connect to server");
        exit(1);
    }
    printf("Connected to server\n");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Отправляем сообщение серверу
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Failed to send message to server");
            exit(1);
        }

        // Если сообщение равно "The End", завершаем работу клиента №1
        if (strcmp(buffer, "The End") == 0) {
            printf("Terminating...\n");
            break;
        }
    }

    // Закрываем сокет
    close(client_socket);

    return 0;
}

