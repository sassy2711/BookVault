# Book Vault - An Online Library Management System

A comprehensive library management system built using C, Shell scripting, and Socket programming. This system provides robust functionality for both **admin** and **clients**, ensuring efficient management of library resources and user operations.

## Features

- **Admin Features:**
  - Login and Signup functionalities.
  - Add, update, delete books (CRUD operations).
  - View current borrow/return logs.

- **Client Features:**
  - Borrow and return books seamlessly.
  - View available books and transaction history.

## Technical Details

- **Process Management:** Efficient handling of multiple users using multi-threading.
- **File Handling & Locking:** Secure access to resources via file system operations and locks.
- **Inter-process Communication (IPC):** Facilitates smooth interactions between clients and server using socket programming.
- **Multi-threading:** Enables simultaneous operations for better performance.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/sassy2711/OnlineLibraryManagementSystem.git

## Usage 

 1. Open two terminals.
 2. Run the server on one terminal:-
    ```bash
    gcc library_server.c -lpthread
    ./a.out
3. Run the client on the other terminal:-
    ```bash
    gcc library_client.c -lpthread
    ./a.out
4. You will see a menu in the client side.
5. Follow the instructions and enjoy.


## Unit Testing
The provided code performs unit testing for a library management system using the CUnit testing framework. It ensures the functionality of various modules, including admin operations, book management, member management, and borrowing/returning books. 

Below given is the description for each test suite:

### Admin Tests
![Suite_admin_login  ](images/Suite_admin_login.png "Suite_admin_login")

1. **`test_admin_check_valid_credentials()`**  
   Tests whether valid admin credentials are correctly recognized by the `admin_check` function.

2. **`test_admin_check_invalid_credentials()`**  
   Tests whether invalid admin credentials are correctly rejected by the `admin_check` function.

---

### Book Management Tests
![Suite_book_management ](images/Suite_book_management.png "Suite_book_management")

3. **`test_add_new_book()`**  
   Tests the functionality of adding a new book to the system by checking if the book details are correctly written to `books.txt`.

4. **`test_remove_book_found()`**  
   Tests the removal of a book that exists in the system, expecting success.

5. **`test_remove_book_not_found()`**  
   Tests the removal of a book that does not exist in the system, expecting failure.

6. **`test_search_book_found()`**  
   Tests searching for a book that exists, expecting success.

7. **`test_search_book_not_found()`**  
   Tests searching for a book that does not exist, expecting failure.

8. **`test_modify_book_count_success()`**  
   Tests modifying the count of a book that exists in the system, expecting success.

9. **`test_modify_book_count_failure()`**  
   Tests modifying the count of a book that does not exist, expecting failure.

10. **`test_display_books_non_empty()`**  
    Tests displaying the list of books when there are entries in the database, expecting success.

11. **`test_display_books_empty()`**  
    Tests displaying the list of books when the database is empty, expecting failure.

---

### Member Management Tests
![Suite_member_management](images/Suite_member_management.png "Suite_member_management")

12. **`test_add_member_success()`**  
    Tests adding a new member with valid credentials, expecting success.

13. **`test_add_member_failure()`**  
    Tests adding a new member with invalid or empty input, expecting failure.

14. **`test_login_member_success()`**  
    Tests logging in with valid member credentials, expecting success.

15. **`test_login_member_failure()`**  
    Tests logging in with invalid credentials, expecting failure.

16. **`test_remove_member_success()`**  
    Tests removing a member who exists in the system, expecting success.

17. **`test_remove_member_failure()`**  
    Tests removing a member who does not exist, expecting failure.

18. **`test_display_members_non_empty()`**  
    Tests displaying the list of members when there are entries, expecting success.

19. **`test_display_members_empty()`**  
    Tests displaying the list of members when the database is empty, expecting failure.

---

### Borrowing and Returning Tests
![Suite_borrowing_books](images/Suite_borrowing_books.png "Suite_borrowing_books")

20. **`test_borrow_book()`**  
    Tests borrowing a book by decrementing its count in `books.txt` and adding an entry to `borrowed_books.txt`.

21. **`test_return_book()`**  
    Tests returning a borrowed book by incrementing its count in `books.txt` and removing the corresponding entry from `borrowed_books.txt`.

---
![Suite_test_summary](images/Suite_test_summary.png "Suite_test_summary")


