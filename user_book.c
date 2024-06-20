#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
// #include "pds.h"
#include "user_book.h"
#include "operations.h"

void add_user(FILE *user_db, struct User *user)
{
    fseek(user_db, 0, SEEK_END);
    fwrite(user, sizeof(struct User), 1, user_db);
    fflush(user_db);
}

int search_user(FILE *user_db, int user_id, char *password, struct User *user)
{
    fseek(user_db, 0, SEEK_SET);
    while (fread(user, sizeof(struct User), 1, user_db) != 0)
    {
        //printf("username: %s\n", user->password);
        if (user->user_id == user_id && strcmp(user->password, password) == 0 && user->is_deleted == 0)
        {
            return 0;
        }
    }
    return 1;
}

void add_book(FILE *book_db, struct Book *book)
{
    fseek(book_db, 0, SEEK_END);
    fwrite(book, sizeof(struct Book), 1, book_db);
    fflush(book_db);
}

int search_book(FILE *book_db, int book_id, struct Book *book)
{
    fseek(book_db, 0, SEEK_SET);
    //printf("before loop\n");
    while (fread(book, sizeof(struct Book), 1, book_db) != 0)
    {
        //printf("book_id: %d\n", book->book_id);
        if (book->book_id == book_id && book->is_deleted == 0)
        {
            return 0;
        }
    }
    //printf("after loop\n");
    return 1;
}

void show_all_books(FILE *book_db, int client_socket)
{
    struct Book book;
    char buffer[1024];
    fseek(book_db, 0, SEEK_SET);
    while (fread(&book, sizeof(struct Book), 1, book_db) != 0)
    {
        if (book.is_deleted == 0)
        {
            sprintf(buffer, "Title: %s\nAuthor: %s\nBook ID: %d\nNumber of copies available: %d\n\n", book.title, book.author, book.book_id, book.copies_available);
            send(client_socket, buffer, 1023, 0);
        }
    }
    send(client_socket, "Finished", 1023, 0);
}

void set_copies_available(FILE *book_db, int book_id, int new_copies_available)
{
    struct Book book;
    if (search_book(book_db, book_id, &book) == 0)
    {
        fseek(book_db, -1 * sizeof(struct Book), SEEK_CUR);
        book.copies_available = new_copies_available;
        fwrite(&book, sizeof(struct Book), 1, book_db);
        fflush(book_db);
    }
    else
    {
        printf("Book with this book_id does not exist.\n");
    }
}

void delete_book(FILE *book_db, int book_id)
{
    struct Book book;
    if (search_book(book_db, book_id, &book) == 0)
    {
        fseek(book_db, -1 * sizeof(struct Book), SEEK_CUR);
        book.is_deleted = 1;
        fwrite(&book, sizeof(struct Book), 1, book_db);
        fflush(book_db);
    }
    else
    {
        printf("Book with this book_id does not exist.\n");
    }
}

void sign_up_user(FILE *user_db, int client_socket)
{
    struct User *user = (struct User *)malloc(sizeof(struct User));
    char username[USERNAME_MAX_SIZE] = {0};
    char password[PASSWORD_MAX_SIZE] = {0};
    int n;

    // Prompt for username
    send(client_socket, "Enter Username:", 17, 0);
    n = recv(client_socket, username, USERNAME_MAX_SIZE, 0);
    if (n == -1)
    {
        perror("recv");
        close(client_socket);
        free(user);
        exit(EXIT_FAILURE);
    }
    username[n] = '\0'; // Null-terminate the received string
    // Prompt for password
    // printf("before enter password\n");
    send(client_socket, "Enter Password:", 17, 0);
    n = recv(client_socket, password, PASSWORD_MAX_SIZE, 0);
    // printf("password: %s\n", password);
    if (n == -1)
    {
        perror("recv");
        close(client_socket);
        free(user);
        exit(EXIT_FAILURE);
    }
    password[n] = '\0'; // Null-terminate the received string

    // Read and update the shared rec_count
    int rec_count = read_user_count();
    user->user_id = rec_count + 1;
    user->signed_up = 1;
    user->signed_in = 0;
    user->is_deleted = 0;
    // Update the rec_count in the file
    write_user_count(user->user_id);

    // Populate user structure
    strcpy(user->username, username);
    strcpy(user->password, password);
    char buffer[1024];
    // Add user to the repository
    add_user(user_db, user);
    // printf("1\n");
    // if (status == 0) {
    sprintf(buffer, "User successfully added with user_id = %d.\n", user->user_id);
    send(client_socket, buffer, 1023, 0);
    //}

    free(user); // Free the allocated memory for user
    // return status;
}

int sign_in_user(FILE *user_db, int client_socket)
{
    struct User *user = (struct User *)malloc(sizeof(struct User));
    int *user_id = (int *)malloc(sizeof(int));
    send(client_socket, "Enter user_id: ", 1023, 0);
    int n;
    char password[PASSWORD_MAX_SIZE] = {0};
    n = recv(client_socket, user_id, sizeof(int), 0);
    if (n == -1)
    {
        perror("recv");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    send(client_socket, "Enter Password:", 1023, 0);
    recv(client_socket, password, PASSWORD_MAX_SIZE, 0);
    // printf("user_id: %d\n", *user_id);
    int status = search_user(user_db, *user_id, password, user);
    // printf("status: %d\n", status);
    if (status == 0)
    {
        send(client_socket, "You have logged in successfully!\n", 1023, 0);
        user->signed_in = 1;
    }
    else
        send(client_socket, "The user_id or the password is wrong.\n", 1023, 0);
    return status;
}

int borrow_book(FILE *user_book_db, FILE *book_db, struct Borrowed *borrowed)
{
    struct Book book;
    //printf("0\n");
    if (search_book(book_db, borrowed->book_id, &book) == 0)
    {   
        //printf("1\n");
        if (book.copies_available >= borrowed->number_of_copies_to_borrow)
        {   
            //printf("2\n");
            fseek(user_book_db, 0, SEEK_END);
            fwrite(borrowed, sizeof(struct Borrowed), 1, user_book_db);
            fflush(user_book_db);
            //printf("3\n");
            set_copies_available(book_db, book.book_id, book.copies_available - borrowed->number_of_copies_to_borrow);
            //printf("4\n");
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
}

int search_borrowed_book_entry(FILE *user_book_db, int user_id, int book_id, int transaction_id, struct Borrowed *borrowed)
{
    fseek(user_book_db, 0, SEEK_SET);
    while (fread(borrowed, sizeof(struct Borrowed), 1, user_book_db) != 0)
    {   
        printf("book_id: %d\n", borrowed->book_id);
        if (borrowed->book_id == book_id && borrowed->user_id == user_id && borrowed->is_returned == 0 && borrowed->transaction_id == transaction_id)
        {
            return 0;
        }
    }
    printf("\n");
    return 1;
}

int delete_borrowed_book_entry(FILE *user_book_db, int user_id, int book_id, int transaction_id)
{
    struct Borrowed borrowed;
    if (search_borrowed_book_entry(user_book_db, user_id, book_id, transaction_id, &borrowed) == 0)
    {
        fseek(user_book_db, -1 * sizeof(struct Borrowed), SEEK_CUR);
        borrowed.is_returned = 1;
        fwrite(&borrowed, sizeof(struct Borrowed), 1, user_book_db);
        fflush(user_book_db);
        return 0;
    }
    else
    {
        return -1;
    }
}

int return_book(FILE *user_book_db, FILE *book_db, int user_id, int book_id, int transaction_id)
{
    struct Book book;
    struct Borrowed borrowed;
    if (delete_borrowed_book_entry(user_book_db, user_id, book_id, transaction_id) == 0)
    {
        search_borrowed_book_entry(user_book_db, user_id, book_id, transaction_id, &borrowed);
        if (search_book(book_db, book_id, &book) == 0)
        {
            set_copies_available(book_db, book_id, book.copies_available + borrowed.number_of_copies_to_borrow);
        }
        return 0;
    }
    else
    {
        return -1;
    }
}
