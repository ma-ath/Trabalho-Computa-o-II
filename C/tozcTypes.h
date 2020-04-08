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
 */
#include "tozcConst.h"

#ifndef TOZC_TYPES_H
#define TOZC_TYPES_H "@(#)tozcTypes.h $Revision$"


typedef enum {tozcEnglish, tozcPortuguese, tozcLanguageQuantifier} tozcLanguageType;


typedef unsigned long long tozcUserIdentifierType;


typedef enum{groupAdmin = 1, groupOwner = 2, groupUser = 4} tozcGroupType;
					/*
					 * 1 - admin
					 * 2 - proprietario
					 * 4 - caroneiro 
					 */ 			

typedef enum{	profileAdmin,
		profileUser,

	    }   tozcProfileType;
					/*
					 * 1 - admin
					 * 2 - proprietario
					 * 3 - admin+proprietario
					 * 4 - caroneiro
					 * 5 - caroneiro+admin
					 * 6 - caroneiro+proprietario
					 * 7 - caroneiro+proprietario+admin
					 */ 		

typedef struct  tozcUserDataType
		{
		  tozcUserIdentifierType uid;
		  char nickname[TOZC_MAX_NICKNAME_SIZE];
		  char password[TOZC_MAX_PASSWORD_SIZE];
		  char passwordCheck[TOZC_MAX_PASSWORD_SIZE];
		  tozcProfileType profileType;
		  char name[TOZC_MAX_USERNAME_SIZE];
		  char nameCheck[TOZC_MAX_USERNAME_SIZE];
		  char email[TOZC_MAX_EMAIL_SIZE];
		  char emailCheck[TOZC_MAX_EMAIL_SIZE];

		  struct tozcUserDataType *next;
		  struct tozcUserDataType *last;

		} tozcUserDataType;

typedef enum {sha256, sha512, md5, des} tozcCryptAlgorithms;

typedef struct tozcNicknameListType
		{
		char nickname[TOZC_MAX_NICKNAME_SIZE];
		struct tozcNicknameListType *next;

		} tozcNicknameListType;
	














#endif /* #ifndef TOZC_TYPES_H */ 
