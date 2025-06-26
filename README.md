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
