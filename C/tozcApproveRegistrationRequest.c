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
#include "tozcApproveRegistrationRequest.h"

#include"Libraries/sendmail.h"




tozcErrorType
TozcApproveRegistrationRequest(char *user,char *nickname)
{
FILE *fileUser,*fileRequest;
FILE *fileCopyUser,*fileCopyRequest;
tozcUserDataType *list,*aux,request,member;
size_t index;
time_t timeExpiration;
tozcUserIdentifierType uid,userId;
char uidStr[TOZC_USER_IDENTIFIER_MAX_LENGTH+1];
char password[TOZC_MAX_PASSWORD_SIZE+1];
char requestPassword[TOZC_MAX_PASSWORD_SIZE_CRYPT+1];
char buffer[TOZC_MAX_BUFFER_SIZE+2];
char emailBody[TOZC_EMAIL_BODY_SIZE+1];
char *validation;
char fileUserName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileRequestName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileCopyUserName[TOZC_MAX_FILENAME_SIZE+1];
char fileCopyRequestName[TOZC_MAX_FILENAME_SIZE+1];
int check;
tozcErrorType erro;
 

  	erro=TozcCheckNickname(nickname,TOZC_NICKNAME_VALID_CHARS,TOZC_MIN_NICKNAME_SIZE,TOZC_MAX_NICKNAME_SIZE);
  	if(erro)
    		return erro;
	
	snprintf(fileUserName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);
        snprintf(fileRequestName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_REQUESTING_USERS_DATA_FILENAME);

	printf("%s\n",fileRequestName);
	

  	do{
    		TozcCreateRandomString(TOZC_SALT_VALID_CHARS,20,fileCopyUserName);
    		fileCopyUser=fopen(fileCopyUserName,"w"); 
  	}
  


	while(!fileCopyUser);
  		fclose(fileCopyUser);
  	do{
    		TozcCreateRandomString(TOZC_SALT_VALID_CHARS,20,fileCopyRequestName);
    		fileCopyRequest=fopen(fileCopyRequestName,"w"); 
  	}
  


	while(!fileCopyRequest);
  		fclose(fileCopyRequest);

  	erro=TozcCopy(fileUserName,fileCopyUserName);
  
	if (erro){
    		remove(fileCopyUserName);
    		return erro; 
  	}
  



	erro=TozcCopy(fileRequestName,fileCopyRequestName);
 

	if (erro){
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return erro; 
  	}
  


  	erro=TozcGetUsers(&list);
  
	if(erro){
    		TozcFreeMemory(&list);
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return erro;
  	}


  	if(!list){
    		TozcFreeMemory(&list);
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return approveRegistrationRequesquestInvalidFile;
  	}
  
  	aux=list;
  

	while(aux){
    		if(!strcmp(aux->nickname,user)){
      
			member.profileType=aux->profileType;
      			member.uid=aux->uid;
    		}

    		if(!strcmp(aux->nickname,nickname)){
      			request.uid=aux->uid;
      			strcpy(request.name,aux->name);
      			request.profileType=aux->profileType;
      			strcpy(request.email,aux->email);
    		}
    		aux=aux->next;
  	}
  
	TozcFreeMemory(&list);

  	fileRequest=fopen(fileRequestName,"w"); 
  	if (!fileRequest){
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return approveRegistrationRequesquestOpenFileError;
  	}
  
	fileCopyRequest=fopen(fileRequestName,"r"); 
  
	if (!fileRequest){
    		fclose(fileRequest);
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return approveRegistrationRequesquestOpenFileError;
  	}

  	check = 0;


  	while(fread(&timeExpiration,1,sizeof(time_t),fileCopyRequest)){

    		fread(&userId,1,sizeof(tozcUserIdentifierType),fileCopyRequest);
    		fread(&uid,1,sizeof(tozcUserIdentifierType),fileCopyRequest);
    		fread(password,1,TOZC_MAX_PASSWORD_SIZE_CRYPT,fileCopyRequest);
    		password[TOZC_MAX_PASSWORD_SIZE_CRYPT]='\0';
    
		if(ferror(fileCopyRequest)){
      			fclose(fileRequest);
      			fclose(fileCopyRequest);
      			TozcCopy(fileCopyRequestName,fileRequestName);
      			remove(fileCopyUserName);
      			remove(fileCopyRequestName);
      			return approveRegistrationRequesquestReadFile;
    		}

    		if(request.uid==uid){
      			if(timeExpiration>time(NULL)){
        
				if(member.profileType==profileAdmin)
          				check=1;
        			if(member.profileType!=profileAdmin)
          				if(member.uid==userId)
            					check=1;
      			}
      
			if(check==0){
        			strcpy(requestPassword,password);
	        		fwrite(&timeExpiration,1,sizeof(time_t),fileRequest);
        			fwrite(&userId,1,sizeof(tozcUserIdentifierType),fileRequest);
        			fwrite(&uid,1,sizeof(tozcUserIdentifierType),fileRequest);
        			fwrite(password,1,TOZC_MAX_PASSWORD_SIZE_CRYPT,fileRequest);
      			}
    		}
    
		else
    
		{
      			fwrite(&timeExpiration,1,sizeof(time_t),fileRequest);
      			fwrite(&userId,1,sizeof(tozcUserIdentifierType),fileRequest);
      			fwrite(&uid,1,sizeof(tozcUserIdentifierType),fileRequest);
      			fwrite(password,1,TOZC_MAX_PASSWORD_SIZE_CRYPT,fileRequest);
    		}
  	}

  	fclose(fileRequest);
  	fclose(fileCopyRequest);

  	if(check==0){
    		TozcCopy(fileCopyRequestName,fileRequestName);
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return approveRegistrationRequesquestUserNotFound;
  	}

  	fileUser=fopen(fileUserName,"w"); 

  	if (!fileUser){
    		TozcCopy(fileCopyRequestName,fileRequestName);
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return approveRegistrationRequesquestOpenFileError;
  	}
  
	fileCopyUser=fopen(fileCopyUserName,"r"); 
  
	if (!fileCopyUser){
    		fclose(fileUser);
    		TozcCopy(fileCopyRequestName,fileRequestName);
    		remove(fileCopyUserName);
    		remove(fileCopyRequestName);
    		return approveRegistrationRequesquestOpenFileError;
  	}
  
	while(fgets(buffer,TOZC_MAX_BUFFER_SIZE+2,fileCopyUser)){
    		if(ferror(fileCopyUser)){
      			fclose(fileUser);
      			fclose(fileCopyUser);
      			TozcCopy(fileCopyRequestName,fileRequestName);
      			TozcCopy(fileCopyUserName,fileUserName);
      			remove(fileCopyUserName);
     	 		remove(fileCopyRequestName);
      			return approveRegistrationRequesquestReadFile;
    		}

    		for(index=0;buffer[index]!=':'&&buffer[index]!='\0';index++)
      			uidStr[index]=buffer[index];
    		
		uidStr[index]='\0';
    		
		uid=(unsigned short) strtoull(uidStr,&validation,10); 
    		if(*validation!='\0'){
      			fclose(fileUser);
      			fclose(fileCopyUser);
      			TozcCopy(fileCopyRequestName,fileRequestName);
      			TozcCopy(fileCopyUserName,fileUserName);
      			remove(fileCopyUserName);
      			remove(fileCopyRequestName);
      			return approveRegistrationRequesquesInvalidFile;
    		}
    
		if(uid==request.uid) {
      			fprintf(fileUser,"%llu:%s:%s:%d:%s:%s",
            		uid,nickname,requestPassword,request.profileType,request.name,request.email); 
      
			if(buffer[strlen(buffer)-1]=='\n')
        			fprintf(fileUser,"\n");
    		}
    
		else
      
		fprintf(fileUser,"%s",buffer);
  	}
  
	fclose(fileUser);
  	fclose(fileCopyUser);


  	remove(fileCopyUserName);
  	remove(fileCopyRequestName);


  	snprintf(emailBody,TOZC_EMAIL_BODY_SIZE+1,
  "Seu pedido para participar foi aprovado! =D\n\n%s\nID:%llu\nNickname(Apelido):%s\nProfile(Perfil):User(Usuario)",
           request.name,request.uid,nickname);

	sendMail(TOZC_SMTP_CLIENT_DOMAIN,TOZC_SMTP_SERVER_FULL_HOSTNAME,TOZC_SMTP_SERVER_PORT,TOZC_MAIN_ADMIN_EMAIL,request.email,NULL,NULL,"Welcome to Vem Comigo",emailBody,NULL);
  return ok;
}


/*$RCSfile$*/	
