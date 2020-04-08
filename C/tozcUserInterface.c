/*
 * Universidade Federal do Rio de Janeiro
 * Escola Politecnica de Engenharia
 * Departamento de Eletronica e de Computação
 * EEL270 - Computacao II - Turma 2015/2
 * Professor Marcelo Luiz Drumond Lanza
 * Autor: Matheus Silva de Lima
 *
 *  $Author$
 *  $Date$
 *  $Log$
 */

#include "tozcUserInterface.h"


static char* tozcCliUserInterfaceMessages[2][3] = 
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

static char* tozcNcursesUserInterfaceMessages[2][3] = 
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

static char* tozcWebUserInterfaceMessages[2][3] = 
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
TozcGetCliUserInterfaceMessage(tozcUserInterfaceMessageNumberType number, tozcLanguageType language)
{
return tozcCliUserInterfaceMessages[number][language];
}; 

char*
TozcGetNcursesUserInterfaceMessage(tozcUserInterfaceMessageNumberType number, tozcLanguageType language)
{
return tozcNcursesUserInterfaceMessages[number][language];
};

char*
TozcGetWebUserInterfaceMessage(tozcUserInterfaceMessageNumberType number, tozcLanguageType language)
{
return tozcWebUserInterfaceMessages[number][language];
};




/* $RCSFile$ */
