/* 
  * Universidade Federal do Rio de Janeiro 
  * Escola Politecnica 
  * Departamento de Eletronica e de Computacao 
  * Prof. Marcelo Luiz Drumond Lanza 
  * EEL 270 - Computacao II - Turma 2015/2
  * 
  * Autor: Matheus Silva de Lima
  * $Author$ 
  * $Date$ 
  * $Log$
  *
  *
*/




#define _XOPEN_SOURCE            600
#include "tozcTypes.h"
#include "tozcError.h"
#include "tozcFunctions.h"
#include "tozcConst.h" 
#include "tozcConfig.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include "Libraries/sendmail.h"
#include "tozcAcceptInvite.h"



tozcErrorType 
TozcAcceptInvite (char *passwordTemp, tozcUserDataType *user)
{

FILE *readDataFile;
FILE *writeDataFile;
FILE *readAbeyFile;
FILE *writeAbeyFile;
char dataFileName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char abeyFileName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char *passwordCrypt;
char *nickname;
char *stringId;
char *email;
char *userReadname;
char *profile;  
tozcCryptAlgorithms algorithm;
unsigned erroMail;
tozcUserIdentifierType id;
char buffer[TOZC_MAX_BUFFER_SIZE+1];
char bufferMod[TOZC_MAX_BUFFER_SIZE+1];
char *verification;
unsigned foundUser,userReadW;
time_t timeExp,readTimeExp;
int readLine;
tozcUserDataType *userRead;
tozcErrorType erro;

 	userRead = (tozcUserDataType*)malloc(sizeof(tozcUserDataType *));

  	if (user->nickname==NULL)
    		return acceptInviteNoNickname;
  	if (passwordTemp==NULL)
    		return acceptInviteNoPassword;
  	if (user->password==NULL)
    		return acceptInviteNoPassword;
  	if (user->passwordCheck==NULL)
    		return acceptInviteNoPassword;
  

  	erro = TozcCheckStringField (user->password, TOZC_PASSWORD_VALID_CHARS, TOZC_MIN_PASSWORD_SIZE, TOZC_MAX_PASSWORD_SIZE);
  	if (erro)
    		return erro;
  	if (strcmp(user->password,user->passwordCheck))
    		return acceptInvitePasswordNotCheck;

  	algorithm = sha512;

  	erro = TozcEncodePasswordWithSpecificAlgorithm (user->password,algorithm ,user->password);
  	if (erro)
    		return erro;
  
 
  	snprintf(dataFileName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);
  
  	readDataFile = fopen(dataFileName,"r");
  	if (readDataFile == NULL)
    		return acceptInviteOpeningReadFile;
 
  	writeDataFile = fopen("temp3","w");
  	if (writeDataFile == NULL){
    		fclose(readDataFile);
    		return acceptInviteOpeningReadFile;
  	}

  foundUser = 0 ;

  	while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, readDataFile)) != NULL){ 
    		if(buffer[strlen(buffer)-1]!='\n'){
      			fclose(readDataFile);
      			fclose(writeDataFile);
      			remove("temp3");
      			return acceptInviteInvalidUser;
    		}
  
    		strcpy(bufferMod,buffer);
    		stringId = strtok(bufferMod, ":");

    		nickname = strtok(NULL,":");
    		passwordCrypt = strtok(NULL,":");

    		if (TozcCheckStringField(passwordCrypt,TOZC_PASSWORD_VALID_CHARS,TOZC_MIN_PASSWORD_SIZE,TOZC_MAX_PASSWORD_SIZE)){ 
      			profile = (char *)malloc(sizeof(char)*(strlen(passwordCrypt)+1));     
      			strcpy(profile,passwordCrypt);
    		}
    	
		else
      
			profile = strtok(NULL,":");

    		userReadname = strtok(NULL,":");
    		
		erro = TozcCheckStringField (userReadname, TOZC_USERNAME_VALID_CHARS, TOZC_MIN_USERNAME_SIZE, TOZC_MAX_USERNAME_SIZE);
    		
		if (erro){
      			fclose(readDataFile);
      			fclose(writeDataFile);
      			remove("temp3");
      			return erro;
    		}

    		email = strtok(NULL,"\n");
    		erro = TozcCheckStringField (email, TOZC_EMAIL_VALID_CHARS, TOZC_MIN_EMAIL_SIZE, TOZC_MAX_EMAIL_SIZE);
    		if (erro){
      			fclose(readDataFile);
      			fclose(writeDataFile);
      			remove("temp3");
      			return erro;
    		}

    		if (!strcmp(user->nickname,nickname)){
      			foundUser = 1;
      			user->uid = (tozcUserIdentifierType) strtoull (stringId,&verification,10);
      			
			if (verification[0]!='\0'){
        			fclose(readDataFile);
        			fclose(writeDataFile);
        			remove("temp3");
        			return acceptInviteInvalidUser;
      			}

      			strcpy(user->name,userReadname);
      			strcpy(user->email,email);
      
			if(!strcmp(profile,"0"))
        			user->profileType = profileAdmin;
      			else 
				if(!strcmp(profile,"1"))
        				user->profileType = profileUser;
      				else 
      				{
        				fclose(readDataFile);
        				fclose(writeDataFile);
        				remove("temp3");
        				return acceptInviteProfileNotFound;
      				}

      			snprintf(abeyFileName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USERS_ABEYANCES_DATA_FILENAME);

      			readAbeyFile = fopen(abeyFileName,"r");
      
			if (readAbeyFile == NULL){
        			fclose(readDataFile);
        			fclose(writeDataFile);
        			remove("temp3");
        			return acceptInviteOpeningReadFile;
      			}
 
      			writeAbeyFile = fopen("temp4","w");
      		
			if (writeAbeyFile == NULL){
        			fclose(readDataFile);
        			fclose(writeDataFile);
        			fclose(readAbeyFile);
        			remove("temp3");
        			return acceptInviteOpeningWriteFile;
      			}

      			readLine =1 ;
      
			while(readLine>0){
        			readLine = fread(&readTimeExp,sizeof(time_t),1,readAbeyFile);
        	
				if (readLine<=0)
          		break;
        
				readLine = fread(&id,sizeof(tozcUserIdentifierType),1,readAbeyFile);
        		
				if (readLine<=0){
          				fclose(readDataFile);
          				fclose(writeDataFile);
          				fclose(readAbeyFile);
          				fclose(writeAbeyFile);
          				remove("temp3");
          				remove("temp4");
          				return acceptInviteInvalidAbeyancesFile;
       	 			}

        			readLine = fread(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,readAbeyFile);
        
				if (readLine<=0){
          				fclose(readDataFile);
          				fclose(writeDataFile);
          				fclose(readAbeyFile);
          				fclose(writeAbeyFile);
          				remove("temp3");
          				remove("temp4");
          				return acceptInviteInvalidAbeyancesFile;
        			}
        
				if (id == user->uid){
          				erro = TozcCheckPassword(passwordTemp, passwordCrypt);
          				if (erro){
            					fclose(readDataFile);
            					fclose(writeDataFile);
            					fclose(readAbeyFile);
            					fclose(writeAbeyFile);
            					remove("temp3");
            					remove("temp4");
            					return erro;
          				}
    
          				timeExp = time(NULL) + TOZC_INVITATION_EXPIRATION_TIME;
          
					if(timeExp>readTimeExp){
            					fclose(readDataFile);
            					fclose(writeDataFile);
            					fclose(readAbeyFile);
           		 			fclose(writeAbeyFile);
            					remove("temp3");
            					remove("temp4");
            					return acceptInviteTimeExpired;
          				}

            
          				userReadW = 1;

          				erroMail = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME, TOZC_SMTP_SERVER_PORT,
					TOZC_EMAIL_FROM,user->email,NULL,NULL,

			   "Bem vindx a comunidade 380Caronas",
					
					TOZC_EMAIL_BODY, NULL);

		  			fprintf(writeDataFile,"%llu:%s:%s:%i:%s:%s\n",user->uid,user->nickname,user->password,user->profileType,user->name,user->email);
        			}
         
        			else
        		
				{
          				fwrite(&readTimeExp,sizeof(time_t),1,writeAbeyFile);
          				fwrite(&id,sizeof(tozcUserIdentifierType),1,writeAbeyFile);
          				fwrite(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,writeAbeyFile);
        			}
      			}
      		
			fclose(readAbeyFile);
      			fclose(writeAbeyFile);
      			rename("temp4",abeyFileName);
 
      			if(!userReadW){
        			fclose(readDataFile);
        			fclose(writeDataFile);
        			remove("temp3");
        			return acceptInviteUserNotFoundInAbeyances;  
      			}	 
    		}
  
    	else
    		{
      			fprintf(writeDataFile,"%s",buffer);
    		}

  	}

  	fclose(readDataFile);
  	fclose(writeDataFile);
  	rename("temp3",dataFileName);   
  
  	if (foundUser != 0)
    		return acceptInviteUserNotFound;


return ok;
}
/* $RCSFile$ */
