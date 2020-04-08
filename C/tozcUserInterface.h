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

#ifndef TOZC_USER_INTERFACE_H
#define TOZC_USER_INTERFACE_H  "@(#)tozcUserInterface.h $Revision$"

	#include "tozcTypes.h"

typedef enum {cli, ncurses, html} tozcUserInterfaceMessageNumberType;

char *
TozcGetCliUserInterfaceMessage (tozcUserInterfaceMessageNumberType, tozcLanguageType);

char *
TozcGetNcursesUserInterfaceMessage (tozcUserInterfaceMessageNumberType, tozcLanguageType);

char *
TozcGetWebUserInterfaceMessage (tozcUserInterfaceMessageNumberType, tozcLanguageType);


#endif

/* $RCSFile$ */
