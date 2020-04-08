/*
 * Universidade Federal do Rio de Janeiro
 * Escola Politecnica
 * Departamento de Eletronica e de Computacao
 * Prof. Marcelo Luiz Drumond Lanza
 * EEL 270 - Computacao II - Turma 2015/2
 *
 * $Author$
 * $Date$
 * $Log$
 */
#ifndef TOZC_CONST_H
#define TOZC_CONST_H "@(#)tozcConst.h $Revision$"

/***********Erros********************/
#define OK				0
#define INVALID_ENVIRONMENT		1
#define INVALID_GETOPT_LETTER		2
/************************************/

#define GETOPT_LONG_VALID_CARACTERS     "CNhaitjnqQvdUluprmecgsS"

/***********************************/
#define VALID_CARACTERS				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#define TOZC_NICKNAME_VALID_CHARS		"abcdefghijklmnopqrstuvwxyz. "
#define TOZC_MAX_NICKNAME_SIZE			65
#define TOZC_MIN_NICKNAME_SIZE			8

#define LANGUAGE_NAME_MAX_CARACTERS		14
#define BUFFER_READ_HTML_PAGE_SIZE		20

#define TOZC_MAX_KEY_SIZE			128


#define TOZC_MAX_BUFFER_SIZE			1024

#define TOZC_USERNAME_VALID_CHARS 		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. _"
#define TOZC_MIN_USERNAME_SIZE 			6
#define TOZC_MAX_USERNAME_SIZE			256


#define TOZC_INVITATION_EXPIRATION_TIME		3600

#define TOZC_USER_IDENTIFIER_MAX_LENGTH		1024
/*Directory Filename*/

#define TOZC_MAX_DIRECTORY_SIZE			1024
#define TOZC_MAX_FILENAME_SIZE			1024


/*Caracters*/

#define TOZC_EMAIL_VALID_CHARS			"_-.@abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#define TOZC_MIN_EMAIL_SIZE			7
#define TOZC_MAX_EMAIL_SIZE			1024

#define TOZC_PASSWORD_VALID_CHARS		"./'\"\\!@#$\%&*()-_=+0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*Password*/
#define TOZC_DES_MAX_PASSWORD_SIZE		13	

#define TOZC_MD5_MIN_PASSWORD_SIZE 		27
#define TOZC_MD5_MAX_PASSWORD_SIZE		34

#define TOZC_SHA256_MIN_PASSWORD_SIZE		48	
#define TOZC_SHA256_MAX_PASSWORD_SIZE		63

#define TOZC_SHA512_MIN_PASSWORD_SIZE		91	
#define TOZC_SHA512_MAX_PASSWORD_SIZE		106

#define TOZC_MAX_SALT_SIZE			20
#define TOZC_MAX_FULLSALT_SIZE			50
#define TOZC_SALT_VALID_CHARS			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./"

#define TOZC_MIN_SALT_SIZE			2
#define TOZC_MD5_MAX_SALT_SIZE			15
#define TOZC_SHA256_MAX_SALT_SIZE		15
#define TOZC_SHA512_MAX_SALT_SIZE		15


#define TOZC_MAX_PASSWORD_SIZE			127
#define TOZC_MIN_PASSWORD_SIZE			6
#define TOZC_MAX_PASSWORD_SIZE_CRYPT		1024


#define TOZC_USER_DATA_FILENAME			"UserData"
#define TOZC_REQUESTING_USERS_DATA_FILENAME	"RequestData"
#define TOZC_EMAIL_ABEYANCES_DATA_FILENAME      "EmailAbeyances"
#define TOZC_UNLOCKING_USERS_DATA_FILENAME	"UnlockedUsers"
#define TOZC_USER_ABEYANCES_FILENAME		"UsersAbeyances"
#ifndef TOZC_LOCKED_USERS_DATA_FILENAME
#define TOZC_LOCKED_USERS_DATA_FILENAME		"LockedUsers"
#endif
/*Email*/

#define TOZC_SMTP_CLIENT_DOMAIN			"del.ufrj.br"
#define TOZC_SMTP_SERVER_FULL_HOSTNAME		"smtp.del.ufrj.br"
#define TOZC_SMTP_SERVER_PORT			25
#define TOZC_EMAIL_FROM				"mathlima@poli.ufrj.br"

#define TOZC_EMAIL_BODY				"Corpo da menssagem aqui\n"
#define TOZC_EMAIL_BODY_SIZE			1024
#define TOZC_MAIN_ADMIN_EMAIL			"mathlima@poli.ufrj.br"
#define TOZC_EMAIL_EXPIRATION_TIME		60*60*24*2

#endif /* #ifndef TOZC_CONST_H */ 
