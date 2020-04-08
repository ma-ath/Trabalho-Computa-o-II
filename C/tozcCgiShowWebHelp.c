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

#include <stdio.h>
#include <stdlib.h>

#include "tozcFunctions.h"
#include "tozcError.h"
#include "tozcConfig.h"
#include "tozcTypes.h"
#include "tozcUserInterface.h"
#include "tozcConst.h"

#include "Libraries/mlcgi.h"


int
main (int argc, char **argv)
{
  environmentType environment;
  char languageString[LANGUAGE_NAME_MAX_CARACTERS]; 
  unsigned returnCode;
  tozcLanguageType language;
/*  tozcErrorType error; */



  if (mlCgiInitialize (&environment))
    exit (ML_CGI_OK);

  if (environment == commandLine)
  {
    printf ("This program may only be executed via web\n");
    exit (ML_CGI_INVALID_REQUEST_METHOD);
  }

  mlCgiBeginHttpHeader ("text/html");
  mlCgiEndHttpHeader ();

  returnCode = 
	mlCgiGetFormStringNoNewLines ("tozcLanguage", languageString, LANGUAGE_NAME_MAX_CARACTERS);
  if (returnCode != ML_CGI_OK)
  {
    printf ("Error getting name - Erro #%i\n", returnCode);
    mlCgiFreeResources ();
    exit (ML_CGI_OK);
  }

    
  language = TozcGetLanguageIndex(languageString);

/*************HTML-PAGE-CODE**************/

printf("<HTML>"); 
printf("  <HEAD>"); 
printf("    <TITLE>380Caronas</TITLE>"); 
printf("  </HEAD>"); 
printf("  <BODY BGCOLOR=\"white\" background=\"img/background.jpg\">"); 
printf("    <DIV ALIGN=\"center\">"); 
printf("      <STRONG><FONT COLOR=\"black\" SIZE=\"+4\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\"><small><span style=\"font-size: 16px;\">"); 
printf("        380Caronas "); 
printf("      </span></small></span></FONT></STRONG><br>"); 
printf("        <div style=\"text-align: center;\">"); 
        if (language == tozcPortuguese) 
printf("                        <var><em><span style=\"font-size:12px;\">Para quem j&aacute; cansou da passagem de &ocirc;nibus</span></em></var></div>"); 
        else 
printf("                        <var><em><span style=\"font-size:12px;\">For those tired of the bus</span></em></var></div>"); 
 
printf("                <div style=\"text-align: center;\">"); 
printf("                        &nbsp;</div>"); 
printf("                <div style=\"text-align: center;\">"); 
printf("                        &nbsp;</div>"); 
printf("    </DIV>"); 
printf("    <br><br><br><br>"); 

	if(language == tozcPortuguese)
	{
	printf ("	<FONT SIZE=\"-1\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">\n");
	printf("                <div style=\"text-align: center;\">"); 
	printf ("	  AJUDA: O SISTEMA AINDA ESTA EM DESENVOLVIMENTO\n");
	printf ("	</div></span></FONT>\n");
	}
	else
	{
	printf ("	<FONT SIZE=\"-1\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">\n");
	printf("                <div style=\"text-align: center;\">"); 
	printf ("	  HELP: SYSTEM IS UNDER DEVELOPMENT\n");
	printf ("	</div></span></FONT>\n");
	}
printf ("    <BR>\n");
printf ("  </BODY>\n");
printf ("</HTML>\n");
  mlCgiFreeResources ();
  return ML_CGI_OK;
}

/* $RCSfile$ */
