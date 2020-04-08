/* 
  * Universidade Federal do Rio de Janeiro 
  * Escola Politecnica 
  * Departamento de Eletronica e de Computacao 
  * Prof. Marcelo Luiz Drumond Lanza 
  * EEL 270 - Computacao II - Turma 2015/2
  * 
  */ 

#ifndef _TOZC_FUNCTIONS_
#define _TOZC_FUNCTIONS_    "@(#)tozcFunctions.h$" 
/* codigo do arquivo */ 


#include "tozcTypes.h"
#include "tozcError.h"

/*
 * tozcLanguageType
 * TozcGetLanguageIndex (char *);
 *
 * Arguments:
 * char* - set of valid characters for the string (aka language name) 
 *
 * Returned code:
 * tozcLanguageType - English(0) or Portuguese(1)
 *   . . .
 * Description:
 * This function returns the corresponding language type given a name of a certain language
 * Returns 'english' as default if the language is not implemented in the system.
 */
tozcLanguageType
TozcGetLanguageIndex(char *);

/*
 * void
 * TozcShowCliHelp (tozcLanguageType);
 *
 * Arguments:
 * language 
 *
 * Returned code:
 * nothing
 *   . . .
 * Description:
 * This function prints the Cli Help
 */
void
TozcShowCliHelp (tozcLanguageType);


tozcErrorType
TozcRunNcursesInterface (tozcLanguageType, char *);

/* char*
 * tozcGetAbsolutFileName(char*,char*);
 *
 * Arguments:
 * char* - 1st string
 * char* - 2nd string
 *
 * Returned code:
 * this function returns the contatenation of the 1st string, followed by de 2nd string
 *   . . .
 * Description:
 * this function returns the contatenation of the 1st string, followed by de 2nd string
 *
 */
char *
TozcGetAbsolutFileName (char *, char *);

/* tozcErrorType
 * TozcCheckStringField (char *, char *, size_t, size_t);
 *
 * 
 *
 * Arguments:
 * char* - String to be validated
 * char* - Valid caracters
 * size_t - min str length
 * size_t - max str length
 *
 * Returned code:
 * this function returns ok if the string is made by the set of valid caracters AND has length in betwen max and min size;
 *   . . .
 * Description:
 * this funtion is used to validate a string
 *
 */
tozcErrorType
TozcCheckStringField (char *, char *, unsigned, unsigned);

/* tozcErrorType
 * TozcCheckNickname (char *, char *, size_t, size_t);
 *
 * 
 *
 * Arguments:
 * char* - String to be validated
 * char* - Valid caracters
 * size_t - min str length
 * size_t - max str length
 *
 * Returned code:
 * this function returns ok if the string is made by the set of valid caracters, has at max one dot AND has length in betwen max and min size;
 *   . . .
 * Description:
 * this funtion is used to validate a string
 *
 */
tozcErrorType
TozcCheckNickname (char *, char *, unsigned, unsigned);

/* tozcErrorType
 * TozcCheckEmail (char *, char *, size_t, size_t);
 *
 * 
 *
 * Arguments:
 * char* - String to be validated
 * char* - Valid caracters
 * size_t - min str length
 * size_t - max str length
 *
 * Returned code:
 * this function returns ok if the string is made by the set of valid caracters, has at max one '@' AND has length in betwen max and min size;
 *   . . .
 * Description:
 * this funtion is used to validate a string
 *
 */
tozcErrorType
TozcCheckEmail (char *, char *, unsigned, unsigned);

/* tozcErrorType
 * TozcCreateRandomString (char *,size_t,char*);
 *
 * 
 *
 * Arguments:
 * char* - String with valid caracters
 * size_t - Size of created string
 * char* - Random string
 *
 * Returned code:
 * this function returns ok if everything goes rigth on creating the random string
 *   . . .
 * Description:
 * this funtion is used to create a random string
 *
 */
tozcErrorType
TozcCreateRandomString (char *, unsigned, char *);

/* tozcErrorType
 * TozcCreateNickname(char *,char *,char *);
 *
 * Arguments:
 * char * - string with the full name (I)
 * char * - first nickname (O)
 * char * - second nickname (O)
 *
 * Returned code:
 * ok - successful function, or corresponding error
 *
 * Description:
 * This function returns ok if at least if the first nickname is generated,
 * if it is not generated, this function returns the correspondent error code
 */
tozcErrorType
TozcCreateNickname (char *, char *, char *);

tozcErrorType
TozcGetCryptAlgorithm (char *, tozcCryptAlgorithms *);

tozcErrorType
TozcEncodePasswordWithSpecificAlgorithm (char *, tozcCryptAlgorithms, char *);

tozcErrorType
TozcEncodePasswordWithSpecificSalt (char *, char *, char *);

tozcErrorType
TozcCheckPassword (char *, char *);

tozcErrorType 
TozcAddUser (tozcUserDataType *);

tozcErrorType
TozcAuthenticateUser (tozcUserDataType *);

tozcErrorType
TozcCreateSimpleList(tozcNicknameListType **,tozcNicknameListType *);

tozcErrorType
TozcCopy(char *,char *);

void
TozcFreeMemory(tozcUserDataType **);

void
TozcFreeList(tozcNicknameListType **);

#endif 


/* $RCSfile$ */ 
