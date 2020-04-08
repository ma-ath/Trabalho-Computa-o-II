/*
 * Universidade Federal do Rio de Janeiro
 * Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * Prof. Marcelo Luiz Drumond Lanza
 * EEL 270 - Computacao II - Turma 2015/2
 *
 * $Author$
 * $Date$
 * $Log$
 */

#include "tozcAcceptInvite.h"
#include "tozcChangeUserProfile.h"
#include "tozcLockUserAccount.h"
#include "tozcUnlockUserAccount.h"
#include "tozcGetUnlockingRequests.h"
#include "tozcGetUsers.h"
#include "tozcApproveRegistrationRequest.h"
#include "tozcChangeUserEmail.h"
#include "tozcConfirmEmailChange.h"
#include "tozcRejectInvite.h"

#include "tozcConfig.h"
#include "tozcFunctions.h"
#include "tozcTypes.h"
#include "tozcUserInterface.h"
#include "tozcError.h"
#include "tozcConst.h"

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


void
TozcShowCliHelp (tozcLanguageType);

tozcErrorType
TozcRunNcursesInterface (tozcLanguageType, char *);

typedef enum {
	f,
	t
	} boolean;


typedef enum {

	tozcArgumentEmail,
	tozcArgumentEmailConfirmation,
	tozcArgumentLanguage,
	tozcArgumentNickname,
	tozcArgumentUsername,
	tozcArgumentUsernameCheck,
	tozcArgumentUserOption,
	tozcArgumentProfileOption,
	tozcArgumentFriendEmail,
	tozcArgumentKey,
	} argumentIndex;


int main(int argc, char** argv)
{

  static struct option longOptions[] = 
    {
    {"help", 0, NULL, 'h'	},
    {"Ncurses", 0, NULL, 'N'	},
    {"Configure", 0, NULL, 'C'	},
    {"add", 0, NULL, 'a'},
    {"invite", 0, NULL, 'i'	},
    {"accept", 0, NULL, 't'	},
    {"reject", 0, NULL, 'j'	},
    {"new", 0, NULL, 'n'	},
    {"request", 0, NULL, 'q'	},
    {"Request", 0, NULL, 'Q'	},
    {"approve", 0, NULL, 'v'	},
    {"disapprove", 0, NULL, 'd'	},
    {"Unlocking", 0, NULL, 'U'	},
    {"lock", 0, NULL, 'l'	},
    {"unlock", 0, NULL, 'u'	},
    {"password", 0, NULL, 'p'	},
    {"reset", 0, NULL, 'r'	},
    {"modify", 0, NULL, 'm'	},
    {"edit", 0, NULL, 'e'	},
    {"confirm", 0, NULL, 'c'	},
    {"group", 0, NULL, 'g'	},
    {"show", 0, NULL, 's'	},
    {"Search", 0, NULL, 'S'	},
    {NULL, 0, NULL, 0	}
    };

  char* argumentNames[] = {
	"email",
	"confirm-email",
	"language",
	"nickname",
	"username",
	"confirm-username",
	"user",
	"profile",
	"friend-email",
	"key",
	NULL
	};


  int opt;
  int longIndex;

  int argumentIndex;
  char* argument, *argumentValue;
  tozcLanguageType lang = tozcEnglish;
  boolean langReceived;
  tozcErrorType erro;
  
  tozcNicknameListType *nicknameList;


  char nickname[TOZC_MAX_NICKNAME_SIZE] = "";
  
  char username[TOZC_MAX_USERNAME_SIZE];
  char usernameCheck[TOZC_MAX_USERNAME_SIZE];
  char email[TOZC_MAX_EMAIL_SIZE];
  char emailCheck[TOZC_MAX_EMAIL_SIZE];
  char friendEmail[TOZC_MAX_EMAIL_SIZE];
  char password[TOZC_MAX_PASSWORD_SIZE];
  char key[TOZC_MAX_KEY_SIZE];

  /*char passwordCheck[TOZC_MAX_PASSWORD_SIZE];*/
   
	opterr = 0;

  strcpy(username,"");
  strcpy(usernameCheck,"");
  strcpy(email,"");
  strcpy(emailCheck,"");

 

  tozcUserDataType* user;
  tozcUserDataType* newUser;
  tozcUserDataType* list;

  user = (tozcUserDataType*) malloc (sizeof(tozcUserDataType));
  newUser = (tozcUserDataType*) malloc (sizeof(tozcUserDataType));
  list = (tozcUserDataType*) malloc (sizeof(tozcUserDataType));
	/*preenchendo os campos*/

  strcpy(friendEmail,"");

  user->uid = getuid();
  strcpy(user->nickname,"");
  strcpy(user->password,"");
  strcpy(user->passwordCheck,"");
  user->profileType = 0;
  strcpy(user->name,"");
  strcpy(user->nameCheck,"");
  strcpy(user->email,"");
  strcpy(user->emailCheck,"");

  user->next = NULL;
  user->last = NULL;


  newUser->uid = getuid();
  strcpy(newUser->nickname,"");
  strcpy(newUser->password,"");
  strcpy(newUser->passwordCheck,"");
  newUser->profileType = 0;
  strcpy(newUser->name,"");
  strcpy(newUser->nameCheck,"");
  strcpy(newUser->email,"");
  strcpy(newUser->emailCheck,"");

  newUser->next = NULL;
  newUser->last = NULL;

/*Codigo*/
  while ((opt = getopt_long(argc,argv,GETOPT_LONG_VALID_CARACTERS,longOptions,&longIndex)) != -1)
	{

	switch(opt){

		case 'C':
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentEmail:
						strcpy(email,argumentValue);
						break;
					
					case tozcArgumentEmailConfirmation:
						strcpy(emailCheck,argumentValue);
						break;
					
					case tozcArgumentUsername:
						strcpy(username,argumentValue);
						break;

					case tozcArgumentUsernameCheck:
                                                strcpy(usernameCheck,argumentValue);
                                                break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

					/*default: printf("=(\n");
						break;*/
					}	
				optind++;

			}

			/*se eu receber todos os argumentos*/
		printf("Configure:\n");
			if (	(email[0] 		!= '\0')&&
				(emailCheck[0]		!= '\0')&&
				(username[0] 		!= '\0')&&
				(usernameCheck[0] 	!= '\0'))
				
				{
				if (TOZC_ADMINISTRATOR_USER_IDENTIFIER != getuid()) /*Se eu puder rodar o programa*/
					if (lang)
						printf("Voce nao pode rodar o programa\n");
					else
						printf("You cannot run this program\n");

				else

					{ /*Obter Senha*/
					if (lang)
						{
						strcpy(user->password,getpass("Senha: "));
						strcpy(user->passwordCheck,getpass("Confirmacao de Senha: "));

						strcpy(user->email,email);
						strcpy(user->emailCheck,emailCheck);
						strcpy(user->name,username);
						strcpy(user->nameCheck,usernameCheck);

						user->profileType = profileAdmin;
						
						erro = TozcAddUser(user);	
						if (erro)
							printf("Erro: %s\n",TozcGetCliErrorMessage(erro,lang));

						}
					else
						{
						strcpy(user->password,getpass("Senha: "));
                                                strcpy(user->passwordCheck,getpass("Confirmacao de Senha: "));
						
						strcpy(user->email,email);
						strcpy(user->emailCheck,emailCheck);
						strcpy(user->name,username);
						strcpy(user->nameCheck,usernameCheck);

						user->profileType = profileAdmin;

						erro = TozcAddUser (user);
						if (erro)
							printf("Erro: %s\n",TozcGetCliErrorMessage(erro,lang));
						}
					
					}	
				}



			else{
				printf("Use %s -C: <email> <email-confirmation> <username> <username-confirmation> [language]\n",argv[0]);
				exit(tozcNotAllArgumentsReceived);}
			/*se nao receber os argumentos*/	
			break; 

		case 'h':
			langReceived = 0;	
			while (optind < argc){ 
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
							
				if (argumentIndex == tozcArgumentLanguage){
					if (strcmp(argumentValue,"portuguese") == 0)
						{
						TozcShowCliHelp(tozcPortuguese);

						langReceived = 1;
						}
					else
						{
						TozcShowCliHelp(tozcEnglish);
		
						langReceived = 1;
						}
						}	
				optind++;
			}

			if (langReceived == 0){
				TozcShowCliHelp(tozcEnglish);}


			break;

		case 'N':
		langReceived = 0;
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
							
				if (argumentIndex == tozcArgumentLanguage){
					if (strcmp(argumentValue,"portuguese") == 0)
						{
						lang = tozcPortuguese;
						langReceived = 1;	
						}
					else
						{
						lang = tozcEnglish;
						langReceived = 1;
						}
				}

	
				if (argumentIndex == tozcArgumentNickname)
					{
					strcpy(nickname,argv[optind]);
					langReceived = 1;
					}
			optind++;
			}
			
			if (!langReceived){
				/*TozcRunNcursesInterface (lang,nickname);*/
				printf("ncurse in english \nnickname: %s\n",nickname);
				}
			else
				{
				/*TozcRunNcursesInterface (lang,nickname);*/
				printf("ncurse %d\nnickname: %s\n",lang,nickname);
				}
			break;

/*							AQUI									*/	

      case 'a':
        printf ("Add: \n");

		newUser->profileType = profileUser; /*default*/


			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentEmail:
						strcpy(newUser->email,argumentValue);
						break;
					
					case tozcArgumentEmailConfirmation:
						strcpy(newUser->emailCheck,argumentValue);
						break;
					
					case tozcArgumentUsername:
						strcpy(newUser->name,argumentValue);
						break;

					case tozcArgumentUsernameCheck:
                                                strcpy(newUser->nameCheck,argumentValue);
                                                break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
					case tozcArgumentProfileOption:
  				            if(!strcmp(argumentValue, "administrator"))
	        			        newUser->profileType = profileAdmin;
        	  			    	else if (!strcmp(argumentValue,"user"))
				                newUser->profileType = profileUser;
							else
				              {
					                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcProfileTypeNotFound,lang));
  					              exit(tozcProfileTypeNotFound);
				              }
				                break;
                    

				          }
				          optind++;
				        }
	
			if (	(user->nickname[0] 	!= '\0')&&
				(newUser->name[0] 	!= '\0')&&
				(newUser->nameCheck[0] 	!= '\0')&&
				(newUser->email[0] 	!= '\0')&&
				(newUser->emailCheck[0] != '\0'))
		{
       		
		        strcpy(user->password,getpass("Admin password: "));
			erro = TozcAuthenticateUser (user);		

	        	if ( erro || (user->profileType!=profileAdmin))
		        {
		          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
	        	  exit(tozcPermissionDenied);
		        }
        
			strcpy(newUser->password,getpass("New User's Password: "));
	        	strcpy(newUser->passwordCheck,getpass("Confirm New User's Password: "));
        
		        newUser->uid = 0;
		        newUser->next = NULL;
	        	newUser->last = NULL;

		        erro = TozcAddUser(newUser); 
		        if (erro)
	        	{
		          printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
		          exit(erro);
			}
		}
		else{
			printf("-a | --add  user=<apelido> username=<nome completo> confirm-username=<nome completo> email=<endereço eletrônico> confirm-email=<endereço eletrônico> profile=<administrator | user> [language=english | portuguese]\n");
		        printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}	
       	
	      break;



      case 'i':
        printf ("Invite:\n");
        newUser->profileType = profileUser; /*default*/
	
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentEmail:
						strcpy(newUser->email,argumentValue);
						break;
					
					case tozcArgumentEmailConfirmation:
						strcpy(newUser->emailCheck,argumentValue);
						break;
					
					case tozcArgumentUsername:
						strcpy(newUser->name,argumentValue);
						break;

					case tozcArgumentUsernameCheck:
                                                strcpy(newUser->nameCheck,argumentValue);
                                                break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
					case tozcArgumentProfileOption:
  				            if(!strcmp(argumentValue, "administrator"))
	        			        newUser->profileType = profileAdmin;
        	  			    	else if (!strcmp(argumentValue,"user"))
				                newUser->profileType = profileUser;
							else
				              {
					                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcProfileTypeNotFound,lang));
  					              exit(tozcProfileTypeNotFound);
				              }
				                break;
                    

				          }
				          optind++;
				        }
	
			if (	(user->nickname[0] 	!= '\0')&&
				(newUser->name[0] 	!= '\0')&&
				(newUser->nameCheck[0] 	!= '\0')&&
				(newUser->email[0] 	!= '\0')&&
				(newUser->emailCheck[0] != '\0'))
		{

        strcpy(user->password,getpass("Your password: ")); 
        if (TozcAuthenticateUser (user))
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }
        if ((user->profileType == profileUser) && (newUser->profileType !=profileUser))
        {
          printf("Error: %s\n\t<users-can-only-invite-other-users>",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }
 
        newUser->password[0] = '\0';
        newUser->passwordCheck[0] = '\0';
        
        newUser->uid = 0;
        newUser->next = NULL;
        newUser->last = NULL;

        erro = TozcAddUser(newUser); 
        if (erro)
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
          exit(erro);
        }

		}
		else
			{printf("-i | --invite user=<apelido> username=<nome completo> confirm-username=<nome completo> email=<endereço eletrônico> confirm-email=<endereço eletrônico> profile=<administrator | user> [language=english | portuguese]\n");
		        printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));	
			exit(tozcNotAllArgumentsReceived);}
       
      break;



      case 't':
        printf ("Accept Invite\n");

			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
                   		          }
				          optind++;
				        }

        if(user->nickname[0]	==	'\0')
        {
	  printf("-t | --accept user=<apelido> [language=english | portuguese]\n");
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
          exit(tozcNotAllArgumentsReceived);
        }

        strcpy(password,getpass("Please enter your temporary password: ")); 
        strcpy(newUser->password,getpass("Please enter your new password: ")); 
        
	strcpy(newUser->passwordCheck,getpass("Please re-enter your new password: "));
 
        erro = TozcAcceptInvite(password,newUser);
	/*erro = ok;*/
        if (erro)
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
          exit(erro);
        }
             
      break;

	
      case 'j':
        printf ("Reject invite\n");

			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
                   		          }
				          optind++;
				        }
        if(user->nickname[0]	==	'\0')
        {
	  
	printf("-j | --reject user=<apelido> [language=english | portuguese]\n");
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
          exit(tozcNotAllArgumentsReceived);
        }

        strcpy(password,getpass("Please enter your temporary password: ")); 
         
	/*erro = TozcRejectInvite(user->nickname, password)*/
	erro = ok;
	if (erro)
        {
	  printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
          exit(erro);
        }
 
      break;


      case 'n':
        printf ("New: \n");

			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentEmail:
						strcpy(newUser->email,argumentValue);
						break;
					
					case tozcArgumentEmailConfirmation:
						strcpy(newUser->emailCheck,argumentValue);
						break;
					
					case tozcArgumentUsername:
						strcpy(newUser->name,argumentValue);
						break;

					case tozcArgumentUsernameCheck:
                                                strcpy(newUser->nameCheck,argumentValue);
                                                break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentFriendEmail:
            					strcpy(friendEmail,argumentValue);
            					break;
          
				          }
				          optind++;
				        }
	
			if (	(friendEmail[0] 	!= '\0')&&
				(newUser->name[0] 	!= '\0')&&
				(newUser->nameCheck[0] 	!= '\0')&&
				(newUser->email[0] 	!= '\0')&&
				(newUser->emailCheck[0] != '\0'))
			{	

 
        		newUser->profileType = profileUser;
			
			erro = ok; /*Temporario*/
			/*erro = TozcRequestRegistration(friendEmail,newUser);*/
			erro = ok; /*Temporario*/
			if (erro)
   	    		{
                   	 printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                 	 exit(erro);
			}

        	

			}
		else{
			printf("-n | --new username=<nome completo> confirm-username=<nome completo> email=<endereço eletrônico> confirm-email=<endereço eletrônico> friend-email=<endereço eletrônico> [language=english | portuguese]\n");
	                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
	

        break;
	
      case 'q':
        printf ("Request per user\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
                   		          }
				          optind++;
				        }
        	if(user->nickname[0] == '\0')
	        {
	          printf("Error: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
	          exit(tozcNotAllArgumentsReceived);
	        }

	        strcpy(user->password,getpass("Your password: ")); 

        	if (TozcAuthenticateUser (user))
	        {
	          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
	          exit(tozcPermissionDenied);
	        }
                 
   		/*TozcGetPendingRegistrationRequestsPerUser(user->nickname,&list); ? */
			erro = ok; /*Temporario*/
                        if (erro)
                        {
			printf("-q | --request user=<apelido> [language=english | portuguese]\n");
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        } 
	      break;


      case 'Q':
        printf ("Request all\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
                   		          }
				          optind++;
				        }
        	if(user->nickname[0]== '\0')
	        {
		  printf("-Q | --Request user=<apelido> [language=english | portuguese]\n");
	          printf("Error: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
	          exit(tozcNotAllArgumentsReceived);
	        }

	        strcpy(user->password,getpass("Your password: ")); 

        	if ((TozcAuthenticateUser (user)) || (user->profileType!=profileAdmin))
	        {
	          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
	          exit(tozcPermissionDenied);
	        }
                 
   		/*erro = TozcGetPendingRegistrationRequests(&list); ? */
			erro = ok; /*Temporario*/
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
 
	      break;

      case 'v':
        printf ("Approve:\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

      				          }
				          optind++;
				        }

        
			if (	(newUser->nickname[0] 	!= '\0')&&
				(user->nickname[0] 	!= '\0'))
			{	

	strcpy(user->password,getpass("Your password: ")); 
        if (TozcAuthenticateUser (user))
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }


			erro = TozcApproveRegistrationRequest (newUser->nickname,user->nickname);
			/*erro = ok; Temporario*/
                        if (erro)
                        {
                         printf("Error(#%d): %s\n",erro,TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
       			}
		else{ 
			printf(" -v | --approve user=<apelido> nickname=<apelido> [language=english | portuguese]\n");
	                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
                     
      break;


       case 'd':
        printf ("Disapprove\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

      				          }
				          optind++;
				        }

        
			if (	(newUser->nickname[0] 	!= '\0')&&
				(user->nickname[0] 	!= '\0'))
			{	

	strcpy(user->password,getpass("Your password: ")); 
        if (TozcAuthenticateUser (user))
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }


			/*erro = TozcDisapproveRegistrationRequest (newUser->nickname,user->nickname); */
			erro = ok; /*Temporario*/
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
			}
		else{ 
			printf("-d | --disapprove user=<apelido> nickname=<apelido> [language=english | portuguese]\n");
	                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
                break;

         case 'U':
		printf("Unlocking\n"); 
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

      				          }
				          optind++;
				        }

        
			if (user->nickname[0] 	!= '\0')
			{	

	strcpy(user->password,getpass("Your password: "));
	if ((TozcAuthenticateUser (user)) || (user->profileType))
        {
	  printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }
			erro = TozcGetUnlockingRequests (user->nickname,&nicknameList);
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
			

       			}
		else{ 
			printf(" -U | --Unlocking user=<apelido>  [language=english | portuguese]\n");
	                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
		break;


      case 'l':
        printf ("Lock\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

      				          }
				          optind++;
				        }

        
			if (user->nickname[0] 	!= '\0')
			{	
        strcpy(user->password,getpass("Your password: ")); 
        if ((TozcAuthenticateUser (user)) || ((user->profileType)&&(newUser->nickname[0] != '\0')))
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }

		if(newUser->nickname[0] == '\0') strcpy(newUser->nickname,user->nickname); 

		erro = TozcLockUserAccount(user->nickname,newUser->nickname);
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }

			}
		else{
			printf("-l | --lock user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n"); 
	                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
       		break;


      case 'u':
        printf ("Unlocking\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

      				          }
				          optind++;
				        }

        
			if (user->nickname[0] 	!= '\0')
			{	
        	strcpy(user->password,getpass("Your password: ")); 
	        if ((TozcAuthenticateUser (user)) || ((user->profileType)&&(newUser->nickname[0] != '\0')))
	        {

	          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
	          exit(tozcPermissionDenied);
	        }

	
		if(newUser->nickname[0] == '\0') strcpy(newUser->nickname,user->nickname); 

		erro = TozcUnlockUserAccount(user->nickname,newUser->nickname);

                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }


			}
			else{
		printf("-u | --unlock user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n");
       	               printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
 			exit(tozcNotAllArgumentsReceived);}

       			break;




      case 'p':
        printf ("Password\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

      				          }
				          optind++;
				        }

        
			if (user->nickname[0] 	!= '\0')
			{	
        strcpy(user->password,getpass("Your password: ")); 
        if ((TozcAuthenticateUser (user)) || ((user->profileType)&&(newUser->nickname[0] != '\0'))) 
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }

        strcpy(password,getpass("Old password: ")); 
        strcpy(newUser->password,getpass("New password: ")); 
        strcpy(newUser->passwordCheck,getpass("Confirm New password: "));


	/*erro = TozcChangeUserPassword(user->nickname,newUser->nickname,password,newUser->password,newUser->passwordCheck); */
	 erro = ok; /*Temporario*/
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
 
			}
			else{
			printf("-p | --password  user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n");
	               printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
       			break;

 

      case 'r':
        printf ("Reset password\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
	       				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

      				          }
				          optind++;
				        }

        
			if (user->nickname[0] 	!= '\0')
			{	

		/*erro = TozcResetUserPassword(user->nickname);*/

			erro = ok; /*Temporario*/
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
 
			}
			else{
			printf("-r | --reset user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n");
	                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
       			break;


      case 'm':
        printf ("Modify\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;
					
					case tozcArgumentUsername:
						strcpy(newUser->name,argumentValue);
						break;

					case tozcArgumentUsernameCheck:
                                                strcpy(newUser->nameCheck,argumentValue);
                                                break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
                   		          }
				          optind++;
				        }
	
			if ((	user->nickname[0]        != '\0')&&
			    (   newUser->name[0]      	 != '\0')&&
			    (   newUser->nameCheck[0]    != '\0'))
		{

        	strcpy(user->password,getpass("Admin password: ")); 
        	if ((TozcAuthenticateUser (user)) || ((user->profileType)&&(newUser->nickname[0] != '\0')))
	        {
	          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
	          exit(tozcPermissionDenied);
	        }

		/*erro = TozcChangeUserName(user->nickname,newUser->nickname,newUser->name,newUser->nameCheck);*/

		erro = ok; /*Temporario*/
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
		}
		else{
			printf("-m | --modify user=<apelido> [nickname=<apelido>] username=<nome completo> confirm-username=<nome completo> [language=english | portuguese]\n");
		        printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));	
			exit(tozcNotAllArgumentsReceived);}
      break;



      case 'e':
        printf ("Opcao edit selecionada\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentNickname:
						strcpy(newUser->nickname,argumentValue);
						break;
					
					case tozcArgumentEmail:
						strcpy(newUser->email,argumentValue);
						break;

					case tozcArgumentEmailConfirmation:
                                                strcpy(newUser->emailCheck,argumentValue);
                                                break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
                   		          }
				          optind++;
				        }
	
			if ((	user->nickname[0] 	!= '\0')&&
			    (	newUser->email[0]	!= '\0')&&
			    (   newUser->emailCheck[0]  != '\0'))
		{

        	strcpy(user->password,getpass("Your password: ")); 
        	if ((TozcAuthenticateUser (user)) || ((user->profileType)&&(newUser->nickname[0] != '\0')))	
	        {
	          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
	          exit(tozcPermissionDenied);
	        }

		erro = TozcChangeUserEmail(user->nickname,newUser->nickname,newUser->email,newUser->emailCheck);
		/*erro = ok;  Temporario*/
                        if (erro)
                        {
                         printf("Error(#%d): %s\n",erro,TozcGetCliErrorMessage(erro,lang));
                         printf("Error(#%d): %s\n",errno,strerror(errno));
                         exit(erro);
                        }	

		}
		else{
			printf("-e | --edit user=<apelido> [nickname=<apelido>] email=<endereço eletrônico> confirm-email=<endereço eletrônico> [language=english | portuguese]\n");
		        printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
			exit(tozcNotAllArgumentsReceived);}
      break;


      case 'c':
        printf ("Confirm\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;

					case tozcArgumentKey:
						strcpy(key,argumentValue);
						break;
          
                   		          }
				          optind++;
				        }
	
			if (	user->nickname[0] 	!= '\0')
		{
		        strcpy(user->password,getpass("Your password: ")); 
		        if ((TozcAuthenticateUser (user)))
		        {
		          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
		          exit(tozcPermissionDenied);
		        }
		erro = TozcConfirmEmailChange(user->nickname,key);
		/*erro = ok; Temporario*/
                        if (erro)
                        {
                         printf("Error(#%d): %s\n",erro,TozcGetCliErrorMessage(erro,lang));
                         printf("Error(#%d): %s\n",errno,strerror(errno));
                         exit(erro);
                        }

		}
			else{	printf(" -c | --confirm user=<apelido> key=<chave>  [language=english | portuguese]\n");
			        printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
				exit(tozcNotAllArgumentsReceived);}	
			
      break;



      case 'g':
        printf ("Group:\n");
        newUser->profileType = profileUser; /*default*/
	
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentNickname:
                                                strcpy(newUser->nickname,argumentValue);
                                                break;

					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
           
					case tozcArgumentProfileOption:
  				            if(!strcmp(argumentValue, "administrator"))
	        			        newUser->profileType = profileAdmin;
        	  			    	else if (!strcmp(argumentValue,"user"))
				                newUser->profileType = profileUser;
							else
				              {
					                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcProfileTypeNotFound,lang));
  					              exit(tozcProfileTypeNotFound);
				              }
				                break;
                    

				          }
				          optind++;
				        }
	
			if (	user->nickname[0] 	!= '\0')	
			{
        strcpy(user->password,getpass("Admin password: ")); 
        if ((TozcAuthenticateUser (user)) || (user->profileType))
        {
          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
          exit(tozcPermissionDenied);
        }
		
		erro = TozcChangeUserProfile(user->nickname,newUser->nickname);
		/*erro = ok;  Temporario*/
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }

			}
				else{
				printf("-g | --group user=<apelido> nickname=<apelido> profile=<administrator | user> [language=english | portuguese]\n");
				        printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
					exit(tozcNotAllArgumentsReceived);}	

      break;


      case 's':
        printf ("Show selecionada\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
				
					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;
				          }
				          optind++;
				        }
			erro = TozcGetUsers(&list);

                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }

	
        while(list)
        {
          printf("uid:%llu\n",list->uid);
          printf("nickname:%s\n",list->nickname);
          printf("profileType:%d\n",list->profileType);
          printf("name:%s\n",list->name);
          printf("email:%s\n\n",list->email);
          list=list->next;
        }






      break;


      case 'S':
        printf ("Search\n");
			while (optind < argc) {
				argument = argv[optind];
				argumentIndex = getsubopt(&argument,argumentNames,&argumentValue);
				
				switch(argumentIndex) {
					case tozcArgumentLanguage:
						lang = TozcGetLanguageIndex(argumentValue);
						break;

            				case tozcArgumentUsername:
            					strcpy(newUser->name,argumentValue);
            					break;

					case tozcArgumentEmail:
						strcpy(newUser->email,argumentValue);
						break;
          
					case tozcArgumentProfileOption:
  				            if(!strcmp(argumentValue, "administrator"))
	        			        newUser->profileType = profileAdmin;
        	  			    	else if (!strcmp(argumentValue,"user"))
				                newUser->profileType = profileUser;
							else
				              {
					                printf("Erro: %s\n",TozcGetCliErrorMessage(tozcProfileTypeNotFound,lang));
  					              exit(tozcProfileTypeNotFound);
				              }
				                break;
            				case tozcArgumentUserOption:
            					strcpy(user->nickname,argumentValue);
            					break;
                   		          }
				          optind++;
				        }
	
			if ((	user->nickname[0] 	!= '\0'))
					{
			strcpy(user->password,getpass("Admin password: "));
	        	if (TozcAuthenticateUser (user))
		        {
		          printf("Error: %s\n",TozcGetCliErrorMessage(tozcPermissionDenied,lang));
		          exit(tozcPermissionDenied);
		        }	

		        /*erro = TozcSearchUsers(newUser->name,newUser->email,newUser->profileType,&list);*/
			erro = ok; /*Temporario*/
                        if (erro)
                        {
                         printf("Error: %s\n",TozcGetCliErrorMessage(erro,lang));
                         exit(erro);
                        }
		                        }
                                else{
		printf("-S | --Search [username=<nome completo ou parte do nome>] [email=<endereço eletrônico ou parte do endereço eletrônico>] [profile=<administrator | user> [language=english | portuguese]\n");
                                        printf("Erro: %s\n",TozcGetCliErrorMessage(tozcNotAllArgumentsReceived,lang));
					exit(tozcNotAllArgumentsReceived);}
		break;



 

	default:
			printf("Opcao invalida\n");
			exit(INVALID_GETOPT_LETTER);
			break;
		};
	};

return 0;
}






void
TozcShowCliHelp (tozcLanguageType lang)
{
printf("Adicionar um usuário ao sistema: -a | --add  user=<apelido> username=<nome completo> confirm-username=<nome completo> email=<endereço eletrônico> confirm-email=<endereço eletrônico> profile=<administrator | user> [language=english | portuguese]\n");
printf("\n");
printf("Convidar um usuário para participar do sistema: -i | --invite user=<apelido> username=<nome completo> confirm-username=<nome completo> email=<endereço eletrônico> confirm-email=<endereço eletrônico> profile=<administrator | user> [language=english | portuguese]\n");
printf("\n");
printf("Aceitar um convite para participar do sistema: -t | --accept user=<apelido> [language=english | portuguese]\n");
printf("\n");
printf("Rejeitar um convite para participar do sistema: -j | --reject user=<apelido> [language=english | portuguese]\n");
printf("\n");
printf("Solicitar cadastramento no sistema: -n | --new username=<nome completo> confirm-username=<nome completo> email=<endereço eletrônico> confirm-email=<endereço eletrônico> friend-email=<endereço eletrônico> [language=english | portuguese]\n");
printf("\n");
printf("Obter a lista de solicitações de cadastramento no sistema sob responsabilidade deste usuário: -q | --request user=<apelido> [language=english | portuguese]\n");
printf("\n");
printf("Obter a lista com todas as solicitações de cadastramento no sistema: -Q | --Request user=<apelido> [language=english | portuguese]\n");
printf("\n");
printf("Aprovar um pedido de cadastramento no sistema: -v | --approve user=<apelido> nickname=<apelido> [language=english | portuguese]\n");
printf("\n");
printf("Reprovar um pedido de cadastramento no sistema: -d | --disapprove user=<apelido> nickname=<apelido> [language=english | portuguese]\n");
printf("\n");
printf("Obter a lista de solicitações de desbloqueio: -U | --Unlocking user=<apelido>  [language=english | portuguese]\n");
printf("\n");
printf("Bloquear a conta de um usuário do sistema: -l | --lock user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n");
printf("\n");
printf("Desbloquear a conta de um usuário do sistema: -u | --unlock user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n");
printf("\n");
printf("Alterar a senha de um usuário do sistema: -p | --password  user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n");
printf("\n");
printf("Criar uma senha temporária para um usuário do sistema: -r | --reset user=<apelido> [nickname=<apelido>] [language=english | portuguese]\n");
printf("\n");
printf("Alterar o nome de um usuário do sistema: -m | --modify user=<apelido> [nickname=<apelido>] username=<nome completo> confirm-username=<nome completo> [language=english | portuguese]\n");
printf("\n");
printf("Alterar o endereço eletrônico de um usuário do sistema: -e | --edit user=<apelido> [nickname=<apelido>] email=<endereço eletrônico> confirm-email=<endereço eletrônico> [language=english | portuguese]\n");
printf("\n");
printf("Confirmar a alteração do endereço eletrônico: -c | --confirm user=<apelido> key=<chave>  [language=english | portuguese]\n");
printf("\n");
printf("Alterar o perfil de um usuário: -g | --group user=<apelido> nickname=<apelido> profile=<administrator | user> [language=english | portuguese]\n");
printf("\n");
printf("Obter todos os usuários: -s | --show [language=english | portuguese]\n");
printf("\n");
printf("Buscar informações sobre usuários: -S | --Search [username=<nome completo ou parte do nome>] [email=<endereço eletrônico ou parte do endereço eletrônico>] [profile=<administrator | user> [language=english | portuguese]\n");

}
/* $RCSFile$ */ 
