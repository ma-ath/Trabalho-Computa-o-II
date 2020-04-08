/* Universidade Federal do Rio de Janeiro
 * Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * EEL270 - Computacao II - Turma 2015/2
 * Prof. Marcelo Luiz Drumond Lanza
 * 
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

tozcLanguageType
TozcGetLanguageIndex(char* language)
{
        if (language == NULL)
                return tozcEnglish;

        if (((strcmp(language,"portuguese") != 0) && (strcmp(language,"'portuguese'") != 0)))
                return tozcEnglish;
        else
                return tozcPortuguese;
}


char *
TozcGetAbsolutFileName (char *directory, char *file)
{
	static char string[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
 
	strcpy(string,directory);
	strcat(string,file);
  
  return string;
}



tozcErrorType
TozcCheckStringField (char *string, char *caracteresValidos, unsigned minimo, unsigned maximo)
{
int tamanho,loop,loop2;

if (string == NULL)
        return checkStringFieldArgumentNull;
if (caracteresValidos == NULL)
        return checkStringFieldArgumentNull;
if (minimo > maximo)
        return checkStringFieldMinGreaterThanMax;


for(loop = 0;string[loop] != '\0';loop++)
        {
        loop2 = 0;
        while(caracteresValidos[loop2] != string[loop])
                {
                if (caracteresValidos[loop2] == '\0')
                        return checkStringFieldInvalidCaracter;
                loop2++;
                }
        }
tamanho = strlen(string);

if ((tamanho < minimo)||(tamanho > maximo))
        return checkStringFieldInvalidLength;

return ok;
}



tozcErrorType
TozcCheckNickname (char *string, char *validChars, unsigned minStringSize, unsigned maxStringSize)
{
unsigned stringSize, stringIndex, charsIndex;
unsigned charFound, dotFound;
 
dotFound=0;

	if ((string == NULL) || (validChars==NULL))
		return checkNicknameArgumentNull;
  
	for (stringIndex=0;string[stringIndex]!='\0';stringIndex++){

		if (string[stringIndex] == '.'){
			if (dotFound)
				return checkNicknameInvalidCaracter;
      			else
        			dotFound=1;
    		}

		charFound = 0;

		for (charsIndex=0;validChars[charsIndex]!='\0';charsIndex++){

			if (string[stringIndex] == validChars[charsIndex]){
				charFound=1;
	        	break;
	      		}
	    	}

		if (!charFound)
			return checkNicknameInvalidCaracter;

  	}

	stringSize = stringIndex;

	if ((stringSize<minStringSize)||(stringSize>maxStringSize))
		return checkNicknameInvalidLength;

  return ok;
}

tozcErrorType
TozcCheckEmail (char *string, char *validChars, unsigned minStringSize, unsigned maxStringSize)
{
unsigned stringSize, stringIndex, charsIndex;
unsigned charFound, atFound;
 
atFound=0;

	if ((string == NULL) || (validChars==NULL))
		return checkEmailArgumentNull;
  
	for (stringIndex=0;string[stringIndex]!='\0';stringIndex++){

		if (string[stringIndex] == '@'){
			if (atFound)
				return checkEmailInvalidCaracter;
      			else
        			atFound=1;
    		}
    
    		charFound = 0;
    
		for (charsIndex=0;validChars[charsIndex]!='\0';charsIndex++){
      			if (string[stringIndex] == validChars[charsIndex]){
        			charFound=1;
        		break;
      			}
    		}
    		if (!charFound)
      			return checkEmailInvalidCaracter;

  	}

	stringSize = stringIndex;

  	if ((stringSize<minStringSize)||(stringSize>maxStringSize))
    		return checkEmailInvalidLength;

  return ok;
}

tozcErrorType
TozcCreateRandomString (char* caracteresValidos, size_t comprimento, char* stringGerada)
{

unsigned int loop = 0;
unsigned int loop2= 0;

/*Tratamento de erros*/
if (caracteresValidos == NULL)
        return createRandomStringNullArgument;

if (stringGerada == NULL)
        return createRandomStringNullArgument;

if (comprimento <= 0)
        return createRandomStringInvalidLength;

for(loop = 0;caracteresValidos[loop] != '\0';loop++)
   {
   for (loop2 = 0;caracteresValidos[loop2] != '\0';loop2++)
        {
        if ((caracteresValidos[loop2] == caracteresValidos[loop])&&(loop != loop2))
                return createRandomStringInvalidCaracter;

        }
   }
/*Depois de tratamento de erros*/
srand(time(NULL));

for(loop = 0;loop <= comprimento;loop++)
{
  stringGerada[loop] = caracteresValidos[rand() % strlen(caracteresValidos)];
  stringGerada[loop+1] = '\0';
}

return ok;
}



tozcErrorType
TozcCreateNickname (char *username, char *firstOption, char *secondOption)
{
unsigned index,charIndex,usernameLength,numberOfSpaces;
int previousSpaceIndex,lastSpaceIndex;
char lowerUsername[TOZC_MAX_USERNAME_SIZE+1];
char firstName[TOZC_MAX_USERNAME_SIZE+1];
char beforeLastName[TOZC_MAX_USERNAME_SIZE+1];
char lastName[TOZC_MAX_USERNAME_SIZE+1];

	if (username==NULL)
    		return createNicknameArgumentNull;

  numberOfSpaces=0;

  lastSpaceIndex = -1;

  	for (charIndex=0;username[charIndex]!='\0';charIndex++){
    		lowerUsername[charIndex] = (char)tolower((int)username[charIndex]);
    		if (username[charIndex]==' '){
      			numberOfSpaces++;
      			previousSpaceIndex = lastSpaceIndex;
      			lastSpaceIndex = charIndex;
      			if (numberOfSpaces==1)
       				snprintf(firstName,charIndex+1,"%s",lowerUsername);
    		}  
  	}

  	usernameLength = charIndex;
  
	if (!numberOfSpaces)
    		return createNicknameInvalidLength;

  	index=0;

  	for (charIndex=lastSpaceIndex+1;username[charIndex]!='\0';charIndex++){
    		lastName[index] = lowerUsername[charIndex];
    		index++;
  	}

  	lastName[index]='\0';
  	snprintf(firstOption,TOZC_MAX_NICKNAME_SIZE,"%s.%s",firstName,lastName);


  	if (numberOfSpaces>1){
    		index=0;
    		for (charIndex=previousSpaceIndex+1;charIndex<lastSpaceIndex;charIndex++){
      			beforeLastName[index] = lowerUsername[charIndex];
      			index++;
    		}
    	beforeLastName[index]='\0';
    	snprintf(secondOption,TOZC_MAX_NICKNAME_SIZE,"%s.%s",firstName,beforeLastName);
  	}
return ok;
}


tozcErrorType
TozcGetCryptAlgorithm (char *cryptPassword, tozcCryptAlgorithms *cryptAlgorithm)
{
unsigned cryptPasswordSize;


  	if (cryptPassword == NULL)
    		return getCryptAlgorithmArgumentNull;

  	cryptPasswordSize = strlen(cryptPassword);

  	if (cryptPasswordSize < 13)
    		return getCryptAlgorithmInvalidCryptPasswordSize;

  	if ((cryptPassword[0]!='$') && (cryptPasswordSize==TOZC_DES_MAX_PASSWORD_SIZE))
		*cryptAlgorithm = des;

  	else

if ((cryptPassword[1]=='1')&&((cryptPassword[2]=='$') && ((cryptPasswordSize>=TOZC_MD5_MIN_PASSWORD_SIZE) || cryptPasswordSize<=TOZC_MD5_MAX_PASSWORD_SIZE)))
    	*cryptAlgorithm = md5;
	else

if ((cryptPassword[1]=='5')&&((cryptPassword[2]=='$') && ((cryptPasswordSize>=TOZC_SHA256_MIN_PASSWORD_SIZE) || cryptPasswordSize<=TOZC_SHA256_MAX_PASSWORD_SIZE)))
	*cryptAlgorithm = sha256;
	else

if ((cryptPassword[1]=='6')&&((cryptPassword[2]=='$') && ((cryptPasswordSize>=TOZC_SHA512_MIN_PASSWORD_SIZE) || cryptPasswordSize<=TOZC_SHA512_MAX_PASSWORD_SIZE)))
    *cryptAlgorithm = sha512;
	else

    return getCryptAlgorithmInvalidArgument;    


return ok;
}




tozcErrorType
TozcEncodePasswordWithSpecificAlgorithm (char *password, tozcCryptAlgorithms cryptAlgorithm, char *cryptPassword)
{
tozcErrorType error;

char salt[TOZC_MAX_SALT_SIZE+1],fullSalt[TOZC_MAX_FULLSALT_SIZE+1];

  	if (password == NULL)
    		return encodePasswordWithSpecificAlgorithmArgumentNull;

  	switch (cryptAlgorithm){
    		case des:
      			error = TozcCreateRandomString (TOZC_SALT_VALID_CHARS, TOZC_MIN_SALT_SIZE, salt);
      			if (error)
        			return error;
      			cryptPassword=crypt(password,salt);
    		break;

	    	case md5:
      			error = TozcCreateRandomString (TOZC_SALT_VALID_CHARS, TOZC_MD5_MAX_SALT_SIZE, salt);
      			if (error)
        			return error;
      			snprintf(fullSalt,TOZC_MAX_SALT_SIZE,"$1$%s",salt);
      			strcpy(cryptPassword,crypt(password,fullSalt));
    		break;

    		case sha256:
      			error = TozcCreateRandomString (TOZC_SALT_VALID_CHARS, TOZC_SHA256_MAX_SALT_SIZE, salt);
     	 		if (error)
        			return error;
      			snprintf(fullSalt,TOZC_MAX_SALT_SIZE,"$5$%s",salt);
      			strcpy(cryptPassword,crypt(password,fullSalt));
   	 	break;
   
 		case sha512:
			error = TozcCreateRandomString (TOZC_SALT_VALID_CHARS, TOZC_SHA512_MAX_SALT_SIZE, salt);
      			if (error)
        			return error;

      			snprintf(fullSalt,TOZC_MAX_SALT_SIZE,"$6$%s",salt);
      			strcpy(cryptPassword,crypt(password,fullSalt));
		break;
  	}	

return ok;
}



tozcErrorType
TozcEncodePasswordWithSpecificSalt (char *password, char *salt, char *cryptPassword)
{

tozcErrorType error;

 
  	if (password == NULL)
    		return encodePasswordWithSpecificSaltArgumentNull;
  
	if(salt == NULL)
    		return encodePasswordWithSpecificSaltArgumentNull;

 
	error = TozcCheckStringField (salt, TOZC_SALT_VALID_CHARS, TOZC_MIN_SALT_SIZE, TOZC_MAX_SALT_SIZE);

  	if ((error)&&(error != checkStringFieldInvalidCaracter))
    		return error;

  	if (error){
    		if (strlen(salt)<3)
      			return encodePasswordWithSpecificSaltInvalidSalt;
    		if ((salt[0]!='$')||(salt[2]!='$'))
      			return encodePasswordWithSpecificSaltInvalidSalt;
  	} 

  	cryptPassword=crypt(password,salt);

return ok;
}





tozcErrorType
TozcCheckPassword (char *password, char *cryptPassword)
{
unsigned cryptPasswordSize;
unsigned charIndex;
unsigned especialCaracter;
char salt[TOZC_MAX_SALT_SIZE+1];


  	if (password == NULL)
    		return checkPasswordArgumentNull;
  
	if (cryptPassword == NULL)
    		return checkPasswordArgumentNull;

  cryptPasswordSize = strlen(cryptPassword);
  
	if ((cryptPasswordSize < TOZC_DES_MAX_PASSWORD_SIZE)||(cryptPasswordSize > TOZC_SHA512_MAX_PASSWORD_SIZE))
    		return checkPasswordInvalidCryptPasswordSize;


  	if ((cryptPassword[0]!='$') && (cryptPasswordSize==TOZC_DES_MAX_PASSWORD_SIZE)){
    		snprintf(salt,TOZC_MIN_SALT_SIZE, "%s",cryptPassword);
  	}

  	else 
	if ((cryptPassword[0]=='$') && (cryptPassword[2]=='$')){ 
			especialCaracter=0;
			for (charIndex=0;charIndex<cryptPasswordSize;charIndex++){
				if (cryptPassword[charIndex]=='$'){
					especialCaracter++;
					if (especialCaracter==3)
					break;
				}
			}

			if (especialCaracter<3)
      			return checkPasswordInvalidCrypt;
    
    		snprintf(salt,charIndex+1, "%s",cryptPassword);
  	}
  	else
		return checkPasswordInvalidCrypt;    

  	if (strcmp(crypt(password,salt),cryptPassword))
    		return passwordInvalido;

return ok;
}




tozcErrorType
TozcAddUser(tozcUserDataType *User)
{
FILE *DataReadFile;
FILE *DataWriteFile;
FILE *AbeyancesReadFile;
FILE *AbeyancesWriteFile;
tozcErrorType Error;
char CryptPassword[TOZC_MAX_PASSWORD_SIZE + 1];
char CryptTemporaryPassword[TOZC_MAX_PASSWORD_SIZE + 1];
char DataFileName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char AbeyancesFileName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char *Nickname;
char NicknameFirstOption[TOZC_MAX_NICKNAME_SIZE+1];
char NicknameSecondOption[TOZC_MAX_NICKNAME_SIZE +1]; 
tozcCryptAlgorithms CryptAlgorithm;
unsigned MailError;
tozcUserIdentifierType Id, PreviousId;
char Buffer[TOZC_MAX_BUFFER_SIZE+1];
char ModifiedBuffer[TOZC_MAX_BUFFER_SIZE+1];
char *IdString;
char *Verification;
unsigned UserWasInvited,UserWasWritten;
char TemporaryPassword[TOZC_MAX_PASSWORD_SIZE+1];
time_t ExpirationTime,ReadExpirationTime;
int ReadLines;
char EmailBodyMessage[5000 +1];

	Error = TozcCheckEmail (User->email, TOZC_EMAIL_VALID_CHARS, TOZC_MIN_EMAIL_SIZE, TOZC_MAX_EMAIL_SIZE);

	if (Error)
    		return Error;
  	if (strcmp(User->email,User->emailCheck))
    		return addUserEmailNotCheck;
  
  	if (User->name==NULL)
    		return addUserNameNull;
  	if (User->nameCheck==NULL)
    		return addUserNameCheckNull;
  
  	Error = TozcCheckStringField (User->name, TOZC_USERNAME_VALID_CHARS, TOZC_MIN_USERNAME_SIZE, TOZC_MAX_USERNAME_SIZE);

  	if (Error)
    		return Error;
  	if (strcmp(User->name,User->nameCheck))
    		return addUserUsernameNotCheck;




	if ((strcmp(User->password,""))&&(strcmp(User->passwordCheck,""))){
    		UserWasInvited = 0;
    		
		Error = TozcCheckStringField (User->password, TOZC_PASSWORD_VALID_CHARS, TOZC_MIN_PASSWORD_SIZE, TOZC_MAX_PASSWORD_SIZE);
    		if (Error)
      			return Error;
    		if (strcmp(User->password,User->passwordCheck))
      			return addUserPasswordsNotCheck;

    		CryptAlgorithm = sha512;

    		Error = TozcEncodePasswordWithSpecificAlgorithm (User->password,CryptAlgorithm ,CryptPassword);
    		if (Error)
      			return Error;
	}
	else 
	if ((!strcmp(User->password,""))&&(!strcmp(User->passwordCheck,""))){
    		UserWasInvited = 1;
    		strcpy(CryptPassword,"");
    		TozcCreateRandomString(TOZC_PASSWORD_VALID_CHARS, 16, TemporaryPassword);
    		CryptAlgorithm = 512;
    		Error = TozcEncodePasswordWithSpecificAlgorithm(TemporaryPassword,CryptAlgorithm ,CryptTemporaryPassword);
    		if (Error)
      			return Error;
	}

	Error = TozcCreateNickname(User->name, NicknameFirstOption, NicknameSecondOption);
  	if (Error)
    		return Error;

  	strcpy(User->nickname,NicknameFirstOption);
	snprintf(DataFileName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);

  	DataReadFile = fopen(DataFileName,"r");
  	if (DataReadFile==NULL){
    		if (User->profileType != profileAdmin)
      			return addUserPermissionDenied;
    		DataWriteFile = fopen(DataFileName,"w");
   		if(DataWriteFile==NULL)
      			return addUserDataWriteOpeningError;

    	fprintf(DataWriteFile,"%llu:%s:%s:%i:%s:%s\n",User->uid,User->nickname,CryptPassword,User->profileType,User->name,User->email);
    	fclose(DataWriteFile);
  	}

  	else

  	{
    	while ((fgets (Buffer, TOZC_MAX_BUFFER_SIZE, DataReadFile)) != NULL){ 
      		if(Buffer[strlen(Buffer)-1]!='\n'){
        		fclose(DataReadFile);
        		return addUserInvalidUserFile;
      		}

      		IdString = strtok(Buffer, ":");
      		Nickname = strtok(NULL, ":");
      		if (!strcmp(Nickname,NicknameFirstOption))
        		strcpy(User->nickname,NicknameSecondOption);
    	}

    	fclose(DataReadFile); 
    
    	DataReadFile = fopen(DataFileName,"r");
    	if (DataReadFile == NULL)
      		return addUserOpeningFileError;

 
    	DataWriteFile = fopen("temp","w");
    	if (DataWriteFile == NULL){
      		fclose(DataReadFile);
      		return addUserOpeningFileError;
   	 }

    	PreviousId = 0;

    	while ((fgets (Buffer, TOZC_MAX_BUFFER_SIZE, DataReadFile)) != NULL){ 
      		if(Buffer[strlen(Buffer)-1]!='\n'){
        		fclose(DataReadFile);
        		fclose(DataWriteFile); 
        		return addUserInvalidUserFile;
      		}
      
		strcpy(ModifiedBuffer,Buffer);
      		IdString = strtok(ModifiedBuffer, ":");
      		Id = (tozcUserIdentifierType)strtoull(IdString,&Verification,10);
      		if (Verification[0]!='\0'){
        	
		fclose(DataReadFile);
        	fclose(DataWriteFile); 
        		return addUserInvalidUserFile;
      		}

      		if ((Id - PreviousId)>1){
        		User->uid = PreviousId +1;
        		fprintf(DataWriteFile,"%llu:%s:%s:%i:%s:%s\n",User->uid,User->nickname,CryptPassword,User->profileType,User->name,User->email);
      		}
     
      		fprintf (DataWriteFile, "%s", Buffer);
      		PreviousId = Id;
    	}

    	if (!User->uid){
      		User->uid = Id + 1;
      		fprintf(DataWriteFile,"%llu:%s:%s:%i:%s:%s\n",User->uid,User->nickname,CryptPassword,User->profileType,User->name,User->email);
    	}

    	if (!UserWasInvited)
      		MailError = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME, TOZC_SMTP_SERVER_PORT, TOZC_EMAIL_FROM,User->email,NULL,NULL,
               "Seu cadastro foi feito com sucesso!",

	"Parabéns!\n\tBem vindo ao 380Caronas, o único lugar onde a passagem não vai aumentar!"

		, NULL);

    	fclose(DataReadFile);
    	fclose(DataWriteFile); 
    	rename("temp",DataFileName);
  
	}

  	if (UserWasInvited){
    		UserWasWritten = 0;
    		snprintf(AbeyancesFileName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USERS_ABEYANCES_DATA_FILENAME);
    		
		AbeyancesReadFile = fopen(AbeyancesFileName,"r");
    		if (AbeyancesReadFile==NULL){

			AbeyancesWriteFile = fopen(AbeyancesFileName,"w");
      			if (AbeyancesWriteFile==NULL)
        			return addUserOpeningFileError;

      			snprintf(EmailBodyMessage,5000,"Sua senha temporaria e...:\n \"%s\" \n Time 380Caronas.",TemporaryPassword);
      			
			MailError = sendMail(TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME, TOZC_SMTP_SERVER_PORT, TOZC_EMAIL_FROM,User->email,NULL,NULL,

                   "Um convite para voce . . .", 

			EmailBodyMessage, NULL);

      			ExpirationTime = time(NULL) + TOZC_INVITATION_EXPIRATION_TIME;
      			fwrite(&ExpirationTime,sizeof(time_t),1,AbeyancesWriteFile);
      			fwrite(&User->uid,sizeof(tozcUserIdentifierType),1,AbeyancesWriteFile);
      			fwrite(CryptTemporaryPassword,sizeof(char),strlen(CryptTemporaryPassword),AbeyancesWriteFile);
      			fclose(AbeyancesWriteFile); 
    		}
    		else
    		{
      			AbeyancesWriteFile = fopen("temp2","w");
      			if (AbeyancesWriteFile == NULL){
        			fclose(AbeyancesReadFile);
        			return addUserOpeningFileError;
     			 }

      ReadLines =1 ;

     			while(ReadLines>0){
        			ReadLines = fread(&ReadExpirationTime,sizeof(time_t),1,AbeyancesReadFile);
        			if (ReadLines<=0)
          		break;

        			ReadLines = fread(&Id,sizeof(tozcUserIdentifierType),1,AbeyancesReadFile);
        			if (ReadLines<=0){
          				fclose(AbeyancesReadFile);
          				fclose(AbeyancesWriteFile);
          				return addUserInvalidAbeyancesFile;
       				 }

        			ReadLines = fread(&CryptPassword,sizeof(char),TOZC_MAX_PASSWORD_SIZE,AbeyancesReadFile);
        			if (ReadLines<=0){ 
          				fclose(AbeyancesReadFile);
          				fclose(AbeyancesWriteFile);
          				return addUserInvalidAbeyancesFile;
        			}

        			ExpirationTime = time(NULL) + TOZC_INVITATION_EXPIRATION_TIME;

        			if(ExpirationTime<ReadExpirationTime){
          				UserWasWritten = 1;
						snprintf(EmailBodyMessage,5000,"Sua senha temporaria e...:\n \"%s\" \nTime 380Caronas.",TemporaryPassword);

          				MailError = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME,
							      TOZC_SMTP_SERVER_PORT, TOZC_EMAIL_FROM,User->email,NULL,NULL,
                     "Voce foi convidado!"
							       , EmailBodyMessage, NULL);

          				fwrite(&ExpirationTime,sizeof(time_t),1,AbeyancesWriteFile);
          				fwrite(&User->uid,sizeof(tozcUserIdentifierType),1,AbeyancesWriteFile);
          				fwrite(CryptTemporaryPassword,sizeof(char),strlen(CryptTemporaryPassword),AbeyancesWriteFile);
        			} 
        
        			fwrite(&ReadExpirationTime,sizeof(time_t),1,AbeyancesWriteFile);
        			fwrite(&Id,sizeof(int),1,AbeyancesWriteFile);
        			fwrite(CryptPassword,sizeof(char),TOZC_MAX_PASSWORD_SIZE,AbeyancesWriteFile);
      			}

		if(!UserWasWritten){
     
			snprintf(EmailBodyMessage,5000,"Sua senha temporaria e...:\n \"%s\" \nTime 380Caronas.",TemporaryPassword);
       			MailError = sendMail (TOZC_SMTP_CLIENT_DOMAIN, TOZC_SMTP_SERVER_FULL_HOSTNAME,
					      TOZC_SMTP_SERVER_PORT, TOZC_EMAIL_FROM,User->email,NULL,NULL,
                    "Voce foi convidado!"
					    , EmailBodyMessage, NULL);

       			fwrite(&ExpirationTime,sizeof(time_t),1,AbeyancesWriteFile);
       			fwrite(&User->uid,sizeof(tozcUserIdentifierType),1,AbeyancesWriteFile);
       			fwrite(CryptTemporaryPassword,sizeof(char),strlen(CryptTemporaryPassword),AbeyancesWriteFile);
		} 
	
		fclose(AbeyancesReadFile);
		fclose(AbeyancesWriteFile);
		rename("temp2",AbeyancesFileName);
		}
  	}


return ok;
}



tozcErrorType
TozcAuthenticateUser (tozcUserDataType *User)
{
FILE *DataReadFile;
tozcErrorType Error;
char *CryptPassword;
char DataFileName[TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1];
char *Nickname;
char *IdString;
char *Username;
char *Email;
char *Profile; 
unsigned userFound;
char buffer[TOZC_MAX_BUFFER_SIZE+1];
char ModifiedBuffer[TOZC_MAX_BUFFER_SIZE+1];
char *verification;


	Error = TozcCheckNickname (User->nickname, TOZC_NICKNAME_VALID_CHARS, TOZC_MIN_NICKNAME_SIZE, TOZC_MAX_NICKNAME_SIZE);
    	if (Error)
      		return Error;

  	snprintf(DataFileName,TOZC_MAX_DIRECTORY_SIZE+TOZC_MAX_FILENAME_SIZE+1, "%s%s", TOZC_DATA_DIRECTORY,TOZC_USER_DATA_FILENAME);
  
  	DataReadFile = fopen(DataFileName,"r");
  	if (DataReadFile == NULL)
    		return authenticateUserOpeningFileError;
 
  	userFound =0;

  	while ((fgets (buffer, TOZC_MAX_BUFFER_SIZE, DataReadFile)) != NULL){ 
    		if(buffer[strlen(buffer)-1]!='\n'){
      			fclose(DataReadFile);
      			return authenticateUserInvalidUserFile;
    		}
    
		strcpy(ModifiedBuffer,buffer);
    		IdString = strtok(ModifiedBuffer, ":");

    		Nickname = strtok(NULL,":");
    	
		if (!strcmp(User->nickname,Nickname)){
      			userFound = 1;
      			User->uid = (tozcUserIdentifierType) strtoull (IdString,&verification,10);
      	
			if (verification[0]!='\0'){
	        		fclose(DataReadFile);
        			return authenticateUserInvalidUserFile;
      			}

      			CryptPassword = strtok(NULL,":");
      		
			Error = TozcCheckPassword(User->password, CryptPassword);
      			if (Error){
	        		fclose(DataReadFile);
        			return Error;
      			}
	       
      			Profile = strtok(NULL,":");
      
			if(!strcmp(Profile,"0"))
        			User->profileType = profileAdmin;
	
	      		else 
	
				if(!strcmp(Profile,"1"))
        				User->profileType = profileUser;
      		
				else 
	      				{
		        			fclose(DataReadFile);
        					return authenticateUserProfileNotFound; 
      					}


	      		Username = strtok(NULL,":");
      
			Error = TozcCheckStringField (Username, TOZC_USERNAME_VALID_CHARS, TOZC_MIN_USERNAME_SIZE, TOZC_MAX_USERNAME_SIZE);
	      		if (Error){
        			fclose(DataReadFile);
        			return Error;
      			}

	      		strcpy(User->name,Username);

      			Email = strtok(NULL,"\n");
      	
			Error = TozcCheckStringField (Email, TOZC_EMAIL_VALID_CHARS, TOZC_MIN_EMAIL_SIZE, TOZC_MAX_EMAIL_SIZE);
	      		if (Error){
        		fclose(DataReadFile);
        			return Error;
	      		}
      	
			strcpy(User->email,Email);
	
    		}
  	}
  
	if (!userFound)
    		return authenticateUserUserNotFound; 


fclose(DataReadFile);
return ok;
}



tozcErrorType
TozcCreateSimpleList (tozcNicknameListType **first,tozcNicknameListType *user)
{

tozcNicknameListType *last,*aux;

	if(user==NULL)
    		return createSimpleListArgumentNull;

  	aux = *first;

  	if(*first==NULL)
	{
	    *first = user;
	    return ok;
	}
  aux = *first;

	  while(aux != NULL)
		  {
			


		    if (strcmp(user->nickname,aux->nickname) < 0)
			    {

			      if(aux == *first)
				      {
				        (user)->next = *first;
				        *first = user;
				      }
			      else
				      {
				        (user)->next = aux;
				        last->next = user;
				      }
	      break;
		   	    }
		    last = aux;
		    last = last->next;
      

		  }
	  if (aux == NULL)
	  {
	    last->next = user;
	    (user)->next = NULL;
	  }

return ok;    
}




tozcErrorType
TozcCopy(char *originalName,char *copyName)
{
FILE *original;
FILE *copy;
unsigned char buffer[TOZC_MAX_BUFFER_SIZE];
size_t size;
   
	original=fopen(originalName,"r"); 
	if (original == NULL)
    		return copyFileError;
  	
	copy=fopen(copyName,"w"); 
  	if (copy == NULL){
   		fclose(original);
    		return copyOpenFileError;
  	}

  	while((size=fread(buffer,1,TOZC_MAX_BUFFER_SIZE,original))){
    		if(ferror(original)){

      			fclose(original);
      			fclose(copy);
      			return copyReadFileError;
    		}
    		fwrite(buffer,1,size,copy);
  	}

  	fclose(original);
  	fclose(copy);
return ok;
}

void
TozcFreeMemory(tozcUserDataType **list)
{
tozcUserDataType *last;
tozcUserDataType *next;
tozcUserDataType *aux;
  
  	next=*list;
  	if(next)
  		last=next->last;
  	while(next){
    		aux=next;
    		next=aux->next;
    		free(aux);
  	}
  
	while(last){
    		aux=last;
    		last=aux->last;
    		free(aux);
  	}
}

void
TozcFreeList(tozcNicknameListType **list)
{
tozcNicknameListType *next, *aux;
  
next=*list;
while(next)
{
 aux=next;
 next=aux->next;
 free(aux);
}
}
/*$RCSfile$*/ 
