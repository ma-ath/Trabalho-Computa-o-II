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
#include "tozcChangeUserEmail.h"
#include "Libraries/sendmail.h"




tozcErrorType
TozcChangeUserEmail(char *user,char *nickname,char *email, char *emailCheck)
{
FILE *fileEmail;
tozcUserDataType *aux,*list;
size_t index;
time_t timeExp;
tozcUserIdentifierType uid;
char mailBody[TOZC_EMAIL_BODY_SIZE+1];
char fileEmailName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char key[TOZC_MAX_KEY_SIZE+1];
int found;
tozcErrorType erro;


  	if(nickname[0]=='\0')
    		strcpy(nickname,user);
  	erro=TozcCheckNickname(nickname,TOZC_NICKNAME_VALID_CHARS,TOZC_MIN_NICKNAME_SIZE,TOZC_MAX_NICKNAME_SIZE);
  	if(erro)
    		return erro;

  	erro=TozcGetUsers(&list);
  	if(erro){
    		TozcFreeMemory(&list);
    		return erro;
  	}

  	if(!list){
    		TozcFreeMemory(&list);
    		return changeUserEmailInvalidFile;
  	}
  
  	found=0;
  
	aux=list;
  
	while(aux){
    		if(!strcmp(aux->nickname,nickname)){
      			found=1;
      			uid=aux->uid;
    		}
    
		aux=aux->next;
  	}
  
	TozcFreeMemory(&list);
  
  	if(found==0)
    		return changeUserEmailUserNotFound;

        snprintf(fileEmailName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_EMAIL_ABEYANCES_DATA_FILENAME);

	printf("%s\n",fileEmailName);

  	erro=TozcCreateRandomString(TOZC_SALT_VALID_CHARS,TOZC_MAX_KEY_SIZE,key);
  	if(erro)
    		return erro;

  	timeExp=time(NULL)+TOZC_EMAIL_EXPIRATION_TIME;
  
  	for(index=strlen(email);index<TOZC_EMAIL_BODY_SIZE;index++)
   		 email[index]='\0';
	


	fileEmail = fopen(fileEmailName,"a"); 


 
	if (fileEmail == NULL)
    		return changeUserEmailOpenFileError;

  	fwrite(&timeExp,1,sizeof(time_t),fileEmail);
  	fwrite(&uid,1,sizeof(tozcUserIdentifierType),fileEmail);
  	fwrite(email,1,TOZC_EMAIL_BODY_SIZE,fileEmail);
  	fwrite(key,1,TOZC_MAX_KEY_SIZE,fileEmail);
  	fclose(fileEmail);
  
  	snprintf(mailBody,TOZC_EMAIL_BODY_SIZE+1,
            
	 "\nVoce tem 48 horas para confirmar esse email. Sua chave de validacao e :%s",
        
	     key);
  
	sendMail(TOZC_SMTP_CLIENT_DOMAIN,TOZC_SMTP_SERVER_FULL_HOSTNAME,TOZC_SMTP_SERVER_PORT,TOZC_MAIN_ADMIN_EMAIL,email,NULL,NULL,"380Caronas - Confirmacao",mailBody,NULL);

  return ok;


}

/*$RCSfile$*/	
