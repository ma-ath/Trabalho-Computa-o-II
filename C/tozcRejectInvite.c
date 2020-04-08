/* Universidade Federal do Rio de Janeiro
 * Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma2015/2
 * Prof. Marcelo Luiz Drumond Lanza
 * Autor: Matheus SIlva de Lima
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
#include "tozcRejectInvite.h"

#include "Libraries/sendmail.h"



tozcErrorType
TozcRejectInvite (char * oldPassword,char* user)
{
FILE *fileUser,*fileAbeyances;
FILE *userCpy,*abeyancesCpy;
size_t index,argumentIndex;
time_t timeExp;
tozcUserIdentifierType uid, userId;
char uidStr[TOZC_USER_IDENTIFIER_MAX_LENGTH+1];
char password[TOZC_MAX_PASSWORD_SIZE_CRYPT+1];
char buffer[TOZC_MAX_BUFFER_SIZE+2];
char mailBody[TOZC_EMAIL_BODY_SIZE+1];
char nickname[TOZC_MAX_NICKNAME_SIZE+1];
char email[TOZC_MAX_EMAIL_SIZE+1];
char *validation;
char fileUserName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileAbeyancesName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileUsersName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
int found;
tozcErrorType erro;


	erro=TozcCheckNickname(user,TOZC_NICKNAME_VALID_CHARS,TOZC_MIN_NICKNAME_SIZE,TOZC_MAX_NICKNAME_SIZE);
  	if(erro)
    		return erro;

  	snprintf(fileUserName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);
 	
  	fileUser=fopen(fileUserName,"r"); 
  	if(!fileUser)
    		return rejectInviteOpenFile;
  
	do{
    		TozcCreateRandomString(TOZC_SALT_VALID_CHARS,20,fileUsersName);
    		userCpy=fopen(fileUsersName,"w"); 
  	}
  
	while(!userCpy);
  		do{
    			fgets(buffer,TOZC_MAX_BUFFER_SIZE+2,fileUser);
    			if(ferror(fileUser)){
      				fclose(fileUser);
      				fclose(userCpy);
      				remove(fileUsersName);
      				return rejectInviteOpenFile;
    			}
    
			for(index=0;buffer[index]!=':'&&buffer[index]!='\0';index++)
      				uidStr[index]=buffer[index];
    
			uidStr[index]='\0';
    			index++;
    
			for(argumentIndex=0;buffer[index+argumentIndex]!=':'&&buffer[index+argumentIndex]!='\0';argumentIndex++)
      				nickname[argumentIndex]=buffer[index+argumentIndex];
    
			nickname[argumentIndex]='\0';
    			if(!(strcmp(nickname,user))){
      
				index=argumentIndex+index+1;
      				for(argumentIndex=0;buffer[index+argumentIndex]!=':'&&buffer[index+argumentIndex]!='\0';argumentIndex++);
      					index=argumentIndex+index+1;
      				for(argumentIndex=0;buffer[index+argumentIndex]!=':'&&buffer[index+argumentIndex]!='\0';argumentIndex++);
      					index=argumentIndex+index+1;
      				for(argumentIndex=0;buffer[index+argumentIndex]!=':'&&buffer[index+argumentIndex]!='\0';argumentIndex++);
      					index=argumentIndex+index+1;
      				for(argumentIndex=0;buffer[index+argumentIndex]!='\n'&&buffer[index+argumentIndex]!='\0';argumentIndex++);
        				email[argumentIndex]=buffer[index+argumentIndex];
      				email[argumentIndex]='\0';
      
				found=0;
      
				userId=strtoull(uidStr,&validation,10); 
      
				if(*validation!='\0'){
        				fclose(fileUser);
        				fclose(userCpy);
        				remove(fileUsersName);
        				return rejectInviteInvalidFile;
      				}
    			}
    
			else
      
				fprintf(userCpy,"%s",buffer);
  		}
  	while(buffer[strlen(buffer)-1]=='\n');
  		fclose(userCpy);
  		fclose(fileUser);
  
		if(!found){
    			remove(fileUsersName);
    			return rejectInviteUserNotFound;
  		}

  		snprintf(fileAbeyancesName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_ABEYANCES_FILENAME);
  		
 		fileAbeyances=fopen(fileAbeyancesName,"r"); 
  
		if(!fileAbeyances){
    			remove(fileUsersName);
    			return rejectInviteOpenFile;
 		 }
  
		do{
    			TozcCreateRandomString(TOZC_SALT_VALID_CHARS,20,fileAbeyancesName);
    			abeyancesCpy=fopen(fileAbeyancesName,"w"); 
  		}
  
		while(!abeyancesCpy);
  			while(fread(&timeExp,1,sizeof(time_t),fileAbeyances));{
    				fread(&uid,1,sizeof(tozcUserIdentifierType),fileAbeyances);
    				fread(password,1,TOZC_MAX_PASSWORD_SIZE_CRYPT,fileAbeyances);
    				password[TOZC_MAX_PASSWORD_SIZE_CRYPT]='\0';
    				if(ferror(fileAbeyances)){
      					fclose(fileAbeyances);
      					fclose(abeyancesCpy);
      					remove(fileUsersName);
      					remove(fileAbeyancesName);
      					return rejectInviteReadFile;
    				}
    			
				if(uid==userId){
      					found=0;
      
					if(password!=oldPassword){
        					fclose(fileAbeyances);
        					fclose(abeyancesCpy);
      	  					remove(fileUsersName);
        					remove(fileAbeyancesName);
        					return rejectInviteOldPasswordNotCheck;
      					}
      
					if(timeExp<time(NULL)){
        					fclose(fileAbeyances);
        					fclose(abeyancesCpy);
        					remove(fileUsersName);
        					remove(fileAbeyancesName);
        					return rejectInviteInviteExpired;
      					}
    				}
    
				else

    				{
      					fwrite(&uid,1,sizeof(tozcUserIdentifierType),abeyancesCpy);
      					fwrite(&timeExp,1,sizeof(time_t),abeyancesCpy);
      					fwrite(password,1,TOZC_MAX_PASSWORD_SIZE_CRYPT,abeyancesCpy);
    				}
  			}
  
			fclose(fileAbeyances);
  			fclose(abeyancesCpy);
  			if(!found){

    				remove(fileUsersName);
    				remove(fileAbeyancesName);
    				return rejectInviteUserNotFound;
  			}
  
			erro=TozcCopy(fileUsersName,fileUserName);
  			if (erro){
    				remove(fileUsersName);
    				remove(fileAbeyancesName);
    				return erro; 
  			}

  			erro=TozcCopy(fileAbeyancesName,fileAbeyancesName);
  			if (erro){
    				remove(fileUsersName);
    				remove(fileAbeyancesName);
    				return erro; 
  			}

  			remove(fileUsersName);
  			remove(fileAbeyancesName);

  			snprintf(mailBody,TOZC_EMAIL_BODY_SIZE+1,

           "Voce rejeitou com sucesso o convite para participar da comunidade 380Caronas =(\n\n");

  sendMail(TOZC_SMTP_CLIENT_DOMAIN,TOZC_SMTP_SERVER_FULL_HOSTNAME,TOZC_SMTP_SERVER_PORT,TOZC_MAIN_ADMIN_EMAIL,email,NULL,NULL,"Invite Rejected",mailBody,NULL);
  return ok;

}

/*$RCSfile$*/	
