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
#include "tozcGetUnlockingRequests.h"


tozcErrorType 
TozcGetUnlockingRequests(char *userNickname,  tozcNicknameListType **first)
{
/********************Variaveis*********************/

FILE *fileReadData;
FILE *fileUnlockRead;
tozcErrorType erro;
char fileDataName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char fileUnlockName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char *nickname;
char *idStr;  
char buffer[TOZC_MAX_BUFFER_SIZE+2];
char buffer2[TOZC_MAX_BUFFER_SIZE+2];
char *verification;
int linesRead;
unsigned foundUser;
tozcNicknameListType *unlockedUser; 
tozcUserIdentifierType idb;
tozcUserIdentifierType idText;
time_t expirationTime;
 
/*Codigo*/
  	if (userNickname == NULL)
    		return getUnlockingRequestsNoNickname;
    
  	snprintf(fileUnlockName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_UNLOCKING_USERS_DATA_FILENAME);
  
	fileUnlockRead = fopen(fileUnlockName,"r");
  	linesRead =1 ;
  	while(linesRead>0){

    		linesRead = fread(&expirationTime,sizeof(time_t),1,fileUnlockRead);
    		if (linesRead<=0)
      		break;
    
		linesRead = fread(&idb,sizeof(tozcUserIdentifierType),1,fileUnlockRead);
      	
		if (linesRead<=0){
 			fclose(fileUnlockRead);
        		return getUnlockingRequestsInvalidUnlockFile;
      		}

      		if (expirationTime>time(NULL)){

        		snprintf(fileDataName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);

			fileReadData = fopen(fileDataName,"r");
			if (fileReadData == NULL){
		 	 	fclose(fileUnlockRead);
		  		return getUnlockingRequestsOpenReadFile;
			}

		foundUser = 0;
		while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, fileReadData)) != NULL){ 
          		if(buffer[strlen(buffer)-1]!='\n'){
            			fclose(fileUnlockRead);
            			fclose(fileReadData);
            			return getUnlockingRequestsInvalidUserFile;
          		}
  
          	strcpy(buffer2,buffer);

          	idStr = strtok(buffer2, ":");
          	nickname = strtok(NULL,":");

          	foundUser = 1;
          
		idText = (tozcUserIdentifierType)strtoull(idStr,&verification,10);
          
		if (verification[0]!='\0'){
            		fclose(fileUnlockRead);
            		fclose(fileReadData);
            		return getUnlockingRequestsInvalidUserFile;
          	}
          
		if(idText==idb){
            		unlockedUser = (tozcNicknameListType *)malloc(sizeof(tozcNicknameListType));
           		strcpy(unlockedUser->nickname,nickname);
            		unlockedUser->next = NULL;
            		
			erro = TozcCreateSimpleList(first,unlockedUser);
            
			if(erro){
              			fclose(fileUnlockRead);
              			fclose(fileReadData);
              			return getUnlockingRequestsInvalidUserFile;
            		}
          	}
        }
	fclose(fileReadData);          
      	}
  	}

  	if (!foundUser)
    		return getUnlockingRequestsUserNotFound;
       
 
return ok;
} 
/*$RCSFile$*/
