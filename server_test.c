#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "library_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void test_admin_check_valid_credentials(void) {
    char buffer[] = "1,Admin,Pw123";
    int result = admin_check(buffer);
    CU_ASSERT_EQUAL(result, 1);  
}

void test_admin_check_invalid_credentials(void) {
    char buffer[] = "1,Admin,hello";
    int result = admin_check(buffer);
    CU_ASSERT_EQUAL(result, 0);  
}

void test_add_new_book(void) {
    FILE *fd = fopen("books.txt", "w");
    if (fd == NULL) {
        CU_FAIL("Failed to create test file");
        return;
    }
    fclose(fd);

    char buffer1[] = "2,B123,Harry Potter,Rowling,101";
    int result1 = add_new_book(buffer1);

    CU_ASSERT_EQUAL(result1, 1); 
    fd = fopen("books.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(fd);

    char line[400];
    fgets(line, sizeof(line), fd);
    CU_ASSERT_STRING_EQUAL(line, "B123,Harry Potter,Rowling,101\n");

    fclose(fd);
}


void test_remove_book_found(void) {
    char buffer[] = "3,B123";
    int result = remove_book(buffer);
    CU_ASSERT_EQUAL(result, 1);  
}

void test_remove_book_not_found(void) {
    char buffer[] = "3,B234";
    int result = remove_book(buffer);
    CU_ASSERT_EQUAL(result, 0); 
}


void test_search_book_found(void) {
    char buffer[] = "4,B123";
    int new_socket = 1; 
    int result = search_book(buffer, new_socket);

    CU_ASSERT_EQUAL(result, 1);
}

void test_search_book_not_found(void) {
    char buffer[] = "4,B234";
    int new_socket = 1; 
    int result = search_book(buffer, new_socket);

    CU_ASSERT_EQUAL(result, 0);
}


void test_modify_book_count_success(void) {
    char buffer[] = "5,B123,200"; 
    int result = modify_book_count(buffer);
    CU_ASSERT_EQUAL(result, 1);  
}

void test_modify_book_count_failure(void) {
    char buffer[] = "5,B234,200";
    int result = modify_book_count(buffer);
    CU_ASSERT_EQUAL(result, 0);  
}


void test_display_books_non_empty(void) {
    char buffer[] = "d"; 
    int new_socket = 1; 
    int result = display_books(buffer, new_socket);

    CU_ASSERT_EQUAL(result, 1);
}

void test_display_books_empty(void) {
    char buffer[] = "d";
    int new_socket = 1; 
    int result = display_books(buffer, new_socket);
    CU_ASSERT_EQUAL(result, 0);
}


void test_display_members_non_empty(void) {
    char buffer[] = "d";
    int new_socket = 1; 
    int result = display_members(buffer, new_socket);

    CU_ASSERT_EQUAL(result, 1);
}

void test_display_members_empty(void) {
    char buffer[] = "d";
    int new_socket = 1; 
    int result = display_members(buffer, new_socket);

    CU_ASSERT_EQUAL(result, 0);
}


void test_add_member_success() {
    char test_input[] = "6,TestUser,Testpwd";
    int result = add_member(test_input);
    CU_ASSERT_EQUAL(result, 1);
    
    FILE* fd = fopen("logins.txt", "r");
    char line[400];
    int found = 0;
    while (fgets(line, sizeof(line), fd) != NULL) {
        if (strcmp(line, "TestUser,Testpwd\n") == 0) {
            found = 1;
            break;
        }
    }
    fclose(fd);
    CU_ASSERT_EQUAL(found, 1);
}

void test_add_member_failure() {
    char test_input[] = "";
    int result = add_member(test_input);
    CU_ASSERT_EQUAL(result, 0);
}


void test_login_member_success() {
    char test_input_add[] = "6,TestUser,Testpwd";
    add_member(test_input_add);

    char test_input_login[] = "9,TestUser,Testpwd";
    int result = login_member(test_input_login);
    CU_ASSERT_EQUAL(result, 1);
}

void test_login_member_failure() {
    char test_input_login[] = "9,TestUser,pwd";
    int result = login_member(test_input_login);
    CU_ASSERT_EQUAL(result, 0);
}



void test_remove_member_success() {
    char buffer[] = "c,TestUser";
    int result = remove_member(buffer);
    CU_ASSERT_EQUAL(result, 1); 
}

void test_remove_member_failure() {
    char buffer[] = "c,TestUser";
    int result = remove_member(buffer);
    CU_ASSERT_EQUAL(result, 0); 
}


void test_borrow_book() {
    FILE *books_file = fopen("books.txt", "w");
    fprintf(books_file, "B123,Harry Potter,Rowling,5\n");
    fclose(books_file);

    char borrow_request[] = "8,B123,TestUser";
    int result = borrow_book(borrow_request);

    
    CU_ASSERT_EQUAL(result, 1);

    books_file = fopen("books.txt", "r");
    char line[400];
    fgets(line, sizeof(line), books_file);
    fclose(books_file);
    CU_ASSERT_STRING_EQUAL(line, "B123,Harry Potter,Rowling,4\n");

    FILE *borrowed_file = fopen("borrowed_books.txt", "r");
    fgets(line, sizeof(line), borrowed_file);
    fclose(borrowed_file);
    CU_ASSERT_STRING_EQUAL(line, "B123,TestUser\n");
}


void test_return_book() {
    char return_request[] = "a,B123,TestUser";
    int result = return_book(return_request);

    CU_ASSERT_EQUAL(result, 1);

    FILE *books_file = fopen("books.txt", "r");
    char line[400];
    fgets(line, sizeof(line), books_file);
    CU_ASSERT_STRING_EQUAL(line, "B123,Harry Potter,Rowling,5\n");
    fclose(books_file);
}



int main() {
    // Initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Create a test suite
    CU_pSuite psuite = CU_add_suite("Admin Login Check", NULL, NULL);
    if (psuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(psuite, "test_admin_check_valid_credentials", test_admin_check_valid_credentials);
    CU_add_test(psuite, "test_admin_check_invalid_credentials", test_admin_check_invalid_credentials);


    psuite = CU_add_suite("Book Management Tests", NULL, NULL);
    if (psuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    CU_add_test(psuite, "test_add_new_book", test_add_new_book);

    CU_add_test(psuite, "test_search_book_found", test_search_book_found);
    CU_add_test(psuite, "test_search_book_not_found", test_search_book_not_found);

    CU_add_test(psuite, "test_remove_book_found", test_remove_book_found);
    CU_add_test(psuite, "test_remove_book_not_found", test_remove_book_not_found);

    CU_add_test(psuite, "test_add_new_book", test_add_new_book);
    CU_add_test(psuite, "test_modify_book_count_success", test_modify_book_count_success);
    CU_add_test(psuite, "test_modify_book_count_failure", test_modify_book_count_failure);

    CU_add_test(psuite, "test_display_books_non_empty", test_display_books_non_empty);
    CU_add_test(psuite, "test_remove_book_found", test_remove_book_found);
    CU_add_test(psuite, "test_display_books_empty", test_display_books_empty);



    // Member tests
    psuite = CU_add_suite("Member Management Tests", NULL, NULL);
    if (psuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    CU_add_test(psuite, "test_add_member_success", test_add_member_success);
    CU_add_test(psuite, "test_add_member_failure", test_add_member_failure);

        
    CU_add_test(psuite, "test_display_members_non_empty", test_display_members_non_empty);
    
    CU_add_test(psuite, "test_remove_member_success", test_remove_member_success);
    CU_add_test(psuite, "test_remove_member_failure", test_remove_member_failure);
    
    CU_add_test(psuite, "test_display_members_empty", test_display_members_empty);

    CU_add_test(psuite, "test_login_member_success", test_login_member_success);
    CU_add_test(psuite, "test_login_member_failure", test_login_member_failure);


    psuite = CU_add_suite("Borrowing Tests", NULL, NULL);
    if (psuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(psuite, "test_borrow_book_success", test_borrow_book);    
    
    CU_add_test(psuite, "test_return_book_success", test_return_book);



    // Run the tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
