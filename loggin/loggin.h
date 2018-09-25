#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct user {
	char* user_name;
	char* password;
	char *first_name, *last_name;
	int date_of_birth[3];
	struct user* next;
	struct user* prev;
}User;

typedef struct user_list {
	User* head;
	User* tail;
}UserList;

#define MAX_CHARS 30

/////////////////user+list
UserList make_empty_list();
User* make_new_user(char* user_name, char* password, char* first_name, char* last_name, int* birth_date);
void insert_user_to_tail(UserList* lst, User* user);
void print_user(User user);
void print_user_list(UserList lst);
void save_user_list(UserList lst);
UserList load_user_list();
void delete_user_from_list(User* user);
void admin_options(User* user, UserList* lst);

////////////////functions
void registration(UserList* lst);
User* sign_in(UserList lst);
void user_settings(User* user, UserList* lst);
User* find_user_according_to_user_name(char* user_name, UserList lst);
void change_user_password(User* user);
