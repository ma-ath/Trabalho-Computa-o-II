/* 
  * Universidade Federal do Rio de Janeiro 
  * Escola Politecnica 
  * Departamento de Eletronica e de Computacao 
  * Prof. Marcelo Luiz Drumond Lanza 
  * EEL 270 - Computacao II - Turma 2015/2
  * Autor: Matheus Silva de Lima
  *
  * $Author$ 
  * $Date$ 
  * $Log$
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

#include "tozcLockUserAccount.h"


tozcErrorType 
TozcLockUserAccount(char *adminName, char *userNickname)
{
/*Variaveis*/
FILE *fileDataRead;
FILE *fileDataWrite;
FILE *fileLockRead;
FILE *fileLockWrite;
tozcErrorType erro;
char fileDataName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileLockName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char *passwordCrypt;
char *nickname;
char *idStr;
char *email;
char *username;
char *profile;  
char buffer[TOZC_MAX_BUFFER_SIZE+2];
char bufferMod[TOZC_MAX_BUFFER_SIZE+2];
char *verification;
unsigned erroMail;
int linesRead;
unsigned foundUser,userW;
tozcUserDataType *userLocked; 
tozcUserIdentifierType userId;
tozcUserIdentifierType userIdRead;
tozcUserIdentifierType id;
time_t timeB;
time_t timeBR;
char emailBodyMessage[10000 +1];
/*Codigo*/

  	userLocked = (tozcUserDataType *)malloc(sizeof(tozcUserDataType));
 
  	if (userNickname == NULL)
    		snprintf(userLocked->nickname,TOZC_MAX_NICKNAME_SIZE+1,"%s",adminName);
  	else
    		snprintf(userLocked->nickname,TOZC_MAX_NICKNAME_SIZE+1,"%s",userNickname);
    
 
  	snprintf(fileDataName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);
  
  	fileDataRead = fopen(fileDataName,"r");
  	if (fileDataRead == NULL)
    		return lockUserAccountOpenReadFile;

  	foundUser =0 ;

  	while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, fileDataRead)) != NULL){	 
    		if(buffer[strlen(buffer)-1]!='\n'){
      			fclose(fileDataRead);
      			return lockUserAccountInvalidUserFile;
    		}
  
    		strcpy(bufferMod,buffer);
    		idStr = strtok(bufferMod, ":");

    		nickname = strtok(NULL,":");
 
    		if (!strcmp(adminName,nickname)){
      			foundUser =1;
      			userId = (tozcUserIdentifierType)strtoull(idStr,&verification,10);
      			if (verification[0]!='\0'){
        			fclose(fileDataRead);
        			return lockUserAccountInvalidUserFile;
      			}
      		break;
    		}
  	}
  
	fclose(fileDataRead);
  	if (!foundUser)
    		return lockUserAccountUserNotFound;
    
	fileDataRead = fopen(fileDataName,"r");
  
	if (fileDataRead == NULL)
    		return lockUserAccountOpenReadFile;
 
  	fileDataWrite = fopen("temp5","w");
  	if (fileDataWrite == NULL){
    		fclose(fileDataRead);
    		return lockUserAccountOpenWriteFile;
  	}

  	foundUser =0 ;

  	while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, fileDataRead)) != NULL){ 
    		if(buffer[strlen(buffer)-1]!='\n'){
     	 		fclose(fileDataRead);
      			fclose(fileDataWrite);
      			remove("temp5");
      			return lockUserAccountInvalidUserFile;
    		}
  
    		strcpy(bufferMod,buffer);
    		idStr = strtok(bufferMod, ":");

    		nickname = strtok(NULL,":");
    		passwordCrypt = strtok(NULL,":");
    		
		if (TozcCheckStringField(passwordCrypt,TOZC_PASSWORD_VALID_CHARS,TOZC_MIN_PASSWORD_SIZE,TOZC_MAX_PASSWORD_SIZE)){ 
      			profile = (char *)malloc(sizeof(char)*(strlen(passwordCrypt)+1));     
      			strcpy(profile,passwordCrypt);
    		}
    
		else
      	
		profile = strtok(NULL,":");

    		username = strtok(NULL,":");
    
		erro = TozcCheckStringField (username, TOZC_USERNAME_VALID_CHARS, TOZC_MIN_USERNAME_SIZE, TOZC_MAX_USERNAME_SIZE);
    		if (erro){
      			fclose(fileDataRead);
      			fclose(fileDataWrite);
      			remove("temp5");
      			return erro;
    		}

    		email = strtok(NULL,"\n");
    
		erro = TozcCheckStringField (email, TOZC_EMAIL_VALID_CHARS, TOZC_MIN_EMAIL_SIZE, TOZC_MAX_EMAIL_SIZE);
    		if (erro){
      			fclose(fileDataRead);
      			fclose(fileDataWrite);
     	 		remove("temp5");
      			return erro;
    		}

    		if (!strcmp(userLocked->nickname,nickname)){
      			foundUser = 1;
      			userLocked->uid = (tozcUserIdentifierType)strtoull(idStr,&verification,10);
      			if (verification[0]!='\0'){
        			fclose(fileDataRead);
        			fclose(fileDataWrite);
        			remove("temp5");
        			return lockUserAccountInvalidUserFile;
      			}

      		strcpy(userLocked->name,username);
      		strcpy(userLocked->email,email);
      
		if(!strcmp(profile,"0"))
        		userLocked->profileType = profileAdmin;
      
		else 
			
			if(!strcmp(profile,"1"))
        			userLocked->profileType = profileAdmin;
      			
			else {
        			fclose(fileDataRead);
        			fclose(fileDataWrite);
        			remove("temp5");
        			return lockUserAccountProfileNotFound; 
      			}

      		userW = 0;

      		snprintf(fileLockName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_LOCKED_USERS_DATA_FILENAME);
      
		fileLockRead = fopen(fileLockName,"r");
     	 	if (fileLockRead==NULL){
        		fileLockWrite = fopen(fileLockName,"w");
        		if (fileLockWrite==NULL)
          			return lockUserAccountOpenWriteFile;
        		
			snprintf(emailBodyMessage,5000,"Sua conta foi bloqueada.\n\n 380Caronas");

        erroMail = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME, TOZC_SMTP_SERVER_PORT, TOZC_EMAIL_FROM,userLocked->email,NULL,NULL,
                   "Conta Bloqueada", emailBodyMessage, NULL);

        		timeB = time(NULL);
        
			fwrite(&(userLocked->uid),sizeof(tozcUserIdentifierType),1,fileLockWrite);
        		fwrite(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,fileLockWrite);
        		fwrite(&timeB,sizeof(time_t),1,fileLockWrite);
        		fwrite(&userId,sizeof(tozcUserIdentifierType),1,fileLockWrite);
        		fclose(fileLockWrite); 
      		}
      
		else{
        
			fileLockWrite = fopen("temp2","w");
       			 if (fileLockWrite == NULL){
          			fclose(fileDataRead);
          			fclose(fileDataWrite);
          			remove("temp5");
          			fclose(fileLockRead);
          			return lockUserAccountOpenWriteFile;
        		}
        
			linesRead =1 ;
        
			while(linesRead>0){
          			linesRead = fread(&id,sizeof(tozcUserIdentifierType),1,fileLockRead);
          			if (linesRead<=0)
            				break;
          
				linesRead = fread(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,fileLockRead);
          			if (linesRead<=0){
            				fclose(fileDataRead);
            				fclose(fileDataWrite);
            				remove("temp5");
            				fclose(fileLockRead);
            				fclose(fileLockWrite);
            				remove("temp2");
            				return lockUserAccountOpenWriteFile;
          			}

          			linesRead = fread(&timeBR,sizeof(time_t),1,fileLockRead);
          			if (linesRead<=0){
            				fclose(fileDataRead);
            				fclose(fileDataWrite);
            				remove("temp5");
            				fclose(fileLockRead);
            				fclose(fileLockWrite);
            				remove("temp2");
	    				return lockUserAccountInvalidLockFile;
          			}

          			linesRead = fread(&userIdRead,sizeof(tozcUserIdentifierType),1,fileLockRead);
         
		 		if (linesRead<=0){
            				fclose(fileDataRead);
            				fclose(fileDataWrite);
            				remove("temp5");
            				fclose(fileLockRead);
            				fclose(fileLockWrite);
            				remove("temp2");
            				return lockUserAccountInvalidLockFile;
          			}

          			if(userLocked->uid<id){

            				userW = 1;
            				snprintf(emailBodyMessage,5000,"Sua conta foi bloqueada");
					
					erroMail = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME, 
					TOZC_SMTP_SERVER_PORT, TOZC_EMAIL_FROM,userLocked->email,NULL,NULL,"Conta Bloqueada",
					TOZC_EMAIL_BODY, NULL);
            
					timeB = time(NULL);
            				fwrite(&(userLocked->uid),sizeof(tozcUserIdentifierType),1,fileLockWrite);
            				fwrite(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,fileLockWrite);
            				fwrite(&timeB,sizeof(time_t),1,fileLockWrite);
            				fwrite(&userId,sizeof(tozcUserIdentifierType),1,fileLockWrite);
          			} 
        
          			fwrite(&id,sizeof(tozcUserIdentifierType),1,fileLockWrite);
          			fwrite(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,fileLockWrite);
          			fwrite(&timeBR,sizeof(time_t),1,fileLockWrite);
          			fwrite(&userIdRead,sizeof(tozcUserIdentifierType),1,fileLockWrite);
        		}
        
			if(!userW){
          
				snprintf(emailBodyMessage,5000,"Your account was blocked.");
		
				erroMail = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME,
				TOZC_SMTP_SERVER_PORT, TOZC_EMAIL_FROM,userLocked->email,NULL,NULL,"Conta Bloqueada",
				TOZC_EMAIL_BODY, NULL);
          
				timeB = time(NULL);
        	  		fwrite(&(userLocked->uid),sizeof(tozcUserIdentifierType),1,fileLockWrite);
	        	  	fwrite(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,fileLockWrite);
        	  		fwrite(&timeB,sizeof(time_t),1,fileLockWrite);
	          		fwrite(&userId,sizeof(tozcUserIdentifierType),1,fileLockWrite);
        		}		 
        	
			fclose(fileLockRead);
        		fclose(fileLockWrite);
        		rename("temp2",fileLockName);
      		}

      		fprintf(fileDataWrite,"%llu:%s::%i:%s:%s\n",userLocked->uid,userLocked->nickname,userLocked->profileType,userLocked->name,userLocked->email);
    	}

	else

     		fprintf(fileDataWrite,"%s",buffer); 
  	}

  	fclose(fileDataRead);
  	fclose(fileDataWrite);
  	rename("temp5",fileDataName);
  
  	if (!foundUser)
    		return lockUserAccountUserNotFound;
       
 
return ok;
} 
/*$RCSFile$*/
