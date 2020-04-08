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
#include "tozcChangeUserProfile.h"

tozcErrorType 
TozcChangeUserProfile(char *nickname, char *userNickname)
{
FILE *fileDataRead;
FILE *fileDataWrite;
FILE *fileLockRead;
FILE *fileRequestRead;
tozcErrorType error;
char dataFilename[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char lockFilename[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char requestFileName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char *passwordCrypt,*newNickname, *uidStr, *email, *username, *profile;  
char buffer[TOZC_MAX_BUFFER_SIZE+2],bufferMod[TOZC_MAX_BUFFER_SIZE+2];
char *verification;
int linesRead;
unsigned foundUser;
tozcUserDataType *newUser; 
tozcUserIdentifierType newId;
tozcUserIdentifierType readNewId;
tozcUserIdentifierType id;
time_t readTime;

  	newUser = (tozcUserDataType *)malloc(sizeof(tozcUserDataType));
 
  	if (!strcmp(userNickname,""))
    		snprintf(newUser->nickname,TOZC_MAX_NICKNAME_SIZE+1,"%s",nickname);
  	
	else
    		snprintf(newUser->nickname,TOZC_MAX_NICKNAME_SIZE+1,"%s",userNickname);
    
 
  	snprintf(dataFilename,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);
  
  	fileDataRead = fopen(dataFilename,"r");
  	if (fileDataRead == NULL)
    		return changeUserProfileOpenReadfile;

  	foundUser =0 ;

  	while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, fileDataRead)) != NULL){ 
   		if(buffer[strlen(buffer)-1]!='\n'){
	      		fclose(fileDataRead);
	      		return changeUserInvalidUsersFile;
	    	}
  
	    	strcpy(bufferMod,buffer);
	    	uidStr = strtok(bufferMod, ":");

	    	newNickname = strtok(NULL,":");

 
	    	if (!strcmp(nickname,newNickname)){
	      		foundUser =1;
	     		 newId = (tozcUserIdentifierType)strtoull(uidStr,&verification,10);
	      
			if (verification[0]!='\0'){
	        		fclose(fileDataRead);
	        		return changeUserInvalidUsersFile;
	      		}
	      	break;
    		}
  	}



  	fclose(fileDataRead);
  	if (foundUser != 1)
    		return changeUserUserNotFound;
  


  	fileDataRead = fopen(dataFilename,"r");
  	if (fileDataRead == NULL)
    		return changeUserProfileOpenReadfile;

 
  	fileDataWrite = fopen("temp5","w");
  	if (fileDataWrite == NULL){
    		fclose(fileDataRead);
    		return changeUserProfileOpenWritefile;
  	}

  	foundUser =0 ;

  	while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, fileDataRead)) != NULL){ 
	    	if(buffer[strlen(buffer)-1]!='\n'){
	    		fclose(fileDataRead);
	      		fclose(fileDataWrite);
	      		remove("temp5");
	      		return changeUserInvalidUser;
	    	}
  
	    	strcpy(bufferMod,buffer);
	    	uidStr = strtok(bufferMod, ":");

    		newNickname = strtok(NULL,":");
    		passwordCrypt = strtok(NULL,":");
	    
		if (TozcCheckStringField(passwordCrypt,TOZC_PASSWORD_VALID_CHARS,TOZC_MIN_PASSWORD_SIZE,TOZC_MAX_PASSWORD_SIZE)){ 
	      		profile = (char *)malloc(sizeof(char)*(strlen(passwordCrypt)+1));     
	      		strcpy(profile,passwordCrypt);
	    	}
	    
		else
	    
		{
	      		strcpy(newUser->password,passwordCrypt);
	      		profile = strtok(NULL,":");
	    	}

   	 	username = strtok(NULL,":");

	    	error = TozcCheckStringField (username, TOZC_USERNAME_VALID_CHARS, TOZC_MIN_USERNAME_SIZE, TOZC_MAX_USERNAME_SIZE);
	    	if (error){
	      		fclose(fileDataRead);
	      		fclose(fileDataWrite);
	      		remove("temp5");
	      		return error;
	    	}

    		email = strtok(NULL,"\n");

	    	error = TozcCheckStringField (email, TOZC_EMAIL_VALID_CHARS, TOZC_MIN_EMAIL_SIZE, TOZC_MAX_EMAIL_SIZE);
	    	
		if (error){
	      		fclose(fileDataRead);
	      		fclose(fileDataWrite);
	      		remove("temp5");
	      		return error;
	    	}

		if (!strcmp(newUser->nickname,nickname)){
	      		foundUser = 1;
	      		newUser->uid = (tozcUserIdentifierType)strtoull(uidStr,&verification,10);
	     		if (verification[0]!='\0'){
	        		fclose(fileDataRead);
	        		fclose(fileDataWrite);
	        		remove("temp5");
	        		return changeUserInvalidUsersFile;
	      		}



      			strcpy(newUser->name,username);
      			strcpy(newUser->email,email);

      			if(!strcmp(profile,"0"))
        			newUser->profileType = profileAdmin;
      			else 
		
				if(!strcmp(profile,"1"))
        				newUser->profileType = profileUser;
      
				else{
        				fclose(fileDataRead);
        				fclose(fileDataWrite);
        				remove("temp5");
        				return changeUserProfileNotFound; 
      				}
   
      			if (newUser->profileType)
        			newUser->profileType = profileAdmin;
      
			else{
    
        			snprintf(lockFilename,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_LOCKED_USERS_DATA_FILENAME);
        			fileLockRead = fopen(lockFilename,"r");
        			if (fileLockRead!=NULL){
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
              					return changeUserInvalidLockFile;
            				}
            		
				linesRead = fread(&readTime,sizeof(time_t),1,fileLockRead);
            
				if (linesRead<=0){
              				fclose(fileDataRead);
              				fclose(fileDataWrite);
              				remove("temp5");
              				fclose(fileLockRead);
              				return changeUserInvalidLockFile;
            			}
            
				linesRead = fread(&readNewId,sizeof(tozcUserIdentifierType),1,fileLockRead);
            
				if (linesRead<=0){
              				fclose(fileDataRead);
              				fclose(fileDataWrite);
              				remove("temp5");
              				fclose(fileLockRead);
              				return changeUserInvalidLockFile;
           	 		}	

            			if(readNewId==newId){
             	 			fclose(fileDataRead);
              				fclose(fileDataWrite);
              				remove("temp5");
              				fclose(fileLockRead);
              				return changeUserCantChange;
            			}     
          		}
          		fclose(fileLockRead);
        	}
        
		snprintf(requestFileName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_REQUESTING_USERS_DATA_FILENAME);
        
		fileRequestRead = fopen(requestFileName,"r");
        
		if (fileRequestRead!=NULL){
          		linesRead =1 ;
          		while(linesRead>0){
            			linesRead = fread(&readTime,sizeof(time_t),1,fileRequestRead);
            			if (linesRead<=0)
             			 break;
            			
			linesRead = fread(&id,sizeof(tozcUserIdentifierType),1,fileRequestRead);
            
			if (linesRead<=0){
              			fclose(fileDataRead);
              			fclose(fileDataWrite);
              			remove("temp5");
              			fclose(fileRequestRead);
              			return changeUserInvalidRequestFile;
            		}
            
			linesRead = fread(&readNewId,sizeof(tozcUserIdentifierType),1,fileRequestRead);
            
			if (linesRead<=0){
              			fclose(fileDataRead);
              			fclose(fileDataWrite);
              			remove("temp5");
              			fclose(fileRequestRead);
              			return changeUserInvalidRequestFile;
            		}

            linesRead = fread(passwordCrypt,sizeof(char),TOZC_SHA512_MAX_PASSWORD_SIZE,fileRequestRead);
            
			if (linesRead<=0){
              			fclose(fileDataRead);
              			fclose(fileDataWrite);
              			remove("temp5");
              			fclose(fileRequestRead);
              			return changeUserInvalidRequestFile;
            		}

            		if(readNewId==newId){
              			fclose(fileDataRead);
              			fclose(fileDataWrite);
              			remove("temp5");
              			fclose(fileRequestRead);
              			return changeUserCantChange;
            		}   
          	}
          	fclose(fileRequestRead);
        }
        newUser->profileType = profileUser;
      	} 
      	fprintf(fileDataWrite,"%llu:%s:%s:%i:%s:%s\n",newUser->uid,newUser->nickname,newUser->password,newUser->profileType,newUser->name,newUser->email);
    	}
    
	else
     		fprintf(fileDataWrite,"%s",buffer); 
	}

  	fclose(fileDataRead);
 	fclose(fileDataWrite);
  	rename("temp5",dataFilename);
 
  	if (!foundUser)
    		return changeUserUserNotFound;
       
 
return ok;
} 
/*$RCSFile$*/
