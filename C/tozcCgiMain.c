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
printf("    <TABLE ALIGN=\"center\">");
printf("      <FORM ACTION=\"CGIs/tozcLogin.cgi\" METHOD=\"post\">");
	if (language == tozcPortuguese)
printf("         <INPUT TYPE=\"hidden\" NAME=\"tozcLanguage\" VALUE=\"portuguese\">");
	else
printf("         <INPUT TYPE=\"hidden\" NAME=\"tozcLanguage\" VALUE=\"english\">");

printf("         <TR>");
	if (language == tozcPortuguese)
printf("           <TD ALIGN=\"left\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Usu&aacute;rio:</span></TD>");
	else
printf("           <TD ALIGN=\"left\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">User:</span></TD>");

printf("           <TD><INPUT TYPE=\"text\" NAME=\"tozcNickname\"></TD>");
printf("         </TR>");
printf("         <TR>");
	if (language == tozcPortuguese)
printf("           <TD ALIGN=\"left\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Senha:</span></TD>");
	else
printf("           <TD ALIGN=\"left\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Password:</span></TD>");

printf("           <TD><INPUT TYPE=\"password\" NAME=\"tozcPassword\"></TD>");
printf("         </TR>");
printf("         <TR>");
	if (language == tozcPortuguese)
printf("           <TD COLSPAN=\"2\" ALIGN=\"center\" style=\"font-family:trebuchet ms,helvetica,sans-serif;\"><INPUT TYPE=\"submit\" VALUE=\"Entrar\"></TD>");
	else
printf("           <TD COLSPAN=\"2\" ALIGN=\"center\" style=\"font-family:trebuchet ms,helvetica,sans-serif;\"><INPUT TYPE=\"submit\" VALUE=\"Login\"></TD>");

printf("         </TR>");
printf("         <TR>");
printf("           <TD COLSPAN=\"2\">&nbsp;</TD>");
printf("         </TR>");
printf("         <TR>");
	if (language == tozcPortuguese){
printf("           <TD ALIGN=\"left\"><A HREF=\"./tozcCgiShowRequestPasswordResetForm.cgi?tozcLanguage='portuguese'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Esqueci minha senha</span></A></TD>");
printf("           <TD ALIGN=\"right\"><A HREF=\"./tozcCgiShowRequestRegistrationForm.cgi?tozcLanguage='portuguese'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Pedir Cadastro!</span></A></TD>");
	}else{

printf("           <TD ALIGN=\"left\"><A HREF=\"./tozcCgiShowRequestPasswordResetForm.cgi?tozcLanguage='english'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Lost my password</span></A></TD>");
printf("           <TD ALIGN=\"right\"><A HREF=\"./tozcCgiShowRequestRegistrationForm.cgi?tozcLanguage='english'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Ask for an account!</span></A></TD>");};

printf("         </TR>");
printf("         <TR>");
	if (language == tozcPortuguese){
printf("           <TD ALIGN=\"left\"><A HREF=\"./tozcCgiShowLicence.cgi?tozcLanguage='portuguese'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Licen&ccedil;a de Uso</span></A></TD>");
printf("           <TD ALIGN=\"right\"><A HREF=\"./tozcCgiShowWebHelp.cgi?tozcLanguage='portuguese'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Ajuda</span></A></TD>");
	}else{
printf("           <TD ALIGN=\"left\"><A HREF=\"./tozcCgiShowLicence.cgi?tozcLanguage='english'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">User agreement</span></A></TD>");
printf("           <TD ALIGN=\"right\"><A HREF=\"./tozcCgiShowWebHelp.cgi?tozcLanguage='english'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Need help</span></A></TD>");};
printf("         </TR>");
printf("         <TR>");
printf("           <TD COLSPAN=\"2\">");
	if (language == tozcPortuguese)
printf("<A HREF=\"./tozcCgiMain.cgi?tozcLanguage='english'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">English</span></A></TD>");
	else
printf("<A HREF=\"./tozcCgiMain.cgi?tozcLanguage='portuguese'\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">Portuguese</span></A></TD>");

printf("         </TR>");
printf("      </FORM>");
printf("     </TABLE>");
printf("    <BR>");
printf("     <FONT SIZE=\"-2\">");
printf("        <A HREF=\"MAILTO: mathlima@poli.ufrj.br\"><span style=\"font-family:trebuchet ms,helvetica,sans-serif;\">");
printf("                <div style=\"text-align: center;\">");
printf("                Matheus Silva de Lima");
printf("                </div>");
printf("        </span>");
printf("        </A>");
printf("      </FONT>");
printf("  </BODY>");
printf("</HTML>");


  mlCgiFreeResources ();
  return ML_CGI_OK;
}

/* $RCSfile$ */
