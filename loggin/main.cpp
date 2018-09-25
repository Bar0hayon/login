#include "loggin.h"



void main()
{
	UserList lst = load_user_list();
	User* user;
	int user_input=99;

	while (user_input != 0)
	{
		printf("WELCOME!\n");
		printf("please choose one of the followings:\n");
		printf("\t1.sign in\n");
		printf("\t2.register (new user)\n");
		printf("0.exit\n");
		scanf("%d", &user_input);

		switch (user_input)
		{
		case(1):
			user = sign_in(lst);
			if (user)
			{
				printf("Hi %s, you have signed in successfully!\n", user->first_name);
				user_settings(user, &lst);
			}
			else
				user_input = 0;
			break;
		case (2):
			registration(&lst);
			printf("you have registered successfuly!\n");
			break;
		case(0):
			break;
		default:
			printf("input is not valid!\n");
			break;
		}
	}
	printf("GOOD BYE!\n");

	save_user_list(lst);
	system("pause");
}