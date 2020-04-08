/* Universidade Federal do Rio de Janeiro
 * Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma2015/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Matheus Silva de Lima
 *
 * $Author$
 * $Date$
 * $Log$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tozcFunctions.h"
#include "tozcTypes.h"
#include "tozcError.h"
#include "tozcConfig.h"
#include "tozcConst.h"
#include "tozcGetUsers.h"
#include "tozcConfirmEmailChange.h"


tozcErrorType
TozcConfirmEmailChange(char *user,char *userKey)
{
FILE *usersFile;
FILE *fileEmail;
FILE *usersCopy;
FILE *emailCpy;
tozcUserDataType *aux,*list;
size_t index,argumentIndex;
time_t timeExp;
tozcUserIdentifierType uid,userId;
tozcProfileType profile;
char uidStr[TOZC_USER_IDENTIFIER_MAX_LENGTH+1];
char passwordCrypt[TOZC_MAX_PASSWORD_SIZE_CRYPT+1];
char userName[TOZC_MAX_USERNAME_SIZE+1];
char email[TOZC_EMAIL_BODY_SIZE+1];
char userEmail[TOZC_EMAIL_BODY_SIZE+1];
char buffer[TOZC_MAX_BUFFER_SIZE+2];
char *validation;
char key[TOZC_MAX_KEY_SIZE+1];
char fileUsersName[TOZC_MAX_FILENAME_SIZE+1];
char fileEmailName[TOZC_MAX_FILENAME_SIZE+1];
char usersCpyName[TOZC_MAX_FILENAME_SIZE+1];
char emailCpyName[TOZC_MAX_FILENAME_SIZE+1];
int checked,found;
tozcErrorType error;


  	error=TozcCheckStringField(userKey,TOZC_SALT_VALID_CHARS,TOZC_MAX_KEY_SIZE,TOZC_MAX_KEY_SIZE);
  	if(error)
    		return confirmEmailChangeInvalidKey;


  	snprintf(fileUsersName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME); 
  	snprintf(fileEmailName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_EMAIL_ABEYANCES_DATA_FILENAME); 

  	do{
    		TozcCreateRandomString(TOZC_SALT_VALID_CHARS,20,usersCpyName);
    		usersCopy=fopen(usersCpyName,"w"); 
  	}

  	while(!usersCopy);
  		fclose(usersCopy);
  	do{
    		TozcCreateRandomString(TOZC_SALT_VALID_CHARS,20,emailCpyName);
    		emailCpy=fopen(emailCpyName,"w"); 
  	}

  	while(!emailCpy);
  		fclose(emailCpy);

  	error=TozcCopy(fileUsersName,usersCpyName);
  	if (error){
   		remove(usersCpyName);
    		return error; 
  	}

  	error=TozcCopy(fileEmailName,emailCpyName);
  	if (error){
    		remove(usersCpyName);
    		remove(emailCpyName);
    		return error; 
  	}
  
  	error=TozcGetUsers(&list);
  
	if(error){
    		TozcFreeMemory(&list);
	    	remove(usersCpyName);
    		remove(emailCpyName);
    		return error;
  	}
  
	if(!list){
    		TozcFreeMemory(&list);
    		remove(usersCpyName);
    		remove(emailCpyName);
    		return confirmEmailChangeInvalidFile;
  	}
  
  	found=1;

  	aux=list;
  	while(aux){
    		if(!strcmp(aux->nickname,user)){
      			uid=aux->uid;
	      		strcpy(userName,aux->name);
      			profile=aux->profileType;
      			found=0;
    		}
    	aux=aux->next;
  	}
  
	TozcFreeMemory(&list);
  
  	if(found==1){
	    	remove(usersCpyName);
    		remove(emailCpyName);
    		return confirmEmailChangeUserNotFound;
  	}
    
  	checked=1;

  	fileEmail=fopen(fileEmailName,"w"); 
  
	if (!fileEmail){
    		remove(usersCpyName);
    		remove(emailCpyName);
    		return confirmEmailChangeOpenFile;
 	 }
  
	emailCpy=fopen(fileEmailName,"r"); 

  	if (!emailCpy){
    		fclose(fileEmail);
    		remove(usersCpyName);
    		remove(emailCpyName);
    		return confirmEmailChangeOpenFile;
  	}

  	while(fread(&timeExp,1,sizeof(time_t),emailCpy)){
    		fread(&userId,1,sizeof(tozcUserIdentifierType),emailCpy);
    		fread(email,1,TOZC_EMAIL_BODY_SIZE,emailCpy);
      		email[TOZC_EMAIL_BODY_SIZE]='\0';
    		fread(key,1,TOZC_MAX_KEY_SIZE,emailCpy);
      		key[TOZC_MAX_KEY_SIZE]='\0';
    
		if(ferror(emailCpy)){
      			fclose(fileEmail);
      			fclose(emailCpy);
      			TozcCopy(emailCpyName,fileEmailName);
      			remove(usersCpyName);
      			remove(emailCpyName);
      			return confirmEmailChangeReadingFile;
    		}
    
	if(uid==userId){
      		if(timeExp>time(NULL))
        		if(!strcmp(key,userKey))
          			checked=0;
      			strcpy(userEmail,email);
    	}
    
	else
    
		{
      			fwrite(&timeExp,1,sizeof(time_t),fileEmail);
      			fwrite(&userId,1,sizeof(tozcUserIdentifierType),fileEmail);
	      		fwrite(email,1,TOZC_EMAIL_BODY_SIZE,fileEmail);
      			fwrite(key,1,TOZC_MAX_KEY_SIZE,fileEmail);
    		}
  	}
  
	fclose(fileEmail);
  	fclose(emailCpy);
  
  	if(checked==1){
    		TozcCopy(emailCpyName,fileEmailName);
    		remove(usersCpyName);
    		remove(emailCpyName);
    		return confirmEmailChangeAccessDenied;
  	}
  

  	usersFile=fopen(fileUsersName,"w"); 

  	if (!usersFile){
    		TozcCopy(emailCpyName,fileEmailName);
    		remove(usersCpyName);
    		remove(emailCpyName);
    		return confirmEmailChangeOpenFile;
  	}
  
	usersCopy=fopen(usersCpyName,"r"); 
  
	if (!usersCopy){
    		fclose(usersFile);
    		TozcCopy(emailCpyName,fileEmailName);
    		remove(usersCpyName);
    		remove(emailCpyName);
    		return confirmEmailChangeOpenFile;
  	}

  	while(fgets(buffer,TOZC_MAX_BUFFER_SIZE+2,usersCopy)){
    		if(ferror(usersCopy)){
      			fclose(usersFile);
	      		fclose(usersCopy);
      			TozcCopy(emailCpyName,fileEmailName);
      			TozcCopy(usersCpyName,fileUsersName);
	      		remove(usersCpyName);
      			remove(emailCpyName);
      			return confirmEmailChangeReadingFile;
    		}

    		for(index=0;buffer[index]!=':'&&buffer[index]!='\0';index++)
      			uidStr[index]=buffer[index];
    		uidStr[index]='\0';
    
		for(argumentIndex=0;buffer[index+argumentIndex]!=':'&&buffer[index+argumentIndex]!='\0';argumentIndex++);
    			index=argumentIndex+index+1;

    		for(argumentIndex=0;buffer[index+argumentIndex]!=':'&&buffer[index+argumentIndex]!='\0';argumentIndex++)
     	 		passwordCrypt[argumentIndex]=buffer[index+argumentIndex];
   
		 passwordCrypt[argumentIndex]='\0';
    			userId=(unsigned short) strtoull(uidStr,&validation,10);
    		
		if(*validation!='\0'){
      			fclose(usersFile);
      			fclose(usersCopy);
      			TozcCopy(emailCpyName,fileEmailName);
      			TozcCopy(usersCpyName,fileUsersName);
      			remove(usersCpyName);
      			remove(emailCpyName);
      			return confirmEmailChangeInvalidFile;
    		}
    
		if(uid==userId){
      			fprintf(usersFile,"%llu:%s:%s:%d:%s:%s",
          		userId,user,passwordCrypt,profile,userName,userEmail); 
      			if(buffer[strlen(buffer)-1]=='\n')
        			fprintf(usersFile,"\n");
   	 	}
    
		else
      	
		fprintf(usersFile,"%s",buffer);
  	}

fclose(usersFile);
fclose(usersCopy);
remove(usersCpyName);
remove(emailCpyName);
  
return ok;
}

/*$RCSfile$*/	
