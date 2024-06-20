#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "operations.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

int read_user_count() {
    int rec_count = 0;
    FILE *file = fopen(USER_COUNT_FILE, "r");
    if (file != NULL) {
        flock(fileno(file), LOCK_SH);  // Shared lock for reading
        fscanf(file, "%d", &rec_count);
        flock(fileno(file), LOCK_UN);  // Unlock
        fclose(file);
    }
    return rec_count;
}

void write_user_count(int user_count) {
    FILE *file = fopen(USER_COUNT_FILE, "w");
    if (file != NULL) {
        flock(fileno(file), LOCK_EX);  // Exclusive lock for writing
        fprintf(file, "%d", user_count);
        flock(fileno(file), LOCK_UN);  // Unlock
        fclose(file);
    }
}

int read_book_count() {
    int rec_count = 0;
    FILE *file = fopen(BOOK_COUNT_FILE, "r");
    if (file != NULL) {
        flock(fileno(file), LOCK_SH);  // Shared lock for reading
        fscanf(file, "%d", &rec_count);
        flock(fileno(file), LOCK_UN);  // Unlock
        fclose(file);
    }
    return rec_count;
}

void write_book_count(int book_count) {
    FILE *file = fopen(BOOK_COUNT_FILE, "w");
    if (file != NULL) {
        flock(fileno(file), LOCK_EX);  // Exclusive lock for writing
        fprintf(file, "%d", book_count);
        flock(fileno(file), LOCK_UN);  // Unlock
        fclose(file);
    }
}

int read_transaction_count() {
    int rec_count = 0;
    FILE *file = fopen(TRANSACTION_COUNT_FILE, "r");
    if (file != NULL) {
        flock(fileno(file), LOCK_SH);  // Shared lock for reading
        fscanf(file, "%d", &rec_count);
        flock(fileno(file), LOCK_UN);  // Unlock
        fclose(file);
    }
    return rec_count;
}

void write_transaction_count(int transaction_count) {
    FILE *file = fopen(TRANSACTION_COUNT_FILE, "w");
    if (file != NULL) {
        flock(fileno(file), LOCK_EX);  // Exclusive lock for writing
        fprintf(file, "%d", transaction_count);
        flock(fileno(file), LOCK_UN);  // Unlock
        fclose(file);
    }
}

