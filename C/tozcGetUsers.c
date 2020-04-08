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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tozcError.h"
#include "tozcConfig.h"
#include "tozcConst.h"
#include "tozcTypes.h" 
#include "tozcFunctions.h" 
#include "tozcGetUsers.h"


tozcErrorType
TozcGetUsers (tozcUserDataType **first)
{
FILE *fileUsers;
tozcUserDataType *next;
tozcUserDataType *firstAux = NULL;
tozcUserDataType *aux = NULL;
char buffer[TOZC_MAX_BUFFER_SIZE+2];
char * string;
char * validation;	
char fileUsersName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];

	
  	snprintf(fileUsersName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME); 
  
  	fileUsers=fopen(fileUsersName,"r"); 
  	if(!fileUsers)
    		return getUsersOpenReadFile;
  	while(fgets(buffer, TOZC_MAX_BUFFER_SIZE, fileUsers) != NULL){
    		if(ferror(fileUsers)){
      			fclose(fileUsers);
      			return getUsersOpenReadFile;
    		}

	    	next = (tozcUserDataType *) malloc(sizeof(tozcUserDataType));	
	    	if(next == NULL){	
      			fclose(fileUsers);
      			free(next);
	      		return(getUsersNoMemory);
    		}
		
	    	string = strtok(buffer, ":"); 
    		
		if(buffer[0] == '-'){	
      			fclose(fileUsers);
      			free(next);
	      		return getUsersFileErro;
    		}

	    	next->uid = strtoul(string, &validation, 10);
    		if((*validation) !='\0'){	
      			fclose(fileUsers);
	      		free(next);
			return getUsersFileErro;
	    	}

		string = strtok(NULL, ":"); 
		strcpy(next->nickname, string);	
	
		string = strtok(NULL, ":"); 
		
		string = strtok(NULL, ":"); 
		next->profileType = (unsigned short) strtoul(string, &validation, 10);
		if(strlen(validation) != 0)
		{
			fclose(fileUsers);
			free(next);
			return getUsersFileErro;
		}
		string = strtok(NULL, ":"); 
		strcpy(next->name, string);
		
		string = strtok(NULL, "\0"); 
		strncpy(next->email, string, TOZC_MAX_EMAIL_SIZE+1);
      if(next->email[strlen(next->email)-1]=='\n')
        next->email[strlen(next->email)-1]='\0'; 

		next->next = NULL;

		if(!firstAux)
		{
			next->last = NULL;
			next->next = NULL;
			firstAux = next;
			aux = firstAux;
		}
		else
		{
			next->last = aux;
			aux->next = next;
			
			aux = next;
			
		}

	}	
	
	
	*first = firstAux;

	fclose(fileUsers);	
		
	return ok;
}
/*$RCSfile$*/	
