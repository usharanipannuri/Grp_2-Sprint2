#include<iostream>
#include<unistd.h>
#define limit 2

using namespace std;

void login()
{
	//Assigning loginAttempts initially
	int loginAttempt=0;
        string username,password;
        string choice;
        
        cout<<"Are you an admin(yes/no) : ";
        cin>>choice;
	
	if(choice=="yes")
	{
Admin:		//Asking for login with admin credentials
		cout<<"\n\t\tEnter login credentials for ADMIN PORTAL"<<endl;
                sleep(1);
                cout<<"\nEnter username : ";
		cin>>username;
		cout<<"Enter password : ";
                cin>>password;
		
		//Validating the admin credentials
		if(username=="Admin" && password=="polling")
		{
			cout<<"\n\t\t\tWELCOME TO ADMIN PORTAL"<<endl;
		}
		else
		{
			//checking loginAttempts
			if(loginAttempt<limit)
			{
				cout<<"\n\t\t\tError !! Enter valid credentials"<<endl;
				loginAttempt++;
				
				//choice to enter credentials once
				goto Admin;
			}

			//if admin credentials are invalid upto the limit
			else
			{
				cout<<"\n\t\t\t Sorry !! Too many attempts <You have no access to login>"<<endl;
				exit(0);
			}
		}
	}
	else
	{
User:
		//Asking for login user credentials
		cout<<"\n\t\tEnter login credentials for USER PORTAL"<<endl;
                sleep(1);
                cout<<"\nEnter username : ";
                cin>>username;
                cout<<"Enter password : ";
                cin>>password;
		
                //Validating the user credentials
                if(username=="User" && password=="2022")
                {
                        cout<<"\n\t\t\tWELCOME TO USER PORTAL"<<endl;
                }
                else
                {
			//Chekcing loginAttempts
                        if(loginAttempt<limit)
                        {
                                cout<<"\n\t\t   Error !! Enter valid credentials"<<endl;
                                loginAttempt++;

				//choice to enter credentials once
				goto User;
                        }

			//if user credentilas are invalid upto the loginAttempt
                        else
			{
				cout<<"\n\t\t\t Sorry !! Too many attempts <You have no access to login>"<<endl;
                                exit(0);
                        }
                }
	}
}
