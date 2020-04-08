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

#include "tozcError.h"


static char* tozcCliErrorMessages[2][150] = 
{
	{
	
"ok",
"argumentoVazio",
"argumentoInvalido",
"tamanhoInvalido",
"caracterInvalido",
"passwordInvalido",
"checkStringFieldArgumentNull",
"checkStringFieldMinGreaterThanMax",
"checkStringFieldInvalidCaracter",
"checkStringFieldInvalidLength",
"checkNicknameArgumentNull",
"checkNicknameInvalidLength",		/*10*/
"checkNicknameInvalidCaracter",
"checkEmailArgumentNull",
"checkEmailInvalidCaracter",
"checkEmailInvalidLength",
"createRandomStringNullArgument",
"createRandomStringInvalidLength",
"createRandomStringInvalidCaracter",
"createNicknameArgumentNull",
"createNicknameInvalidLength",
"getCryptAlgorithmArgumentNull",		/*20*/
"getCryptAlgorithmInvalidCryptPasswordSize",   
"getCryptAlgorithmInvalidArgument",
"encodePasswordWithSpecificAlgorithmArgumentNull",
"encodePasswordWithSpecificSaltArgumentNull",
"encodePasswordWithSpecificSaltInvalidSalt",
"checkPasswordArgumentNull",
"checkPasswordInvalidCryptPasswordSize",
"checkPasswordInvalidCrypt",
"addUserEmailNotCheck",
"addUserNameNull",			/*30*/
"addUserNameCheckNull",
"addUserUsernameNotCheck",
"addUserPasswordsNotCheck",
"addUserPermissionDenied",
"addUserDataWriteOpeningError",
"addUserInvalidUserFile",
"addUserOpeningFileError",
"addUserInvalidAbeyancesFile",
"authenticateUserOpeningFileError",
"authenticateUserInvalidUserFile",
"authenticateUserProfileNotFound",
"authenticateUserUserNotFound",
"tozcProfileTypeNotFound",
"tozcNotAllArgumentsReceived",
"tozcPermissionDenied",
"acceptInviteNoNickname",
"acceptInviteNoPassword",
"acceptInvitePasswordNotCheck",
"acceptInviteOpeningReadFile",
"acceptInviteOpeningWriteFile",
"acceptInviteInvalidUser",
"acceptInviteProfileNotFound",
"acceptInviteInvalidAbeyancesFile",
"acceptInviteTimeExpired",
"acceptInviteUserNotFoundInAbeyances",
"acceptInviteUserNotFound",	
"changeUserProfileOpenReadfile",
"changeUserProfileOpenWritefile",
"changeUserInvalidUsersFile",
"changeUserInvalidUser",
"changeUserUserNotFound",
"changeUserProfileNotFound",
"changeUserInvalidLockFile",
"changeUserCantChange",
"changeUserInvalidRequestFile",
"lockUserAccountOpenReadFile",
"lockUserAccountOpenWriteFile",
"lockUserAccountInvalidUserFile",
"lockUserAccountInvalidLockFile",
"lockUserAccountUserNotFound",
"lockUserAccountProfileNotFound",
"unlockUserAccountUserNotFound",
"unlockUserAccountOpenReadFile",
"unlockUserAccountOpenWriteFile",
"unlockUserAccountInvalidUserFile",
"unlockUserAccountProfileNotFound",
"unlockUserAccountInvalidLockFile",
"unlockUserAccountInvalidUnlockFile",
"getUnlockingRequestsInvalidUnlockFile",
"getUnlockingRequestsOpenReadFile",
"getUnlockingRequestsNoNickname",
"getUnlockingRequestsInvalidUserFile",
"getUnlockingRequestsUserNotFound",
"createSimpleListArgumentNull",
"getUsersOpenReadFile",
"getUsersNoMemory",
"getUsersFileErro",
"approveRegistrationRequesquestInvalidFile",
"approveRegistrationRequesquestOpenFileError",
"approveRegistrationRequesquestReadFile",
"approveRegistrationRequesquestUserNotFound",
"approveRegistrationRequesquesInvalidFile",
"copyFileError",		/*94*/
"copyOpenFileError",
"copyReadFileError",	/**/
"changeUserEmailInvalidFile",
"changeUserEmailUserNotFound",
"changeUserEmailOpenFileError",
"confirmEmailChangeInvalidKey",
"confirmEmailChangeInvalidFile",
"confirmEmailChangeOpenFile",
"confirmEmailChangeReadingFile",
"confirmEmailChangeAccessDenied",
"confirmEmailChangeUserNotFound",
"rejectInviteOpenFile",
"rejectInviteReadFile",
"rejectInviteInvalidFile",
"rejectInviteUserNotFound",
"rejectInviteOldPasswordNotCheck",
"rejectInviteInviteExpired",
	},

	{
	},
};

static char* tozcNcursesErrorMessages[2][3] =
{
	{
	 "",
	 "",
	 ""
	},

	{
	 "",
	 "",
	 ""
	}
};

static char* tozcWebErrorMessages[2][3] =
{
	{
	 "",
	 "",
	 ""
	},

	{
	 "",
	 "",
	 ""
	}
};


char*
TozcGetCliErrorMessage (tozcErrorType error, tozcLanguageType language)
{
return tozcCliErrorMessages[language][error];
};

char*
TozcGetNcursesErrorMessage (tozcErrorType error, tozcLanguageType language)
{
return tozcNcursesErrorMessages[language][error];
};

char*
TozcGetWebErrorMessage (tozcErrorType error, tozcLanguageType language)
{
return tozcWebErrorMessages[language][error];
};

/* $RCSFile$ */
