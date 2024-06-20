#define USER_COUNT_FILE "user_count.txt"
#define BOOK_COUNT_FILE "book_count.txt"
#define TRANSACTION_COUNT_FILE "transaction_count.txt"

int read_user_count();
void write_user_count(int user_count);
int read_book_count();
void write_book_count(int book_count);
int read_transaction_count();
void write_transaction_count(int transaction_count); 