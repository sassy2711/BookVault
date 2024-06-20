//these are the possible values for the EXPECTED_STATUS
#define PS_SUCCESS 0    
#define PS_FAILURE 1
#define USERNAME_MAX_SIZE 50
#define PASSWORD_MAX_SIZE 50
#define TITLE_MAX_SIZE 50
#define AUTHOR_MAX_SIZE 50
//this is just a macro for the maximumm number of children a parent can have, can be changed as per need
#define MAX_bookS_PER_user 10 
#define ADMINISTRATOR_PASS "10101" 

//I have implemented a userfesor-book(parent-child) relationship

struct User{    
	int user_id;
	char username[USERNAME_MAX_SIZE];
	char password[PASSWORD_MAX_SIZE];
    int signed_up;
    int signed_in;
    int is_deleted;
};

struct Book{
    int book_id;
    int copies_available;
    char title[TITLE_MAX_SIZE];
    char author[AUTHOR_MAX_SIZE];
    int is_deleted;
};

struct Borrowed{
    int transaction_id;
    int user_id;
    int book_id;
    int is_returned;
    int number_of_copies_to_borrow;
};

void add_user(FILE* user_db, struct User* user);

int search_user(FILE* user_db, int user_id, char* password, struct User* user);

void sign_up_user(FILE* user_db, int client_socket);

int sign_in_user(FILE* user_db, int client_socket);
 
void add_book(FILE* book_db, struct Book* book);

int search_book(FILE* book_db, int book_id, struct Book* book);

void show_all_books(FILE* book_db, int client_socket);

void set_copies_available(FILE* book_db, int book_id, int new_copies_available);

void delete_book(FILE* book_db, int book_id);

int borrow_book(FILE* user_book_db, FILE* book_db, struct Borrowed* borrowed);

int search_borrowed_book_entry(FILE* user_book_db, int user_id, int book_id, int transaction_id, struct Borrowed* borrowed);

int delete_borrowed_book_entry(FILE* user_book_db, int user_id, int book_id, int transaction_id);

int return_book(FILE *user_book_db, FILE *book_db, int user_id, int book_id, int transaction_id);