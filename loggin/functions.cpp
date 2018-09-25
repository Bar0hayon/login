#include "loggin.h"

char* get_new_password()
{//gets a new user password from the user.
	char *password = (char*)malloc(MAX_CHARS);
	char confirm_pass[MAX_CHARS];

	do
	{
		printf("enter password: ");
		gets_s(password, MAX_CHARS);
		printf("confirm password: ");
		gets_s(confirm_pass, MAX_CHARS);
		if (strcmp(password, confirm_pass) != 0)
			printf("passwords doesn't match! let's try again :)\n");
	} while (strcmp(password, confirm_pass) != 0);

	return password;
}

bool is_free_user_name(UserList lst,char* user_name)
{
	User* user_check = find_user_according_to_user_name(user_name, lst);
	if (user_check)
		return false;
	else
		return true;
}

void registration(UserList* lst)
{
	User* new_user;
	char user_name[MAX_CHARS];
	char* password;

	char first_name[MAX_CHARS];
	char last_name[MAX_CHARS];
	char ch;
	int birth_date[3];

	scanf("%c", &ch);
	do {
		printf("enter user name: ");
		gets_s(user_name, MAX_CHARS);
		if (!is_free_user_name(*lst, user_name))
			printf("user name is allready taken.. please try another one\n");
	} while (!is_free_user_name(*lst, user_name));

	password = get_new_password();
	
	printf("enter first name: ");
	gets_s(first_name, MAX_CHARS);
	printf("enter last name: ");
	gets_s(last_name, MAX_CHARS);

	printf("enter your date of birth according to the format bellow:\n");
	printf("DD MM YY\n");
	scanf("%d%d%d", birth_date, birth_date + 1, birth_date + 2);

	new_user = make_new_user(user_name, password, first_name, last_name, birth_date);
	insert_user_to_tail(lst, new_user);
	free(password);
}

User* find_user_according_to_user_name(char* user_name, UserList lst)
{
	User* p = lst.head;
	
	while (p&&strcmp(p->user_name, user_name) != 0)
		p = p->next;

	if (p)
		return p;
	else
		return NULL;
}

char* get_password()
{//reads password from the user, shows '*' and output the user's input
	char* password = (char*)malloc(MAX_CHARS);
	char ch;
	int i=0;

	printf("Enter password: ");
	ch = _getch();
	if(!ch)
		ch = _getch();
	while (ch!='\r')
	{
		printf("*");
		password[i] = ch;
		i++;
		ch = _getch();
		ch = _getch();
	}
	password[i] = '\0';
	printf("\n");
	return password;
}

User* sign_in(UserList lst)
{
	User *user;
	char user_name[MAX_CHARS];
	char *password,ch;
	int attempts = 3, option;

	printf("Enter user name: ");
	scanf("%c", &ch);
	gets_s(user_name, MAX_CHARS);
	user = find_user_according_to_user_name(user_name, lst);

	if (user)
	{
		password = get_password();
		while (strcmp(password, user->password) != 0 && attempts > 0)
		{
			printf("you have entered a wrong password! (%d more attempts left)\n",attempts);
			password = get_password();
			attempts--;
		}

		if (strcmp(password, user->password) == 0)
			return user;
		else
			return NULL;
	}
	else
	{
		printf("user name was NOT found!\nplease choose one of the followings:\n");
		printf("\t1.Enter a different user name.\n");
		printf("\t2.exit.\n");
		scanf("%d", &option);
		if (option == 1)
			return sign_in(lst);
		else
			return NULL;
	}
}

void change_user_password(User* user)
{
	char* new_pass;

	new_pass = get_new_password();
	user->password = (char*)realloc(user->password, strlen(new_pass)+1);
	strcpy(user->password, new_pass);
}

void user_settings(User* user, UserList* lst)
{
	int user_input = 99;
	char op;
	char ch;

	while (user_input != 0)
	{
		printf("please choose one of the followings:\n");
		printf("\t1.Change Password.\n");
		printf("\t2.Show me my Information.\n");
		printf("\t3.Delete My User account.\n");
		printf("\t4.Administator Options.\n");
		printf("0.Go Back\n");
		scanf("%d", &user_input);

		switch (user_input)
		{
		case(1):
			scanf("%c", &ch);
			change_user_password(user);
			printf("password changed successfully!\n");
			break;
		case(2):
			print_user(*user);
			break;
		case(3):
			printf("are you sure that you want to DELETE your account?(type in y/n)\n");
			scanf("%c", &op);
			scanf("%c", &op);
			if (op == 'y')
			{
				delete_user_from_list(user);
				user_input = 0;
				printf("you have been deleted from our system successfully!\n");
			}
			break;
		case(4):
			admin_options(user, lst);
			break;
		default:
			printf("INPUT IS NOT VALID!\n");
			break;
		}
	}

}