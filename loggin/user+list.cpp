#include "loggin.h"


UserList make_empty_list()
{
	UserList lst;
	lst.head = lst.tail = NULL;
	return lst;
}

User* make_new_user(char* user_name, char* password, char* first_name, char* last_name, int* birth_date)
{
	User* new_user;
	new_user = (User*)malloc(sizeof(User));
	new_user->next = new_user->prev = NULL;

	new_user->user_name = (char*)malloc(strlen(user_name) + 1);
	new_user->password = (char*)malloc(strlen(password) + 1);
	new_user->first_name = (char*)malloc(strlen(first_name) + 1);
	new_user->last_name = (char*)malloc(strlen(last_name) + 1);

	strcpy(new_user->user_name, user_name);
	strcpy(new_user->password, password);
	strcpy(new_user->first_name, first_name);
	strcpy(new_user->last_name, last_name);

	new_user->date_of_birth[0] = birth_date[0];
	new_user->date_of_birth[1] = birth_date[1];
	new_user->date_of_birth[2] = birth_date[2];

	return new_user;
}

void insert_user_to_tail(UserList* lst, User* user)
{
	if (lst->head == NULL)
		lst->head = lst->tail = user;
	else
	{
		lst->tail->next = user;
		user->prev = lst->tail;
		lst->tail = user;
	}
}

void print_user(User user)
{
	printf("user name: %s\n", user.user_name);
	printf("password: %s\n", user.password);
	printf("name: %s %s\n", user.first_name, user.last_name);
	printf("date of birth: %d/%d/%d\n", user.date_of_birth[0], user.date_of_birth[1], user.date_of_birth[2]);
}

void print_user_list(UserList lst)
{
	User* p = lst.head;
	while (p)
	{
		printf("\n");
		print_user(*p);
		p = p->next;
	}
	printf("\n");
}

void save_string(char* str, FILE* f)
//a helper for save_user
{
	int len;
	len = strlen(str);
	fwrite(&len, sizeof(int), 1, f);
	fwrite(str, sizeof(char), len, f);
}

void save_user(User* usr, FILE* f)
//a helper for save_user_list
{
	save_string(usr->user_name,f);
	save_string(usr->password, f);
	save_string(usr->first_name, f);
	save_string(usr->last_name, f);
	
	fwrite(usr->date_of_birth, sizeof(int), 3, f);
}

void save_user_list(UserList lst)
//saves the users list in a binary file(saves an integer with the strings length before each string)
{
	FILE* f;
	User* p=lst.head;

	f=fopen("Users_List.bin", "wb");
	while (p)
	{
		save_user(p, f);
		p = p->next;
	}

	fclose(f);
}

int get_end_of_file(FILE* f)
{
	int res;
	fseek(f, 0, SEEK_END);
	res = ftell(f);
	fseek(f, 0, SEEK_SET);

	return res;
}

char* read_string(FILE* f)
//a helper for read_user
{
	char* res;
	int len;

	fread(&len, sizeof(int), 1, f);
	res = (char*)malloc(len + 1);
	fread(res, sizeof(char), len, f);
	res[len] = '\0';

	return res;
}

User* read_user(FILE* f)
//a helper for load_user_list
{
	User* usr;
	char* user_name;
	char* password;
	char* first_name;
	char* last_name;
	int birth_date[3];

	user_name = read_string(f);
	password = read_string(f);
	first_name = read_string(f);
	last_name = read_string(f);
	fread(birth_date, sizeof(int), 3, f);

	usr = make_new_user(user_name, password, first_name, last_name, birth_date);

	free(user_name);
	free(password);
	free(first_name);
	free(last_name);

	return usr;
}

UserList load_user_list()
{//loads the users list from a binary file
	UserList lst = make_empty_list();
	User* usr;
	FILE* f;
	int fend;

	f=fopen("Users_List.bin", "rb");
	fend = get_end_of_file(f);

	usr = read_user(f);
	insert_user_to_tail(&lst, usr);
	while (ftell(f) != fend)
	{
		usr->next = read_user(f);
		usr = usr->next;
		insert_user_to_tail(&lst, usr);
	}

	fclose(f);
	return lst;
}

void free_user(User* user)
{//helper for delete_user_from_list
	free(user->user_name);
	free(user->password);
	free(user->first_name);
	free(user->last_name);
	free(user);
}

void delete_user_from_list(User* user) 
{
	if(user->next)
		user->next->prev = user->prev;
	if(user->prev)
		user->prev->next = user->next;
	free_user(user);
}

void admin_show_info(UserList lst)
{
	User* user;
	char user_name[MAX_CHARS];
	printf("please enter the user's name OR type in 'show_all' to see all users\n");
	gets_s(user_name, MAX_CHARS);

	if (strcmp(user_name, "show_all") == 0)
		print_user_list(lst);
	else
	{
		user = find_user_according_to_user_name(user_name, lst);
		if (user)
			print_user(*user);
		else
			printf("user was not found!\n");
	}
}

void admin_change_password(UserList lst)
{
	char user_name[MAX_CHARS];
	User* user;

	printf("please enter the user's user name: ");
	gets_s(user_name, MAX_CHARS);
	user = find_user_according_to_user_name(user_name, lst);
	if (user)
	{
		change_user_password(user);
		printf("password changed successfully!\n");
	}
	else
		printf("user was not found!\n");
}

void admin_delete_user(UserList* lst)
{
	char user_name[MAX_CHARS];
	User* user;

	printf("please enter the user name: ");
	gets_s(user_name, MAX_CHARS);
	user = find_user_according_to_user_name(user_name, *lst);
	if (user)
	{
		delete_user_from_list(user);
		printf("user deleted successfully!\n");
	}
	else
		printf("user was not found!\n");
}

void admin_options(User* user, UserList* lst)
{
	int user_input = 99;
	char ch;

	if (strcmp(user->user_name, "admin") == 0)
	{
		while (user_input != 0)
		{
			printf("please choose one of the followings:\n");
			printf("\t1.show user info.\n");
			printf("\t2.change user's password.\n");
			printf("\t3.delete a user.\n");
			printf("0.Go Back.\n");
			scanf("%d", &user_input);

			scanf("%c", &ch);
			switch (user_input)
			{
			case(1):
				admin_show_info(*lst);
				break;
			case(2):
				admin_change_password(*lst);
				break;
			case(3):
				admin_delete_user(lst);
				break;
			default:
				printf("VALUE IS NOT VALID!\n");
				break;
			}
		}
	}
	else
		printf("you DO NOT have the requiered permissions!\n");
}