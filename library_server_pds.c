#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "database/user_book.h"
#include "database/pds.h"
#include <pthread.h>

// #define PORT 8080
// #define BACKLOG 5
// #define BUFFER_SIZE 1024

// // Function to handle client connection

// char user[30] = "user";
// char book[30] = "book";
// int rec_size = sizeof(struct User);
// int linked_rec_size = sizeof(struct Book);

// void user_authentication(int client_socket)
// {
//     char command[1];
//     int n;
//     n = recv(client_socket, command, sizeof(command), MSG_WAITALL);
//     if (n == -1)
//     {
//         perror("recv");
//         close(client_socket);
//         exit(EXIT_FAILURE);
//     }
//     else
//     {
//         if (strcmp(command, "1") == 0)
//         {
//             if (sign_up_user(client_socket) == 0)
//             {
//                 close(client_socket);
//             }
//         }
//         else if (strcmp(command, "2") == 0)
//         {
//             if (sign_in_user(client_socket) == 0)
//             {
//                 close(client_socket);
//             }
//         }
//     }
// }

// int main() {
//     int server_socket, client_socket;
//     struct sockaddr_in server_addr, client_addr;
//     socklen_t client_len = sizeof(client_addr);

//     // Create a TCP server socket
//     server_socket = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_socket == -1) {
//         perror("socket");
//         return EXIT_FAILURE;
//     }

//     // Configure the server address
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     server_addr.sin_port = htons(PORT);

//     // Bind the server socket to the specified port
//     if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
//         perror("bind");
//         close(server_socket);
//         return EXIT_FAILURE;
//     }

//     // Listen for incoming connections
//     if (listen(server_socket, BACKLOG) == -1) {
//         perror("listen");
//         close(server_socket);
//         return EXIT_FAILURE;
//     }

//     printf("Server listening on port %d...\n", PORT);

//     int status;
//     status = pds_create(user, book);
//     if(status !=0) printf("Error occured with status: %d\n", status);
//     status = pds_open(user, book, rec_size, linked_rec_size);
//     if(status !=0) printf("Error occured with status: %d\n", status);
//     write_rec_count(0);
//     while (1) {
//         // Accept a client connection
//         client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
//         if (client_socket == -1) {
//             perror("accept");
//             close(server_socket);
//             return EXIT_FAILURE;
//         }

//         // Fork a new process to handle the client
//         pid_t pid = fork();
//         if (pid == -1) {
//             perror("fork");
//             close(client_socket);
//             close(server_socket);
//             return EXIT_FAILURE;
//         } else if (pid == 0) {
//             // Child process: handle the client connection
//             close(server_socket); // Child doesn't need the server socket
//             //handle_client(client_socket);
//             user_authentication(client_socket);
//             exit(EXIT_SUCCESS);
//         } else {
//             // Parent process: close the client socket and continue
//             close(client_socket);
//             // Optionally, you may want to use `waitpid()` to reap the child processes and prevent zombies
//         }
//     }

//     status = pds_close();
//     if(status !=0) printf("Error occured with status: %d\n", status);
//     // Close the server socket (unreachable due to the infinite loop)
//     close(server_socket);
//     return EXIT_SUCCESS;
// }

// // telnet localhost 8080 --write this in new terminal



#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

// Function to handle client connection
void *handle_client(void *arg)
{
    int client_socket = *(int*)arg;
    char command[1];
    int n;
    n = recv(client_socket, command, sizeof(command), MSG_WAITALL);
    if (n == -1)
    {
        perror("recv");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (strcmp(command, "1") == 0)
        {
            if (sign_up_user(client_socket) == 0)
            {
                close(client_socket);
            }
        }
        else if (strcmp(command, "2") == 0)
        {
            if (sign_in_user(client_socket) == 0)
            {
                close(client_socket);
            }
        }
    }
    pthread_exit(NULL);
    // int client_socket = *(int *)arg;
    // char buffer[BUFFER_SIZE];
    // int n;

    // // Receive data from the client
    // n = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    // if (n == -1)
    // {
    //     perror("recv");
    //     close(client_socket);
    //     pthread_exit(NULL);
    // }

    // // Null-terminate the received data
    // buffer[n] = '\0';

    // // Print the received data
    // printf("Received from client: %s\n", buffer);

    // // Send a response to the client
    // const char *response = "Hello from server!";
    // send(client_socket, response, strlen(response), 0);

    // // Close the client socket
    // close(client_socket);

    // // Exit the thread
    // pthread_exit(NULL);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create a TCP server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the server socket to the specified port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_socket);
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(server_socket, BACKLOG) == -1)
    {
        perror("listen");
        close(server_socket);
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1)
        {
            perror("accept");
            close(server_socket);
            return EXIT_FAILURE;
        }

        // Create a thread to handle the client connection
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, &client_socket) != 0)
        {
            perror("pthread_create");
            close(client_socket);
            close(server_socket);
            return EXIT_FAILURE;
        }

        // Detach the thread so it cleans up its resources automatically upon exit
        pthread_detach(thread_id);
    }

    // Close the server socket (unreachable due to the infinite loop)
    close(server_socket);
    return EXIT_SUCCESS;
}
