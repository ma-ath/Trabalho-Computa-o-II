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
#include "tozcUnlockUserAccount.h"

tozcErrorType 
TozcUnlockUserAccount(char *AdmNickname, char *UserNickname)
{
/*Variaveis*/
FILE *fileDataRead;
FILE *fileDataWrite;
FILE *fileLockRead;
FILE *fileLockWrite;
FILE *fileUnlockRead;
FILE *fileUnlockWrite;
tozcErrorType erro;
char fileDataName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileLockName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileUnlockName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char *passwordCrypt;
char *nickname;
char *idStr;
char *email;
char *username;
char *profile;  
char buffer[TOZC_MAX_BUFFER_SIZE+2];
char buffer2[TOZC_MAX_BUFFER_SIZE+2];
char *verification;
unsigned erroMail;
int linesRead;
unsigned foundUser;
unsigned userW;
tozcUserDataType *userLocked; 
tozcUserIdentifierType idUserRead;
tozcUserIdentifierType id;
time_t timeBR;
time_t timeE;
time_t timeRE;
char emailBodyMessage[10000 +1];
/*codigo*/

	userLocked = (tozcUserDataType *)malloc(sizeof(tozcUserDataType));
 
  	if (UserNickname == NULL)
    		snprintf(userLocked->nickname,TOZC_MAX_NICKNAME_SIZE+1,"%s",AdmNickname);
  	else
    		snprintf(userLocked->nickname,TOZC_MAX_NICKNAME_SIZE+1,"%s",UserNickname);
    
 
  	snprintf(fileDataName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);
  	fileDataRead = fopen(fileDataName,"r");
  	if (fileDataRead == NULL)
    		return unlockUserAccountOpenReadFile; 

  	fileDataWrite = fopen("temp5","w");
  	if (fileDataWrite == NULL){
    		fclose(fileDataRead);
    		return unlockUserAccountOpenReadFile;
  	}

  	foundUser = 0 ;

  	while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, fileDataRead)) != NULL){ 
    		if(buffer[strlen(buffer)-1]!='\n'){
      			fclose(fileDataRead);
      			fclose(fileDataWrite);
      			remove("temp5");
      			return unlockUserAccountInvalidUserFile;
    		}
  
	    	strcpy(buffer2,buffer);
    		idStr = strtok(buffer2, ":");

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
        			return unlockUserAccountInvalidUserFile;
      			}

      			strcpy(userLocked->name,username);
      			strcpy(userLocked->email,email);
      			if(!strcmp(profile,"0"))
        			userLocked->profileType = profileAdmin;
      			else
				if(!strcmp(profile,"1"))
        				userLocked->profileType = profileUser;
      				else{
        				fclose(fileDataRead);
        				fclose(fileDataWrite);
        				remove("temp5");
        				return unlockUserAccountProfileNotFound; 
      				}

      			userW = 0;
      
			snprintf(fileLockName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_LOCKED_USERS_DATA_FILENAME);
      
			fileLockRead = fopen(fileLockName,"r");
      			if (fileLockRead==NULL)
        			return unlockUserAccountOpenWriteFile;
      
      			fileLockWrite = fopen("temp2","w");
      			if (fileLockWrite == NULL){
        			fclose(fileDataRead);
        			fclose(fileDataWrite);
        			remove("temp5");
        			fclose(fileLockRead);
        			return unlockUserAccountOpenWriteFile;
      			}

      			linesRead =1 ;

      			while(linesRead>0){
        			linesRead = fread(&id,sizeof(tozcUserIdentifierType),1,fileLockRead);
        			printf("id: %llu\n",id);
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
          				return unlockUserAccountInvalidLockFile;
        			}

        			linesRead = fread(&timeBR,sizeof(time_t),1,fileLockRead);
        
				if (linesRead<=0){
          				fclose(fileDataRead);
          				fclose(fileDataWrite);
          				remove("temp5");
          				fclose(fileLockRead);
          				fclose(fileLockWrite);
          				remove("temp2");
          				return unlockUserAccountInvalidLockFile;
        			}

        			linesRead = fread(&idUserRead,sizeof(tozcUserIdentifierType),1,fileLockRead);
        			printf("id (cliente):   %llu\n",id);
        			if (linesRead<=0){
          				fclose(fileDataRead);
          				fclose(fileDataWrite);
          				remove("temp5");
          				fclose(fileLockRead);
          				fclose(fileLockWrite);
          				remove("temp2");
          				return unlockUserAccountInvalidLockFile;
        			}

        			if(userLocked->uid!=id){
          				fwrite(&id,sizeof(tozcUserIdentifierType),1,fileLockWrite);
          				fwrite(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,fileLockWrite);
          				fwrite(&timeBR,sizeof(time_t),1,fileLockWrite);
          				fwrite(&idUserRead,sizeof(tozcUserIdentifierType),1,fileLockWrite);
        			}
       
	 			else{/*UNLOCKED USERS FILE*/

          				snprintf(fileUnlockName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s",
							TOZC_DATA_DIRECTORY,TOZC_UNLOCKING_USERS_DATA_FILENAME);

          				fileUnlockRead = fopen(fileUnlockName,"r");
          				if (fileUnlockRead==NULL){
 
            					fileUnlockWrite = fopen(fileUnlockName,"w");
            					if (fileUnlockWrite==NULL){
              						fclose(fileLockRead);
              						fclose(fileLockWrite);
              						remove("temp2");
              						fclose(fileDataRead);
              						fclose(fileDataWrite);
              						remove("temp5");
              						return unlockUserAccountOpenWriteFile;
            					}
            					timeE = time(NULL)+(24*60*60);
            					fwrite(&timeE,sizeof(time_t),1,fileUnlockWrite);
            					fwrite(&(userLocked->uid),sizeof(tozcUserIdentifierType),1,fileUnlockWrite);
            					fclose(fileUnlockWrite); 
          				}
          
					else{
            
						fileUnlockWrite = fopen("temp6","w");
            					if (fileUnlockWrite == NULL){
              						fclose(fileLockRead);
              						fclose(fileLockWrite);
              						remove("temp2");
              						fclose(fileDataRead);
              						fclose(fileDataWrite);
              						remove("temp5");
              						fclose(fileUnlockRead);
              						return unlockUserAccountOpenWriteFile;
            					}
            					linesRead =1 ;
            					while(linesRead>0){
              						linesRead = fread(&timeRE,sizeof(time_t),1,fileUnlockRead);
              						if (linesRead<=0)
                						break;
              						linesRead = fread(&id,sizeof(tozcUserIdentifierType),1,fileUnlockRead);
              						if (linesRead<=0){
                
								fclose(fileLockRead);
        	        					fclose(fileLockWrite);
                						remove("temp2");
                						fclose(fileDataRead);
                						fclose(fileDataWrite);
                						remove("temp5");
                						fclose(fileUnlockRead);
                						fclose(fileUnlockWrite);
                						remove("temp6");
                						return unlockUserAccountInvalidUnlockFile;
              						}
              						fwrite(&timeRE,sizeof(time_t),1,fileUnlockWrite);
              						fwrite(&id,sizeof(tozcUserIdentifierType),1,fileUnlockWrite);
            					}

            					timeE = time(NULL)+(24*60*60);
            					fwrite(&timeE,sizeof(time_t),1,fileUnlockWrite);
            					fwrite(&(userLocked->uid),sizeof(tozcUserIdentifierType),1,fileUnlockWrite);
            					fclose(fileUnlockRead); 
            					fclose(fileUnlockWrite); 
            					rename("temp6",fileUnlockName);
          				}
/*-------------*/

        				fprintf(fileDataWrite,"%llu:%s:%s:%i:%s:%s\n",
					userLocked->uid,userLocked->nickname,passwordCrypt,userLocked->profileType,userLocked->name,userLocked->email);
          
					snprintf(emailBodyMessage,5000,"Your account was requested to be unlocked.");
          
					erroMail = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME, TOZC_SMTP_SERVER_PORT,
					TOZC_EMAIL_FROM,userLocked->email,NULL,NULL,"Conta Possivelmente Desbloqueada", TOZC_EMAIL_BODY, NULL);

       				}  
 			}
			fclose(fileLockRead);
			fclose(fileLockWrite);
			rename("temp2",fileLockName);

    		}
    		
		else
     
			fprintf(fileDataWrite,"%s",buffer); 
  	}

  	fclose(fileDataRead);
  	fclose(fileDataWrite);
	rename("temp5",fileDataName);
  
  	if (!foundUser)
    		return unlockUserAccountUserNotFound;
       
 
return ok;
} 
/*$RCSFile$*/
