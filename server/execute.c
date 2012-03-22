#include <stdio.h>
#include <math.h>
# define OK 0
# define ERROR 1

/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}

void reverse(char s[])
{
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

void * createMsg_s(){

	Msg_s msg = (Msg_s) malloc(sizeof(msg_s));
	if(msg == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}

	msg->msgList = (List) malloc(sizeof(llist));
		if(msg->msgList == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}

	CreateList(msg->msgList);

	return msg;

}

void register(Msg_t msg, Channer ch){

	Msg_s answer = createMsg_s();
	char * user = (msg->data).login_t.user;
	char * pass = (msg->data).login_t.pass;
	char * toPrint;

	int i;
	Element elem;

	FOR_EACH(elem, game->users){

		/* Caso: El usuario ya existe */
		if(strcmp(((User)(elem->data)->user,user) == 0){

			toPrint = malloc(sizeof(strlen("The user already exists") + 1);
			strcpy(toPrint,"The user already exists");
			AddToList(toPrint,answer->msgList);
			answer->status = ERROR;

			comunicate(ch, answer);
			return;

		}
	}
	/* Creo nueva estructura user */
	User newUser = (User) malloc(sizeof(user));
		if(answer == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}	

	/* malloqueo para el nombre del usuario */
	newUser->user = malloc(sizeof(strlen(user)+1));
		if(newUser->user == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}	

	/* malloqueo para la contraseña */
	newUser->pass = malloc(sizeof(strlen(pass)+1));
		if(newUser->pass == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}	

	/* creo lista para los ids de los teams */
	newUser->leaguesIDs = malloc(sizeof(llist));
	if(newUser->leaguesIDs == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}	

	/* inicializo todo */
	strcpy(newUser->user,user);
	strcpy(newUser->pass,pass);
	newUser->leagues = 0;
	CreateList(newUser->leaguesIDs);
		
	toPrint = malloc(sizeof(strlen("Successful registration")+1);
	strcpy(toPrint,"Successful registration");
	AddToList(toPrint,answer->msgList);
	answer->status = OK;

	comunicate(ch,answer);

	return;

}


void executeLogin(Msg_t msg){

 	Msg_s answer = createMsg_s();
	char * user = (msg->data).login_t.user;
	char * pass = (msg->data).login_t.pass;
	char * toPrint;

	int i;
	Element elem;

	/* Me fijo si ya estoy conectado */
	FOR_EACH(elem, game->loggedUsers){

		/* Caso: El usuario ya esta conectado */
		if(strcmp(((char *)(elem->data)->user,user) == 0){

			toPrint = malloc(sizeof(strlen("You are already logged") + 1);
			strcpy(toPrint,"You are already logged");
			AddToList(toPrint,answer->msgList);
			answer->status = ERROR;

			comunicate(ch, answer);
			return;

		}
	}

	/* Me fijo si el usuario existe y lo loggeo*/
	FOR_EACH(elem, game->users){

		/* Caso: El usuario existe */
		if(strcmp(((User)(elem->data)->user,user) == 0 &&
		 			strcmp(((User)(elem->data)->pass,pass) == 0){

			toPrint = malloc(sizeof(strlen("Welcome! You are logged in") + 1);
			strcpy(toPrint,"Welcome! You are logged in");
			AddToList(toPrint,answer->msgList);
			answer->status = OK;

			comunicate(ch, answer);
			return;

		}
	}

	/* Usuario o contraseña incorrectos */

	toPrint = malloc(sizeof(strlen("User or Password incorrects") + 1);
	strcpy(toPrint,"User or Password incorrects");
	AddToList(toPrint,answer->msgList);
	answer->status = ERROR;
	comunicate(ch, answer);
	return;

}

void executeListLeagues(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;

	if(game->leagues->NumEl == 0){
		toPrint = malloc(sizeof(strlen("There is no legaue created") + 1);
		strcpy(toPrint,"There is no legaue created");
		AddToList(toPrint,answer->msgList);
		answer->status = OK;
		comunicate(ch, answer);
		return;
	}

	int dim;
	Element elem;

	FOR_EACH(elem; game->leagues){

		dim = floor(log10(((League)elem->data))->ID);
		toPrint = malloc(dim + 1);
		itoa(((League)elem->data))->ID,toPrint);
		AddToList(toPrint,answer->msgList);

		dim = strlen(((League)(elem->data))->name);
		toPrint = malloc(dim + 1);
		strcpy(toPrint,((League)(elem->data))->name);
		AddToList(toPrint,answer->msgList);

	}

	answer->status = OK;
	comunicate(ch,answer);
	return;

}

void executeListTeams(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;

	if(game->teams->NumEl == 0){
		toPrint = malloc(sizeof(strlen("There is no team created") + 1);
		strcpy(toPrint,"There is no team created");
		AddToList(toPrint,answer->msgList);
		answer->status = OK;
		comunicate(ch, answer);
		return;
	}

	int dim;
	Element elem;
	Element elemTeam;

	FOR_EACH(elem; game->leagues){

		dim = strlen("In League:");
		toPrint = malloc(dim + 1);
		strcpy(toPrint,"In League:");
		AddToList(toPrint,answer->msgList);

		dim = strlen(((League)elem->data)->name);
		toPrint = malloc(dim + 1);
		strcpy(toPrint,((League)elem->data)->name);
		AddToList(toPrint,answer->msgList);

		FOR_EACH(elemTeam; ((League)elem->data)->teams){
			dim = floor(log10((Team)(elemTeam->data)->ID));
			toPrint = malloc(dim + 1);
			itoa((Team)(elemTeam->data)->ID,toPrint);
			AddToList(toPrint,answer->msgList);

			dim = strlen(((Team)elemTeam->data)->owner);
			toPrint = malloc(dim + 1);
			strcpy(toPrint,((Team)elemTeam->data)->owner);
			AddToList(toPrint,answer->msgList);

		}

	}

	answer->status = OK;
	comunicate(ch,answer);
	return;

}

void executeListTrades(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;

	int dim;
	Element elem;
	Element elemTrade;

	FOR_EACH(elem; game->leagues){

		dim = strlen("In League:");
		toPrint = malloc(dim + 1);
		strcpy(toPrint,"In League:");
		AddToList(toPrint,answer->msgList);

		dim = strlen(((League)elem->data)->name);
		toPrint = malloc(dim + 1);
		strcpy(toPrint,((League)elem->data)->name);
		AddToList(toPrint,answer->msgList);

		FOR_EACH(elemTrade; ((Trade)elem->data)->trades){

			if(strcmp(me->user,((Trade)->elemTrade->data)->userFrom) == 0 ||
				strcmp(me->user,((Trade)->elemTrade->data)->userTo) == 0){
				
				dim = floor(log10((Trade)elemTrade->data)->ID);
				toPrint = malloc(dim + 1);
				itoa(((Team)elemTeam->data)->ID,toPrint);
				AddToList(toPrint,answer->msgList);
			}

		}

	}

	answer->status = OK;
	comunicate(ch,answer);
	return;

}
