#Universidade Federal do Rio de Janeiro
#Escola Politecnica
#Departamento de Eletronica e de Computacao
#Prof. Marcelo Luiz Drumond Lanza
#EEL 270 - Computacao II - Turma 2015/2
#
#$Author$
#$Date$
#$Log$


CC = gcc
LD = gcc

CFLAG = -Wall
LFLAG = -Wall

LIBML = -L./Libraries -lmlcgi
LIBSM = -L./Libraries -lsendmail
LIBCRYPT = -lcrypt

DEV_ROOT = /users/matheus.lima/private/EEL270/2015-2/TOZC
WEB_ROOT = /users/matheus.lima/public/html/TOZC

CLI_EXECS = 	tozc
WEB_EXECS = 	tozcCgiMain\
		tozcCgiShowRequestPasswordResetForm\
		tozcCgiShowRequestRegistrationForm\
		tozcCgiShowWebHelp\
		tozcCgiShowLicence

EXECS = $(CLI_EXECS)\
	$(WEB_EXECS)

TOZCCGIMAINOBJS = tozcCgiMain.o\
		  tozcUserInterface.o\
		  tozcError.o\
		  tozcFunctions.o

TOZCCGISHOWLICENSEOBJS = tozcCgiShowLicence.o\
			tozcUserInterface.o\
			tozcError.o\
			tozcFunctions.o


TOZCCGISHOWREQUESTPASSWORDRESETFORMOBJS = tozcCgiShowRequestPasswordResetForm.o\
		  tozcUserInterface.o\
		  tozcError.o\
		  tozcFunctions.o
		

TOZCCGISHOWREQUESTREGISTRATIONFORMOBJS = tozcCgiShowRequestRegistrationForm.o\
		  tozcUserInterface.o\
		  tozcError.o\
		  tozcFunctions.o


TOZCCGISHOWWEBHELPOBJS = tozcCgiShowWebHelp.o\
		  tozcUserInterface.o\
		  tozcError.o\
		  tozcFunctions.o

TOZCOBJS = 	  tozc.o\
		  tozcUserInterface.o\
		  tozcError.o\
		  tozcFunctions.o\
		  tozcCleanAbeyances.o\
		  tozcAcceptInvite.o\
		  tozcChangeUserProfile.o\
		  tozcLockUserAccount.o\
		  tozcUnlockUserAccount.o\
		  tozcGetUnlockingRequests.o\
		  tozcGetUsers.o\
		  tozcApproveRegistrationRequest.o\
		  tozcChangeUserEmail.o\
		  tozcConfirmEmailChange.o\
		  tozcRejectInvite.o 



#targets:

.c.o:
	$(CC) $(CFLAG) -c $(LIBML) $(LIBSM) $(LIBCRYPT) $<

all: $(EXECS)

install:
	mkdir -p $(WEB_ROOT)/Images $(WEB_ROOT)/CSS $(WEB_ROOT)/Javascript
	mkdir -p $(WEB_ROOT)/CGIs
	mkdir -p $(DEV_ROOT)/Data/Cookies
	mkdir -p $(DEV_ROOT)/Binaries
	cp -f $(DEV_ROOT)/Sources/HTML/index_pt-br.html $(WEB_ROOT)
	cp -f $(DEV_ROOT)/Sources/HTML/index_en-us.html $(WEB_ROOT)
	cp -f $(DEV_ROOT)/Sources/C/tozcCgiMain $(WEB_ROOT)/CGIs/tozcCgiMain.cgi
	cp -f $(DEV_ROOT)/Sources/C/tozcCgiShowRequestPasswordResetForm $(WEB_ROOT)/CGIs/tozcCgiShowRequestPasswordResetForm.cgi
	cp -f $(DEV_ROOT)/Sources/C/tozcCgiShowRequestRegistrationForm $(WEB_ROOT)/CGIs/tozcCgiShowRequestRegistrationForm.cgi
	cp -f $(DEV_ROOT)/Sources/C/tozcCgiShowWebHelp $(WEB_ROOT)/CGIs/tozcCgiShowWebHelp.cgi
	cp -f $(DEV_ROOT)/Sources/C/tozcCgiShowLicence $(WEB_ROOT)/CGIs/tozcCgiShowLicence.cgi
	cp -f $(DEV_ROOT)/Sources/C/tozc $(DEV_ROOT)/Binaries
#	cp -f $(DEV_ROOT)/Sources/HTML/CSS/* $(WEB_ROOT)/CSS/
#	cp -f $(DEV_ROOT)/Sources/HTML/Images/* $(WEB_ROOT)/Images/
#	cp -f $(DEV_ROOT)/Sources/HTML/Javascript/* $(WEB_ROOT)/Javascript/ 
	cp -f $(DEV_ROOT)/Sources/C/tozc $(DEV_ROOT)/Binaries
	ln -s $(WEB_ROOT)/index_en-us.html $(WEB_ROOT)/index.html

tozcCgiMain: $(TOZCCGIMAINOBJS)
	$(LD) $(LFLAGS) -o $@ $(TOZCCGIMAINOBJS) $(LIBML) $(LIBSM) $(LIBCRYPT)

tozcCgiShowRequestPasswordResetForm: $(TOZCCGISHOWREQUESTPASSWORDRESETFORMOBJS)
	$(LD) $(LFLAGS) -o $@ $(TOZCCGISHOWREQUESTPASSWORDRESETFORMOBJS) $(LIBML) $(LIBSM) $(LIBCRYPT)

tozcCgiShowRequestRegistrationForm: $(TOZCCGISHOWREQUESTREGISTRATIONFORMOBJS)
	$(LD) $(LFLAGS) -o $@ $(TOZCCGISHOWREQUESTREGISTRATIONFORMOBJS) $(LIBML) $(LIBSM) $(LIBCRYPT)

tozcCgiShowWebHelp: $(TOZCCGISHOWWEBHELPOBJS)
	$(LD) $(LFLAGS) -o $@ $(TOZCCGISHOWWEBHELPOBJS) $(LIBML) $(LIBSM) $(LIBCRYPT)

tozcCgiShowLicence: $(TOZCCGISHOWLICENSEOBJS)
	$(LD) $(LFLAGS) -o $@ $(TOZCCGISHOWLICENSEOBJS) $(LIBML) $(LIBSM) $(LIBCRYPT)

tozc: $(TOZCOBJS)
	$(LD) $(LFLAGS) -o $@ $(TOZCOBJS) $(LIBML) $(LIBSM) $(LIBCRYPT)

#gcc -Wall -o tozc tozc.c tozcFunctions.c tozcUserInterface.c tozcCleanAbeyances.c tozcError.c ./Libraries/libmlcgi.a ./Libraries/libsendmail.a -lcrypt

deinstall:
	rm -rf $(DEV_ROOT)/Binaries $(DEV_ROOT)/Data $(WEB_ROOT) $(DEV_ROOT)/Sources/C/*.o

clean:
	rm -f *.o $(EXECS)
#$RCSFile$
