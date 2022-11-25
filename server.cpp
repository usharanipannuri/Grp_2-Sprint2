#include<iostream>
#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<fstream>
#include<pthread.h>
#include "Logger.h"
#include "header.h"
#define size 2000
#define Voter_size 255
#define Votes_size 200

using namespace std;

int port_number = 2007;

void *communicate (void *message)
{
	int client_sock =  (long) message;
	char server_message[size], client_message[size];
	
	//Initialization of Buffers        
	memset(server_message,'\0',sizeof(server_message));
	memset(client_message,'\0',sizeof(client_message));
    	
	//Receive Name/Voter's ID from the client
	if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
	{
		cout<<"1 Receive Failed. Error!!!!!\n";
		Logger::Error(__FILE__,__LINE__,"SERVER ERROR");

	}
	
	//Copying Voter ID and name into credentials
	char credentials[Voter_size];
	strcpy(credentials, client_message);

	//remove newline character
	char *newline;
	newline = strchr(credentials,'\n');
	if(newline != NULL)
		*newline = '\0';
	
	// Read Voters_List.txt and match Voter's ID and name, if match_exists then allow to vote else end thread    
    	char buff[Voter_size];
    	ifstream ifile;
	string line;

    	ifile.open("Voters_List.txt");
	int voter_found = 0;
	
	//Checking Voter ID and name with the Voters_List
    	while(getline(ifile, line))
	{	
		//remove newline and carriage return characters
		char *newline,*carriage_return;
		strcpy(buff,line.c_str());
		newline = strchr(buff,'\n');
		if(newline != NULL)
			*newline = '\0';

		carriage_return = strchr(buff,'\r');
		if(carriage_return != NULL)
			*carriage_return = '\0';

	        //Comparing Voter details with credentials
		if (strcmp(credentials, buff) == 0)
		{
			voter_found = 1;
			break;
		}		
	}
	
	//Closing Voters_List textfile
	ifile.close();

	// Check if this client has already casted a vote or note. If yes, then end thread
	if (voter_found == 1)
	{
		//Initialization of Buffers        
		memset(server_message,'\0',sizeof(server_message));
		memset(client_message,'\0',sizeof(client_message));

		//Send a greeting message back to client
		strcpy(server_message,  "\n\t\t\t\t********************************************\n"
				        "\t\t\t                       WELCOME VOTER               \n "
					"\t\t\t\t********************************************\n\n\n");

		if (send(client_sock, server_message, strlen(server_message),0)<0)
		{
			cout<<"1 Send Failed. Error!!!!!\n";
			Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
		}	       

	
                //Open Votes textfile to compare Voter details
        	ifile.open("Votes.txt");
		int vote_casted = 0;
		
		while(getline(ifile,line))
		{
			//Checking sequentially in Votes textfile
			char *temp_comma;
			strcpy(buff,line.c_str());
			temp_comma = strchr(buff,',');
			if(temp_comma != NULL)
				*temp_comma = '\0';
						
			if (strcmp(credentials, buff) == 0)
			{
				vote_casted = 1;
				break;
			}		
		}
		

		//Closing Votes textfile
	    	ifile.close();
		
		//vote not casted previously, so allow to cast vote
		if (vote_casted == 0)
		{
			//display candidates list
			ifile.open("Candidates_List.txt");
								
			char candidates[size];
			
			strcpy(candidates, "\t\tFollowing are the list of candidates and their symbols: \n\n");
			
			//Reading all the data from Candidate_List textfile
			while(getline(ifile,line))
			{
				strcpy(buff,line.c_str());
				strcat(candidates,buff);
				
			}

			//Copying candidate details into server message
			strcpy(server_message, candidates);

			//Sending Candidate details to client
			if (send(client_sock, server_message, strlen(server_message),0)<0)
			{
				cout<<"2 Send Failed. Error!!!!!\n";
				Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			}

			//Closing Candidate_List textfile
			ifile.close();
						
			//Initialization of Buffers        
			memset(server_message,'\0',sizeof(server_message));
			memset(client_message,'\0',sizeof(client_message));

			
			//Receive candidate's symbol from client
			if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
			{
				cout<<"2 Receive Failed. Error!!!!!\n";
				Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			}
			
			//write this vote in Votes.txt
                        char symbol[Votes_size];
			strcpy(symbol, client_message);

			char vote[Votes_size];
			strcpy(vote, credentials);
			strcat(vote, ",");
			strcat(vote, symbol);
			
			//Opening Votes textfile to store Casted voter details
			ofstream ofile;
            		ofile.open("Votes.txt", ios::app);

			ofile << vote <<endl;
                        
			//Closing textfile
            		ofile.close();
			
			//Intialization of Buffers        
			memset(server_message,'\0',sizeof(server_message));
			memset(client_message,'\0',sizeof(client_message));
                        
			//Sending casted message to client
			strcpy(server_message, "\n\t\t-------Congratulations! You have succesfully casted your vote.------\n");

			if (send(client_sock, server_message, strlen(server_message),0)<0)
			{
				cout<<"3 Send Failed. Error!!!!!\n";
				Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			}	       						
		}

		else
		{
			//Initialization of Buffers        
			memset(server_message,'\0',sizeof(server_message));
			memset(client_message,'\0',sizeof(client_message));
                        
			//Sending can't vote again message to client
			strcpy(server_message, "\n\t\t     Sorry! You have already casted your vote. You can't vote again....\n\n");	

			if (send(client_sock, server_message, strlen(server_message),0)<0)
			{
				cout<<"4 Send Failed. Error!!!!!\n";
				Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			}			      
		}	
        }

	else
	{
		//Initialization of Buffers        
		memset(server_message,'\0',sizeof(server_message));
		memset(client_message,'\0',sizeof(client_message));
                
		//Sending a message of name is not in the list to client
		strcpy(server_message, "\n\t\t     Sorry! Your name is not in the authorized voters list....\n\n");

		if (send(client_sock, server_message, strlen(server_message),0)<0)
		{
			cout<<"5 Send Failed. Error!!!!!\n";
			Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
		}	       
	}
	
	//Declaration of result

	ifstream afile;

	string symbol1="Bat";
	string symbol2="Tiger";
	string symbol3="Arrow";

	int symbol1count=0; //bat
	int symbol2count=0; //tiger
	int symbol3count=0; //arrow

	char arr[Votes_size];
	string Name_VoterId,Vote_Symbol;

	afile.open("Votes.txt");
	while(getline(afile, line))
	{
		stringstream strm(line);
	        getline(strm,Name_VoterId,',');
	        getline(strm,Vote_Symbol);
	
		if( (symbol1 ==Vote_Symbol))
		{
			symbol1count++;
		}
		else if( (symbol2 == Vote_Symbol))
		{
			symbol2count++;
		}
		else if( (symbol3 == Vote_Symbol))
		{
			symbol3count++;
		}
	}

	if(symbol1count>symbol2count && symbol1count>symbol3count)
	{
		//Bat is the winner
		sleep(1);
		strcpy(server_message,"\n\t\t\t********Live Result : BAT IS THE WINNER*********\n\n" );
                if (send(client_sock, server_message, strlen(server_message),0)<0)
	        {
			cout<<" Send Failed. Error!!!!!\n";
			Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
		}
	}
	else if (symbol2count>symbol1count && symbol2count>symbol3count)
	{
		//Tiger is the winner
		sleep(1);
		strcpy(server_message,"\n\t\t\t********Live Result : TIGER IS THE WINNER*********\n\n" );
		if (send(client_sock, server_message, strlen(server_message),0)<0)
		{
			cout<<" Send Failed. Error!!!!!\n";
		        Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
		}
	}
	else if (symbol3count>symbol1count && symbol3count>symbol2count)
	{
		//Arrow is the winner
		sleep(1);
		 strcpy(server_message,"\n\t\t\t*********Live Result : ARROW IS THE WINNER********\n\n" );
		 if (send(client_sock, server_message, strlen(server_message),0)<0)
		 {
			 cout<<" Send Failed. Error!!!!!\n";
		         Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
		 }
	}
	else
	{
		//Tie situation
		sleep(1);
		strcpy(server_message,"\n\t\t\t*******Live Result : TIE SITUATION*******\n\n" );
		if (send(client_sock, server_message, strlen(server_message),0)<0)
		{
			cout<<" Send Failed. Error!!!!!\n";
		        Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
		}
	}

	close(client_sock);
	pthread_exit(NULL); 

} //closing the communicate funcion

int main(void)
{

    	login();

	int socket_desc, client_sock, client_size; 
	struct sockaddr_in server_addr, client_addr;
	char server_message[size], client_message[size];
	
	//Initialization of  Buffers        
	memset(server_message,'\0',sizeof(server_message));
	memset(client_message,'\0',sizeof(client_message));
	
	//Creating Socket	
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	
	if(socket_desc < 0)
	{
		cout<<"Could Not Create Socket. Error!!!!!\n";
		Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
		return -1;
	}
	
	cout<<"Socket Created\n";
	
	//Binding IP and Port to socket        
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
	{
			cout<<"Bind Failed. Error!!!!!\n";
			Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			return -1;
	}        
	
	cout<<"Bind Done\n";
	Logger::Info(__FILE__,__LINE__,"SERVER and CLIENT Bind is done" );
	
	//Put the socket into Listening State        
	if(listen(socket_desc, 1) < 0)
	{
			cout<<"Listening Failed. Error!!!!!\n";
			Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			return -1;
	}
	
	cout<<"Listening for Incoming Connections.....\n";
	Logger::Info(__FILE__,__LINE__,"SERVER Listening for Incoming Connections.... ");
	
	while (1)
	{		
		//Accept the incoming Connections	
		socklen_t len = sizeof(client_addr);
		client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &len);
	
		if (client_sock < 0)
		{
			cout<<"Accept Failed. Error!!!!!!\n";
			Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			return -1;
		}
		
		printf("Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		Logger::Info(__FILE__,__LINE__,"SERVER Client Connected with IP: %s and Port No: ");
		
		strcpy(client_message, "Connection Established!\n\n");

		// Send the connection message back to client
		if (send(client_sock, client_message, strlen(client_message),0)<0)
		{
			cout<<"5 Send Failed. Error!!!!!\n";
			Logger::Error(__FILE__,__LINE__,"SERVER ERROR");
			return -1;
		}
		
		//Initialization of Buffers        
		memset(server_message,'\0',sizeof(server_message));
		memset(client_message,'\0',sizeof(client_message));
		
		pthread_t thread1;

			
		//create new thread for each client
		pthread_create(&thread1,NULL,communicate,(void*)client_sock);
	    
          }
            
	//Closing the Socket        
	close(socket_desc);
	return 0;       
}
