// Client side C program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "operations.h"
#include "user_book.h"

#define PORT 8080

void admin_menu(){
    printf("\n\nAdministrator Menu:-\n");
    printf("Type a command to perform the corresponding action:\n");
    printf("\t0. Exit\n");
    printf("\t1. Add Book\n");
    printf("\t2. Show Books\n");
    printf("\t3. Update Book\n");
    printf("\t4. Remove Book\n\n");
    printf("Enter command: ");
}

void user_menu(){
    printf("\n\nUser Menu:-\n");
    printf("Type a command to perform the corresponding action:\n");
    printf("\t0. Exit\n");
    printf("\t1. Borrow Book\n");
    printf("\t2. Return Book\n");
    printf("Enter command: ");
}

int main(int argc, char *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }


    //actual stuff starts here

    //char buffer[1];
    int command;
    printf("Enter A to login as administrator.\nEnter U to enter authentication as a user.\n");
    scanf("%s", buffer);
    while (strcmp(buffer, "A") != 0 && strcmp(buffer, "U") != 0)
    {
        printf("Please enter a value that is either A or U.\n");
        scanf("%s", buffer);
    }
    send(sock, buffer, 1023, 0);
    if (strcmp(buffer, "A") == 0)
    {
        printf("Enter administrator password: ");
        scanf("%s", buffer);
        send(sock, buffer, 1023, 0);
        recv(sock, buffer, 1023, 0);
        if (strcmp(buffer, "You have logged in as administrator!") == 0)
        {
            printf("%s\n", buffer);
            // provide administrator menu
            admin_menu();
            scanf("%d", &command);
            while(command != 0){
                send(sock, &command, sizeof(int), 0);
                if(command == 1){
                    int book_count = read_book_count();
                    int copies_available;
                    char title[TITLE_MAX_SIZE];
                    char author[AUTHOR_MAX_SIZE];
                    printf("Enter title: ");
                    scanf("%s", title);
                    printf("Enter author: ");
                    scanf("%s", author);
                    printf("Enter number of copies available: ");
                    scanf("%d", &copies_available);
                    struct Book book;
                    strcpy(book.title,title);
                    strcpy(book.author,author);
                    book.book_id = book_count+1;
                    write_book_count(book.book_id);
                    book.copies_available = copies_available;
                    book.is_deleted = 0;
                    send(sock, &book, sizeof(struct Book), 0);
                    recv(sock, buffer, 1023, 0);
                    printf("%s", buffer);
                }
                else if(command == 2){
                    while(1){
                        recv(sock, buffer, 1023, 0);
                        if(strcmp(buffer, "Finished") != 0){
                            printf("%s", buffer);
                            //recv(sock, buffer, 1023, 0);
                        }
                        else{
                            break;
                        }
                    } 
                }
                else if(command ==  3){
                    int book_id;
                    int new_copies_available;
                    printf("Enter book_id: ");
                    scanf("%d", &book_id);
                    send(sock, &book_id, sizeof(int), 0);
                    printf("Enter new number of copies available: ");
                    scanf("%d", &new_copies_available);
                    send(sock, &new_copies_available, sizeof(int), 0);
                    recv(sock, buffer, 1023, 0);
                    printf("%s", buffer);
                }
                else if(command == 4){
                    int book_id;
                    printf("Enter book_id: ");
                    scanf("%d", &book_id);
                    send(sock, &book_id, sizeof(int), 0);
                    recv(sock, buffer, 1023, 0);
                    printf("%s", buffer);
                }
                admin_menu();
                scanf("%d", &command);
            }
            
        }
        else{
            printf("%s\n", buffer);
        }
    }
    else
    {
        printf("Enter 1 to signup and 2 to login.\n");

        scanf("%s", buffer);
        while (1)
        {
            // printf("%s\n", buffer);
            if (strcmp(buffer, "1") == 0 || strcmp(buffer, "2") == 0)
            {
                send(sock, buffer, 1023, 0);
                break;
            }
            else
            {
                printf("Please enter a value that is either 1(to signup) or 2(to login).\n");
                scanf("%s", buffer);
            }
        }
        int n;
        char username[USERNAME_MAX_SIZE] = {0};
        char password[PASSWORD_MAX_SIZE] = {0};
        int user_id;
        int i = 0;

        while (1)
        {
            n = recv(sock, buffer, 1023, 0);
            printf("%s\n", buffer);
            if (strcmp(buffer, "Enter Username:") == 0)
            {
                scanf("%s", username);
                send(sock, username, USERNAME_MAX_SIZE, 0);
                //printf("i: %d, user\n", i);
            }
            else if (strcmp(buffer, "Enter Password:") == 0)
            {
                scanf("%s", password);
                send(sock, password, PASSWORD_MAX_SIZE, 0);
                //printf("i: %d, pass\n", i);
            }
            else if (strcmp(buffer, "Enter user_id: ") == 0)
            {
                scanf("%d", &user_id);
                send(sock, &user_id, sizeof(int), 0);
                //printf("i: %d, uid\n", i);
            }
            else if(strcmp(buffer, "You have logged in successfully!\n") == 0)
            {
                while(1){
                    user_menu();
                    scanf("%d", &command);
                    while(command != 0 && command != 1 && command != 2){
                        printf("Please enter a command that is 0, 1 or 2.\n");
                        printf("Enter command: ");
                        scanf("%d", &command);
                    }
                    send(sock, &command, sizeof(int), 0);
                    if(command == 1){
                        int user_id;
                        int book_id;
                        int transaction_id;
                        int number_of_copies_to_borrow;
                        printf("Enter user_id: ");
                        scanf("%d", &user_id);
                        printf("Enter book_id: ");
                        scanf("%d", &book_id);
                        printf("Enter number of copies to borrow: ");
                        scanf("%d", &number_of_copies_to_borrow);
                        transaction_id = read_transaction_count()+1;
                        write_transaction_count(transaction_id);
                        struct Borrowed borrowed;
                        borrowed.book_id = book_id;
                        borrowed.user_id = user_id;
                        borrowed.transaction_id = transaction_id;
                        borrowed.number_of_copies_to_borrow = number_of_copies_to_borrow;
                        borrowed.is_returned = 0;
                        send(sock, &borrowed, sizeof(struct Borrowed), 0);
                        recv(sock, buffer, 1023, 0);
                        printf("hi\n");
                        printf("%s", buffer);
                        //borrow book
                    }
                    else if(command == 2){
                        int user_id;
                        int book_id;
                        int transaction_id;
                        printf("Enter user_id: ");
                        scanf("%d", &user_id);
                        printf("Enter book_id: ");
                        scanf("%d", &book_id);
                        printf("Enter transaction_id: ");
                        scanf("%d", &transaction_id);
                        send(sock, &user_id, sizeof(int), 0);
                        send(sock, &book_id, sizeof(int), 0);
                        send(sock, &transaction_id, sizeof(int), 0);
                        recv(sock, buffer, 1023, 0);
                        printf("%s", buffer);
                        //return book
                    }
                    else{
                        return 0;
                    }
                }
            }
            else{
                break;
            }
            
        }
    }

    return 0;
}