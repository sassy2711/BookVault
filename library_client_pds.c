// Client side C program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "database/pds.h"
#include "database/user_book.h"
#include "database/bst.h"

#define PORT 8080

void menu(){
    printf("Enter 1 to signup and 2 to login.\n");
}

int main(int argc, char *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    menu();
    char command[1];
    scanf("%s", command);
    while(1){
            //printf("%s\n", command);
            if(strcmp(command, "1") == 0 || strcmp(command, "2") == 0){
                send(sock, command, sizeof(command), 0);
                break;
            }
            else{
                printf("Please enter a value that is either 1(to signup) or 2(to login).\n");
                scanf("%s", command);
            }    }
    int n;
    char username[USERNAME_MAX_SIZE] = {0};
    char password[PASSWORD_MAX_SIZE] = {0};
    int user_id;
    //int i = 0;
    while(1){
        n = recv(sock, buffer, 1023, 0);
        printf("%s\n", buffer);
        if(strcmp(buffer, "Enter Username:") == 0){
            scanf("%s", username);
            send(sock, username, USERNAME_MAX_SIZE, 0);
            //printf("i: %d, user\n", i);
        }
        else if(strcmp(buffer, "Enter Password:") == 0){
            scanf("%s", password);
            send(sock, password, PASSWORD_MAX_SIZE, 0);
            //printf("i: %d, pass\n", i);
        }
        else if(strcmp(buffer, "Enter user_id: ") == 0){
            scanf("%d", &user_id);
            send(sock, &user_id, sizeof(int), 0);
        }
        else{
            //printf("i: %d, if: 3\n", i);
            break;
        }
        //i++;

    }

    return 0;
}