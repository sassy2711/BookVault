#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "user_book.h"
#include <pthread.h>
#include "operations.h"

#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

// Define mutexes for each shared file
pthread_mutex_t user_book_db_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t book_db_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t user_db_mutex = PTHREAD_MUTEX_INITIALIZER;



FILE *user_db;
FILE *book_db;
FILE *user_book_db;
// Function to handle client connection

char user[30] = "user";
char book[30] = "book";
int rec_size = sizeof(struct User);
int linked_rec_size = sizeof(struct Book);

void user_authentication(FILE *user_db, int client_socket)
{
    user_db = fopen("user_db.bin", "rb+");
    user_book_db = fopen("user_book_db.bin", "rb+");
    book_db = fopen("book_db.bin", "rb+");
    int signed_up = 0;
    int signed_in = 0;
    char buffer[1024];
    int n;
    int command;
    n = recv(client_socket, buffer, 1023, MSG_WAITALL);
    //printf("%s\n", buffer);
    if (n == -1)
    {
        perror("recv");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (strcmp(buffer, "A") == 0)
        {
            //book_db = fopen("book_db.bin", "rb+");
            //printf("1\n");
            recv(client_socket, buffer, 1023, 0);
            if (strcmp(buffer, ADMINISTRATOR_PASS) == 0)
            {
                //printf("2\n");
                send(client_socket, "You have logged in as administrator!", 1023, 0);
                recv(client_socket, &command, sizeof(int), 0);
                    while(command != 0){
                        //printf("%d", command);
                        if(command == 1){
                            struct Book book;
                            recv(client_socket, &book, sizeof(struct Book), 0);
                            pthread_mutex_lock(&book_db_mutex);
                            add_book(book_db, &book);
                            pthread_mutex_unlock(&book_db_mutex);
                            sprintf(buffer, "Book added successfully with book_id %d.\n", book.book_id);
                            send(client_socket, buffer, 1023, 0);
                            //add book
                        }
                        else if(command == 2){
                            pthread_mutex_lock(&book_db_mutex);
                            show_all_books(book_db, client_socket);
                            pthread_mutex_unlock(&book_db_mutex);
                            //show all books
                            
                        }
                        else if(command == 3){
                            int book_id;
                            int new_copies_available;
                            recv(client_socket, &book_id, sizeof(int), 0);
                            recv(client_socket, &new_copies_available, sizeof(int), 0);
                            pthread_mutex_lock(&book_db_mutex);
                            set_copies_available(book_db, book_id, new_copies_available);
                            pthread_mutex_unlock(&book_db_mutex);
                            send(client_socket, "Book modified successfully.\n", 1023, 0);
                            //modify book
                            
                        }
                        else if(command == 4){
                            int book_id;
                            recv(client_socket, &book_id, sizeof(int), 0);
                            pthread_mutex_lock(&book_db_mutex);
                            delete_book(book_db, book_id);
                            pthread_mutex_unlock(&book_db_mutex);
                            send(client_socket, "Book deleted successfully.\n", 1023, 0);
                            //delete book                            
                        }
                        recv(client_socket, &command, sizeof(int), 0);
                    }
                    close(client_socket);
            }
            else
            {
                //printf("3\n");
                send(client_socket, "Wrong admin pass.", 1023, 0);
            }
        }
        else if (strcmp(buffer, "U") == 0)
        {   
            
            recv(client_socket, buffer, 1023, 0);
            if (strcmp(buffer, "1") == 0)
            {
                pthread_mutex_lock(&user_db_mutex);
                sign_up_user(user_db, client_socket);
                pthread_mutex_unlock(&user_db_mutex);
                close(client_socket);
            }
            else if (strcmp(buffer, "2") == 0)
            {
                pthread_mutex_lock(&user_db_mutex);
                signed_in = !sign_in_user(user_db, client_socket);
                pthread_mutex_unlock(&user_db_mutex);
                //printf("signed in: %d\n", signed_in);
                if (signed_in)
                {
                    //printf("command: %d\n", command);
                    recv(client_socket, &command, sizeof(int), 0);
                    while(command != 0){
                        if(command == 1){
                            struct Borrowed borrowed;
                            //printf("hi\n");
                            recv(client_socket, &borrowed, sizeof(struct Borrowed), 0);
                            //printf("hi2\n");
                            pthread_mutex_lock(&user_book_db_mutex);
                            pthread_mutex_lock(&book_db_mutex);
                            int status = borrow_book(user_book_db, book_db, &borrowed);
                            pthread_mutex_unlock(&user_book_db_mutex);
                            pthread_mutex_unlock(&book_db_mutex);
                            //printf("status: %d\n", status);
                            if(status == 0){
                                sprintf(buffer, "Book borrowed successfully with transaction_id %d.\n", borrowed.transaction_id);
                                send(client_socket, buffer, 1023, 0);
                            }
                            else if(status == 1){
                                struct Book book;
                                pthread_mutex_lock(&book_db_mutex);
                                search_book(book_db, borrowed.book_id, &book);
                                pthread_mutex_unlock(&book_db_mutex);
                                sprintf(buffer, "That many copies are not available. Currently %d copies are available.\n", book.copies_available);
                                send(client_socket, buffer, 1023, 0);
                            }
                            else if(status == -1){
                                send(client_socket, "Book with this book_id does not exist.\n", 1023, 0);
                            }
                        }
                        else if(command == 2){
                            int user_id, book_id, transaction_id;
                            recv(client_socket, &user_id, sizeof(int), 0);
                            recv(client_socket, &book_id, sizeof(int), 0);
                            recv(transaction_id, &transaction_id, sizeof(int), 0);
                            struct Borrowed borrowed;
                            pthread_mutex_lock(&user_book_db_mutex);
                            pthread_mutex_lock(&book_db_mutex);
                            if(return_book(user_book_db, book_db, user_id, book_id, transaction_id) == 0){
                                send(client_socket, "Book returned successfully.\n", 1023, 0);
                                //send success
                            }
                            else{
                                send(client_socket, "The user_id, book_id or transaction_id is wrong.\n", 1023, 0);
                                //send failure
                            }
                            pthread_mutex_unlock(&user_book_db_mutex);
                            pthread_mutex_unlock(&book_db_mutex);
                            
                        }
                        recv(client_socket, &command, sizeof(int), 0);
                    }
                    close(client_socket);
                }
                else
                {
                    close(client_socket);
                }
            }
        }
    }
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

    int status;
    
    // write_rec_count(0);
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

        // Fork a new process to handle the client
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            close(client_socket);
            close(server_socket);
            return EXIT_FAILURE;
        }
        else if (pid == 0)
        {
            // Child process: handle the client connection
            close(server_socket); // Child doesn't need the server socket
            // handle_client(client_socket);

            user_authentication(user_db, client_socket);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process: close the client socket and continue
            close(client_socket);
            // Optionally, you may want to use `waitpid()` to reap the child processes and prevent zombies
        }
    }

    fclose(user_db);
    if (status != 0)
        printf("Error occured with status: %d\n", status);
    // Close the server socket (unreachable due to the infinite loop)
    close(server_socket);
    return EXIT_SUCCESS;
}

// telnet localhost 8080 --write this in new terminal
