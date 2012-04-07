#include "../includes/execute.h"
#include "../includes/toPrint.h"

void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[]){
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

void * createMsg_s(int responseType){

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
	
	msg->responseType = responseType;

	CreateList(msg->msgList);

	return msg;

}

void execute(Msg_t msg, Channel ch, User * me){

	int type = msg->type;
	switch(type){

		case REGISTER:
					executeRegister(msg,ch);
					break;

		case LOGIN:
					executeLogin(msg,ch,me);
					break;			

		case LIST_LEAGUES:
					executeListLeagues(msg,ch);
					break;

		case LIST_TEAMS:
					executeListTeams(msg,ch);
					break;

		case LIST_TRADES:
					executeListTrades(msg,ch,me);
					break;

		case LEAGUE_SHOW: 
					executeLeagueShow(msg,ch);
					break;

		case TEAM_SHOW:
					executeTeamShow(msg,ch);
					break;

		case TRADE_SHOW:
					executeTradeShow(msg,ch);
					break;

		case TRADE:
					executeTrade(msg,ch,me);
					break;

		case TRADE_WITHDRAW:
					executeTradeWithdraw(msg,ch,me);
					break;

		case TRADE_ACCEPT:
					executeTradeAccept(msg,ch,me);
					break;

		case TRADE_NEGOTIATE:
					executeTradeNegotiate(msg,ch,me);
					break;

		case LOGOUT:
					executeLogout(msg,ch,me);
					break;

		case JOIN_LEAGUE:
					executeJoinLeague(msg,ch,me);
					break;

		case CREATE_LEAGUE:
					executeCreateLeague(msg,ch,me);
					break;

		case DRAFT:
					executeDraft(msg,ch,me);
					break;		

		case DRAFT_OUT:
//					executeDraftOut(msg,ch);
					break;		
	}

}

void executeRegister(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s(REGISTER);
	char * user = (msg->data).login_t.user;
	char * pass = (msg->data).login_t.pass;
	char * toPrint;

	int rc;
	Element elem;

	rc = pthread_mutex_lock(&game_mutex);

	FOR_EACH(elem, gameAux->users){

		/* Caso: El usuario ya existe */
		if(strcmp(((User)(elem->data))->user,user) == 0){

			printRedColor(answer);
			toPrint = userExists;
			AddToList(toPrint,answer->msgList);
			releasePrintColor(answer);
			answer->status = ERROR;

			rc = pthread_mutex_unlock(&game_mutex);

			communicate(ch, answer);
			return;

		}
	}
	/* Creo nueva estructura user */
	User newUser = (User) malloc(sizeof(user_t));
		if(newUser == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}	

	/* malloqueo para el nombre del usuario */
	newUser->user = malloc(strlen(user)+1);
		if(newUser->user == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}	

	/* malloqueo para la contraseña */
	newUser->pass = malloc(strlen(pass)+1);
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

	AddToList(newUser,gameAux->users);

	printGreenColor(answer);	
	toPrint = successfulReg;
	AddToList(toPrint,answer->msgList);
	answer->status = OK;
	releasePrintColor(answer);

	communicate(ch,answer);

	rc = pthread_mutex_unlock(&game_mutex);

	return;

}

void executeLogin(Msg_t msg, Channel ch, User * me){

 	Msg_s answer = createMsg_s(LOGIN);
	char * user = (msg->data).login_t.user;
	char * pass = (msg->data).login_t.pass;
	char * toPrint;
	char * usuario;

	int rc;
	Element elem;

	rc = pthread_mutex_lock(&game_mutex);

	if( (*me) == NULL){

		/* Me fijo si ya estoy conectado */
		FOR_EACH(elem, gameAux->loggedUsers){

			/* Caso: El usuario ya esta conectado */
			if(strcmp((char *)elem->data,user) == 0){
				
				printRedColor(answer);
				toPrint = logged;
				AddToList(toPrint,answer->msgList);
				releasePrintColor(answer);
				answer->status = ERROR;

				communicate(ch, answer);

				rc = pthread_mutex_unlock(&game_mutex);

				return;

			}
		}

		/* Me fijo si el usuario existe y lo loggeo*/
		FOR_EACH(elem, gameAux->users){

			/* Caso: El usuario existe */

			if(strcmp(((User)elem->data)->user,user) == 0 &&
		 				strcmp(((User)elem->data)->pass,pass) == 0){

				printGreenBlink(answer);
				asprintf(&toPrint,"\n%s", " ____  __   __ _  ____  __   ____  _  _      ____   __ \n(  __)/ _\\ (  ( \\(_  _)/ _\\ / ___)( \\/ )    / ___) /  \\ \n ) _)/    \\/    /  )( /    \\___ \\  )  /     \\___ \\(  O )\n(__) \\_/\\_/\\_)__) (__)\\_/\\_/(____/(__/      (____/ \\__/");
				AddToList(toPrint,answer->msgList);
				releasePrintColor(answer);
				
				printCyanColor(answer);
				asprintf(&toPrint,"\t\tWelcome, you are logged in!");
				AddToList(toPrint,answer->msgList);							
				releasePrintColor(answer);

				/* Agrego el usuario a la lista de loggeados */
				usuario = malloc(strlen(user) + 1);
				if(usuario == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(usuario,user);
				AddToList(usuario,gameAux->loggedUsers);
				answer->status = OK;

				*me = (User)elem->data;


				rc = pthread_mutex_unlock(&game_mutex);

				communicate(ch, answer);
				return;

			}
		}

		/* Usuario o contraseña incorrectos */

		printRedColor(answer);
		toPrint = incorrectPass;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);
		answer->status = ERROR;

	}else{
		/* Ya hay alguien loggeado */
		printRedColor(answer);
		toPrint = alreadyLogged;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);
	}	
	rc = pthread_mutex_unlock(&game_mutex);

	communicate(ch, answer);
	return;

}

void executeListLeagues(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s(LIST_LEAGUES);
	char * toPrint;
	int rc;

	rc = pthread_mutex_lock(&game_mutex);

	if(gameAux->leagues->NumEl == 0){
		printRedColor(answer);
		toPrint = noLeague;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);
		answer->status = OK;

		rc = pthread_mutex_unlock(&game_mutex);

		communicate(ch, answer);
		return;
	}

	Element elem;

	FOR_EACH(elem, gameAux->leagues){

		printGreenColor(answer);
		asprintf(&toPrint,"\n*** Leagues *** \n");
		releasePrintColor(answer);

		/* ID y Nombre */

		printGreenColor(answer);
		asprintf(&toPrint,"\tLeague #%d: %c[%d;%d;%dm\"%s\" \n", (((League)elem->data)->ID), 0x1B,0,32,40, (((League)elem->data)->name));
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);

	}

	rc = pthread_mutex_unlock(&game_mutex);

	answer->status = OK;
	communicate(ch,answer);
	return;

}

void executeListTeams(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s(LIST_TEAMS);
	char * toPrint;
	int rc;

	rc = pthread_mutex_lock(&game_mutex);

	if(gameAux->cantTeams == 0){
		printRedColor(answer);
		toPrint = noTeam;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);
		answer->status = OK;

		rc = pthread_mutex_unlock(&game_mutex);

		communicate(ch, answer);
		return;
	}

	Element elem;
	Element elemTeam;

	FOR_EACH(elem, gameAux->leagues){
		
		printGreenColor(answer);
		asprintf(&toPrint,"\n*** League \"%s\" *** \n", (((League)elem->data)->name));
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);

		FOR_EACH(elemTeam, ((League)elem->data)->teams){

			asprintf(&toPrint, "\tTeam: %d - Owner: %s\n", ((Team)(elemTeam->data))->ID, ((Team)elemTeam->data)->owner);
			AddToList(toPrint,answer->msgList);

		}

	}

	answer->status = OK;
	communicate(ch,answer);

	rc = pthread_mutex_unlock(&game_mutex);

	return;

}

void executeListTrades(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s(LIST_TRADES);
	char * toPrint;

	int rc;
	Element elem;
	Element elemTrade;

	rc = pthread_mutex_lock(&game_mutex);

	if(*me != NULL){

	FOR_EACH(elem, gameAux->leagues){

		printGreenColor(answer);
		asprintf(&toPrint,"\n*** League \"%s\" *** \n", (((League)elem->data)->name));
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);

		FOR_EACH(elemTrade, ((League)elem->data)->trades){



				if(strcmp((*me)->user,((Trade)elemTrade->data)->userFrom) == 0 ||
					strcmp((*me)->user,((Trade)elemTrade->data)->userTo) == 0){
						switch(((Trade)elemTrade->data)->state){

						case AWAITING:
								asprintf(&toPrint,"\n%c[%d;%dmTrade ID %d: %c[%d;%dm%s (%s) ---> %s (%s)\n\t\t%c[%d;%dm[AWAITING]", 0x1B,1,36, ((Trade)elemTrade->data)->ID, 0x1B,0,36, ((Trade)elemTrade->data)->playerFrom, ((Trade)elemTrade->data)->userFrom, ((Trade)elemTrade->data)->userTo, ((Trade)elemTrade->data)->playerTo, 0x1B,5,33);
								AddToList(toPrint,answer->msgList);
								releasePrintColor(answer);
								break;

						case CANCELED:
								asprintf(&toPrint,"\n%c[%d;%dmTrade ID %d: %c[%d;%dm%s (%s) ---> %s (%s)\n\t\t%c[%d;%dm[CANCELED]", 0x1B,1,36, ((Trade)elemTrade->data)->ID, 0x1B,0,36, ((Trade)elemTrade->data)->playerFrom, ((Trade)elemTrade->data)->userFrom, ((Trade)elemTrade->data)->userTo, ((Trade)elemTrade->data)->playerTo, 0x1B,5,31);
								AddToList(toPrint,answer->msgList);
								releasePrintColor(answer);
								break;
						case ACCEPTED:

								asprintf(&toPrint,"\n%c[%d;%dmTrade ID %d: %c[%d;%dm%s (%s) ---> %s (%s)\n\t\t%c[%d;%dm[ACCEPTED]", 0x1B,1,36, ((Trade)elemTrade->data)->ID, 0x1B,0,36, ((Trade)elemTrade->data)->playerFrom, ((Trade)elemTrade->data)->userFrom, ((Trade)elemTrade->data)->userTo, ((Trade)elemTrade->data)->playerTo, 0x1B,5,32);
								AddToList(toPrint,answer->msgList);
								releasePrintColor(answer);
								break;
					}
				}
		}
	}

	if(answer->msgList->NumEl == 0){
		printRedColor(answer);
		toPrint = noTrade;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);
	}
	
	}else{
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);
		answer->status = ERROR;
	}

	rc = pthread_mutex_unlock(&game_mutex);
	answer->status = OK;
	communicate(ch,answer);
	return;

}

void executeLeagueShow(Msg_t msg, Channel ch){



	Msg_s answer = createMsg_s(LEAGUE_SHOW);
	char * toPrint;
	int input = msg->data.show_t.ID;

	int rc;
	Element elem;
	Element elemItem;

	rc = pthread_mutex_lock(&game_mutex);

	FOR_EACH(elem, gameAux->leagues){

		if(((League)(elem->data))->ID == input){

			/* Imprimo el nombre de la league */
			printGreenColor(answer);
			asprintf(&toPrint,"\n*** League \"%s\" *** \n", (((League)elem->data)->name));

			AddToList(toPrint,answer->msgList);
			releasePrintColor(answer);

			/* Jugadores sin draftear */
			printYellowColor(answer);		
			asprintf(&toPrint,"\n** Available Players **");
			AddToList(toPrint,answer->msgList);
			releasePrintColor(answer);

			FOR_EACH(elemItem, ((League)elem->data)->availablePlayers){

				/* Nombre jugador y puntaje */
				
				asprintf(&toPrint,"\n\t%s\t(%d points)", (((Player)elemItem->data)->name), ((Player)elemItem->data)->points);	
				AddToList(toPrint,answer->msgList);

			}

			printYellowColor(answer);
			asprintf(&toPrint,"\n** Teams **\n");
			AddToList(toPrint,answer->msgList);
			releasePrintColor(answer);				

			FOR_EACH(elemItem, ((League)elem->data)->teams){

				asprintf(&toPrint, "\tTeam: %d - Owner: %s - Points: %d\n", ((Team)(elemItem->data))->ID, ((Team)elemItem->data)->owner, ((Team)elemItem->data)->points);
				AddToList(toPrint,answer->msgList);

			}

			rc = pthread_mutex_unlock(&game_mutex);

			answer->status = OK;
			communicate(ch,answer);
			return;

		}

	}

	printRedColor(answer);
	toPrint = incorrectID;
	AddToList(toPrint,answer->msgList);
	releasePrintColor(answer);
	answer->status = OK;

	rc = pthread_mutex_unlock(&game_mutex);

	communicate(ch,answer);
	return;

}

void executeTeamShow(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s(TEAM_SHOW);
	char * toPrint;
	int input = msg->data.show_t.ID;

	int rc;
	Element elemLeague;
	Element elemTeam;
	Element elemPlayer;

	rc = pthread_mutex_lock(&game_mutex);

	FOR_EACH(elemLeague, gameAux->leagues){

		FOR_EACH(elemTeam, ((League)elemLeague->data)->teams){

			if(((Team)elemTeam->data)->ID == input){

				printGreenColor(answer);
				asprintf(&toPrint,"\n*** League \"%s\" *** \n", (((League)elemLeague->data)->name));				
				AddToList(toPrint,answer->msgList);
				releasePrintColor(answer);	

				printYellowColor(answer);
				asprintf(&toPrint, "\tTeam: %d - Owner: %s - Points: %d\n", ((Team)(elemTeam->data))->ID, ((Team)elemTeam->data)->owner, ((Team)elemTeam->data)->points);
				AddToList(toPrint,answer->msgList);
				releasePrintColor(answer);

				asprintf(&toPrint, "\tPlayers:");
				AddToList(toPrint,answer->msgList);				

				/* Player */
				FOR_EACH(elemPlayer, ((Team)elemTeam->data)->players){

					asprintf(&toPrint,"\n\t\t%s\t(%d points)", ((Player)elemPlayer->data)->name, ((Player)elemPlayer->data)->points);	
					AddToList(toPrint,answer->msgList);

				}

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = ERROR;
				communicate(ch,answer);
				return;
			} 
		}
	}

	printRedColor(answer);
	toPrint = incorrectID;
	AddToList(toPrint,answer->msgList);
	releasePrintColor(answer);

	rc = pthread_mutex_unlock(&game_mutex);

	answer->status = ERROR;
	communicate(ch,answer);
	return;
}

void executeTradeShow(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s(TRADE_SHOW);
	char * toPrint;
	int input = msg->data.show_t.ID;

	int rc;
	Element elemLeague;
	Element elemTrade;

	rc = pthread_mutex_lock(&game_mutex);

	if(input > gameAux->cantTrades){

		printRedColor(answer);
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);

		rc = pthread_mutex_unlock(&game_mutex);

		answer->status = ERROR;
		communicate(ch,answer);
		return;

	}

	FOR_EACH(elemLeague, gameAux->leagues){

		FOR_EACH(elemTrade, ((League)elemLeague->data)->trades){

			if(((Trade)elemTrade->data)->ID == input){
				switch(((Trade)elemTrade->data)->state){

					case AWAITING:
							asprintf(&toPrint,"\n%c[%d;%dmTrade ID %d: %c[%d;%dm%s (%s) ---> %s (%s)\n\t\t%c[%d;%dm[AWAITING]", 0x1B,1,36, ((Trade)elemTrade->data)->ID, 0x1B,0,36, ((Trade)elemTrade->data)->playerFrom, ((Trade)elemTrade->data)->userFrom, ((Trade)elemTrade->data)->userTo, ((Trade)elemTrade->data)->playerTo, 0x1B,5,33);
							AddToList(toPrint,answer->msgList);
							releasePrintColor(answer);
							break;

					case CANCELED:
							asprintf(&toPrint,"\n%c[%d;%dmTrade ID %d: %c[%d;%dm%s (%s) ---> %s (%s)\n\t\t%c[%d;%dm[CANCELED]", 0x1B,1,36, ((Trade)elemTrade->data)->ID, 0x1B,0,36, ((Trade)elemTrade->data)->playerFrom, ((Trade)elemTrade->data)->userFrom, ((Trade)elemTrade->data)->userTo, ((Trade)elemTrade->data)->playerTo, 0x1B,5,31);
							AddToList(toPrint,answer->msgList);
							releasePrintColor(answer);
							break;
					case ACCEPTED:

							asprintf(&toPrint,"\n%c[%d;%dmTrade ID %d: %c[%d;%dm%s (%s) ---> %s (%s)\n\t\t%c[%d;%dm[ACCEPTED]", 0x1B,1,36, ((Trade)elemTrade->data)->ID, 0x1B,0,36, ((Trade)elemTrade->data)->playerFrom, ((Trade)elemTrade->data)->userFrom, ((Trade)elemTrade->data)->userTo, ((Trade)elemTrade->data)->playerTo, 0x1B,5,32);
							AddToList(toPrint,answer->msgList);
							releasePrintColor(answer);
							break;
				}

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = OK;
				communicate(ch,answer);
				return;
			}
		}
	}
}

void executeTrade(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s(TRADE);
	char * toPrint;

	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);
		communicate(ch,answer);
		return;
	}

	int teamID = msg->data.trade_t.teamID;
	char * from = (msg->data).trade_t.from;
	char * to = (msg->data).trade_t.to;
	int rc;
	int flag = 0;
	Element elemLeague,elemTeam,elemPlayer, elemID, myTeam;
	Trade t = (Trade)malloc(sizeof(trade_t));
	if(t == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}

	rc = pthread_mutex_lock(&game_mutex);

	/* Si el ID es incorrecto */
	if(teamID > gameAux->cantTeams){
		printRedColor(answer);		
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);
		AddToList(toPrint,answer->msgList);

		rc = pthread_mutex_unlock(&game_mutex);

		answer->status = ERROR;
		communicate(ch,answer);
		return;
	}

	FOR_EACH(elemLeague, gameAux->leagues){
		FOR_EACH(elemTeam, ((League)elemLeague->data)->teams){

			/* Busco el team con el ID ingresado */
			if(((Team)elemTeam->data)->ID == teamID){

				/* Me fijo si la liga esta activa */
				if(((League)elemLeague->data)->status == INACTIVE){
					printRedColor(answer);
					toPrint = inactiveLeague;
					AddToList(toPrint,answer->msgList);
					releasePrintColor(answer);

					rc = pthread_mutex_unlock(&game_mutex);

					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}

				/* Me fijo si el usuario que ejecuta el comando pertenece a la liga */
				FOR_EACH(elemID, (*me)->leaguesIDs){
					if((*((int*)elemID->data)) == ((League)elemLeague->data)->ID){
						flag = 1;
						break;
					}
				}
				if(flag == 0){
					printRedColor(answer);
					toPrint = noTeamInLeague;
					AddToList(toPrint,answer->msgList);
					releasePrintColor(answer);
					rc = pthread_mutex_unlock(&game_mutex);
					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}

				/* Si a el jugador que quiero adquirir lo tiene el otro equipo */
				flag = 0;
				FOR_EACH(elemPlayer, ((Team)elemTeam->data)->players){

					if(strcmp(((Player)elemPlayer->data)->name,to) == 0){
						flag = 1;
						break;
					}
				}
				if(flag == 0){
					printRedColor(answer);
					toPrint = noPlayerTo;
					AddToList(toPrint,answer->msgList);
					releasePrintColor(answer);

					rc = pthread_mutex_unlock(&game_mutex);

					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}

				/* Me fijo si el usuario tiene el jugador que quiere cambiar */
				flag = 0;
				FOR_EACH(myTeam, ((League)elemLeague->data)->teams){
					if(strcmp(((Team)myTeam->data)->owner, (*me)->user) == 0){
						FOR_EACH(elemPlayer, ((Team)myTeam->data)->players){
							if(strcmp(((Player)elemPlayer->data)->name, from) == 0){
								flag = 1;
								break;
							}
						}
						break;
					}
				}
				if(flag == 0){
					printRedColor(answer);
					toPrint = noPlayerFrom;
					AddToList(toPrint,answer->msgList);
					releasePrintColor(answer);

					rc = pthread_mutex_unlock(&game_mutex);

					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}

				/* CREO EL TRADE */
				t->ID = ++(gameAux->cantTrades);
				t->state = AWAITING;
				t->playerFrom = (char*) malloc(strlen(from)+1);
				if(t->playerFrom == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(t->playerFrom,from);


				t->playerTo = (char*) malloc(strlen(to)+1);
				if(t->playerTo == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(t->playerTo,to);

				t->userFrom = (char*) malloc(strlen(((*me)->user)+1));
				if(t->userFrom == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(t->userFrom, (*me)->user);

				t->userTo = (char*) malloc(strlen(((Team)elemTeam->data)->owner)+1);
				if(t->userTo == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(t->userTo,((Team)elemTeam->data)->owner);

				AddToList(t,((League)elemLeague->data)->trades);


				printGreenColor(answer);
				toPrint = successfulTrade;
				AddToList(toPrint,answer->msgList);
				releasePrintColor(answer);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = OK;
				communicate(ch,answer);
				return;

			}
		}		
	}
}

void executeTradeWithdraw(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s(TRADE_WITHDRAW);
	char * toPrint;
	int input = msg->data.trade_t.tradeID;

	int rc;
	Element elemLeague;
	Element elemTrade;

	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);
		communicate(ch,answer);
		return;
	}

	rc = pthread_mutex_lock(&game_mutex);


	if(input > gameAux->cantTrades){

		printRedColor(answer);
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);

		rc = pthread_mutex_unlock(&game_mutex);

		answer->status = ERROR;
		communicate(ch,answer);
		return;

	}else{

		FOR_EACH(elemLeague, gameAux->leagues){

			FOR_EACH(elemTrade, ((League)elemLeague->data)->trades){
				/* Encuentro el trade */
				if(((Trade)elemTrade->data)->ID == input){
					/* Valido que sea el to o el from del trade */
					if(strcmp(((Trade)elemTrade->data)->userTo, (*me)->user) != 0 
						&& strcmp(((Trade)elemTrade->data)->userFrom, (*me)->user) != 0){

						printRedColor(answer);
						toPrint = isNotYourTrade;
						AddToList(toPrint,answer->msgList);
						answer->status = ERROR;
						releasePrintColor(answer);

						rc = pthread_mutex_unlock(&game_mutex);

						answer->status = ERROR;
						communicate(ch,answer);
						return;
					}else{ /* Es mi trade */

						if(((Trade)elemTrade->data)->state != AWAITING){
						
							printRedColor(answer);
							toPrint = endedTrade;
							AddToList(toPrint,answer->msgList);
							answer->status = ERROR;
							releasePrintColor(answer);
							rc = pthread_mutex_unlock(&game_mutex);

							answer->status = ERROR;
							communicate(ch,answer);
							return;

						}else{
							((Trade)elemTrade->data)->state = CANCELED;
							printGreenColor(answer);
							toPrint = tradeCanceled;
							AddToList(toPrint,answer->msgList);
							releasePrintColor(answer);
							rc = pthread_mutex_unlock(&game_mutex);

							answer->status = OK;
							communicate(ch,answer);
							return;
						}		
					}					
				}
			}
		}
	}
}

void executeTradeAccept(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s(TRADE_ACCEPT);
	char * toPrint;
	int input = msg->data.trade_t.tradeID;
	int rc;
	Element elemLeague;
	Element elemTrade;
	Element elemTeamF;
	Element elemTeamT;
	Element elemPlayerF;
	Element elemPlayerT;
	Element playerAux;
	int flagF = 0;
	int flagT = 0;


	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);
		communicate(ch,answer);
		return;
	}

	rc = pthread_mutex_lock(&game_mutex);


	if(input > gameAux->cantTrades){

		printRedColor(answer);
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);

		rc = pthread_mutex_unlock(&game_mutex);

		communicate(ch,answer);
		return;
	}

	FOR_EACH(elemLeague, gameAux->leagues){
		FOR_EACH(elemTrade, ((League)elemLeague->data)->trades){

			/* Encuentro el trade con ID ingresado */
			if(((Trade)elemTrade->data)->ID == input){

				/* Me fijo si es trade mio */
				if(strcmp(((Trade)elemTrade->data)->userTo, (*me)->user) != 0){

					printRedColor(answer);
					toPrint = isNotYourTrade;
					AddToList(toPrint,answer->msgList);
					answer->status = ERROR;
					releasePrintColor(answer);

					rc = pthread_mutex_unlock(&game_mutex);

					communicate(ch,answer);
					return;
				}

				/* Me fijo si el trade esta en espera de respuesta */
				if(((Trade)elemTrade->data)->state != AWAITING){
					printRedColor(answer);
					toPrint = endedTrade;
					AddToList(toPrint,answer->msgList);
					answer->status = ERROR;
					releasePrintColor(answer);

					rc = pthread_mutex_unlock(&game_mutex);

					communicate(ch,answer);
					return;

				}

				/* Itero para buscar el equipo que ofrece el trade */
				FOR_EACH(elemTeamF, ((League)elemLeague->data)->teams){

					/*  Encuentro el team del user from */
					if(strcmp(((Team)elemTeamF->data)->owner, ((Trade)elemTrade->data)->userFrom) == 0){

						/* Me fijo que el que ofrecio el trade siga teniendo el jugador */
						FOR_EACH(elemPlayerF, ((Team)elemTeamF->data)->players){
							if(strcmp(((Player)elemPlayerF->data)->name,((Trade)elemTrade->data)->playerFrom) == 0){
								flagF = 1;
								break;
							}					
						}						
						if(flagF == 0){

							printRedColor(answer);
							toPrint = unavailablePlayerF;
							AddToList(toPrint,answer->msgList);
							answer->status = ERROR;
							releasePrintColor(answer);

							rc = pthread_mutex_unlock(&game_mutex);

							communicate(ch,answer);
							return;													
						}

						/* Itero para buscar mi equipo */

						FOR_EACH(elemTeamT, ((League)elemLeague->data)->teams){

							/* Encuentro mi team */
							if(strcmp(((Team)elemTeamT->data)->owner, ((Trade)elemTrade->data)->userTo) == 0){

								/* Me fijo si sigo teniendo el player */
								FOR_EACH(elemPlayerT, ((Team)elemTeamT->data)->players){
									if(strcmp(((Player)elemPlayerT->data)->name,((Trade)elemTrade->data)->playerTo) == 0){
										flagT = 1;
										break;
									}					
								}

								if(flagT == 0){

									printRedColor(answer);
									toPrint = unavailablePlayerT;
									AddToList(toPrint,answer->msgList);
									answer->status = ERROR;
									releasePrintColor(answer);

									rc = pthread_mutex_unlock(&game_mutex);

									communicate(ch,answer);
									return;													
								}

								/* En esta instancia, los dos tenemos los jugadores */

								/* Saco a playerFrom de userFrom y lo agrego a userTo*/

								playerAux = elemPlayerF;
								Remove(elemPlayerF, ((Team)elemTeamF->data)->players);
								AddToList((Player)playerAux->data, ((Team)elemTeamT->data)->players);

								/* Saco a playerTo de userTo y lo agrego a userFrom*/
								playerAux = elemPlayerT;
								Remove(elemPlayerT, ((Team)elemTeamT->data)->players);
								AddToList((Player)playerAux->data, ((Team)elemTeamF->data)->players);

								((Trade)elemTrade->data)->state = ACCEPTED;


								printGreenColor(answer);
								toPrint = tradeAccepted;
								AddToList(toPrint,answer->msgList);
								releasePrintColor(answer);

								rc = pthread_mutex_unlock(&game_mutex);

								answer->status = OK;
								communicate(ch,answer);
								return;

							}
						}						
					}
				}
			}
		}
	}	
}

void executeTradeNegotiate(Msg_t msg, Channel ch, User * me){
	
	Msg_s answer = createMsg_s(TRADE_NEGOTIATE);
	char * toPrint;
	int input = msg->data.trade_t.tradeID;

	int rc;
	Element elemLeague;
	Element elemTrade;
	Element elemTeam;

	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);
		communicate(ch,answer);
		return;
	}

	rc = pthread_mutex_lock(&game_mutex);


	if(input > gameAux->cantTrades){

		printRedColor(answer);
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);

		rc = pthread_mutex_unlock(&game_mutex);

		answer->status = ERROR;
		communicate(ch,answer);
		return;

	}

	FOR_EACH(elemLeague, gameAux->leagues){

		FOR_EACH(elemTrade, ((League)elemLeague->data)->trades){
			/* Encuentro el trade */
			if(((Trade)elemTrade->data)->ID == input){
				/* Valido que sea el to del trade */
				if(strcmp(((Trade)elemTrade->data)->userTo, (*me)->user) != 0){

					printRedColor(answer);
					toPrint = isNotYourTrade;
					AddToList(toPrint,answer->msgList);
					answer->status = ERROR;
					releasePrintColor(answer);

					rc = pthread_mutex_unlock(&game_mutex);

					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}else{ /* Es mi trade */

					if(((Trade)elemTrade->data)->state != AWAITING){
						
						printRedColor(answer);
						toPrint = endedTrade;
						AddToList(toPrint,answer->msgList);
						answer->status = ERROR;
						releasePrintColor(answer);
						rc = pthread_mutex_unlock(&game_mutex);

						answer->status = ERROR;
						communicate(ch,answer);
						return;
					}else{
						
						/* LO CANCELO */
						
						((Trade)elemTrade->data)->state = CANCELED;
						printGreenColor(answer);
						toPrint = tradeCanceled;
						AddToList(toPrint,answer->msgList);
						answer->status = OK;
						releasePrintColor(answer);
					
						/* Ya esta cancelado el trade AHORA CREO UN TRADE NUEVO */

						FOR_EACH(elemTeam, ((League)elemLeague->data)->teams){

							if(strcmp(((Team)elemTeam->data)->owner, ((Trade)elemTrade->data)->userFrom) == 0){

								msg->data.trade_t.teamID = ((Team)elemTeam->data)->ID;

								rc = pthread_mutex_unlock(&game_mutex);

								executeTrade(msg,ch,me);
								return;
							}
						}						
					}		
				}					
			}
		}
	}
	

	
}


void executeLogout(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s(LOGOUT);
	char * toPrint;
	int rc;
	Element elem;

	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);
		communicate(ch,answer);
		return;

	}else{

		rc = pthread_mutex_lock(&game_mutex);

		/* Lo saco de la lista de loggeados */
		FOR_EACH(elem, gameAux->loggedUsers){

			if(strcmp((char*)elem->data, (*me)->user) == 0){
				break;
			}
		}		
		Remove(elem, gameAux->loggedUsers);

		/* Apunto me a NULL */
		(*me) = NULL;

		printGreenColor(answer);
		toPrint = loggedOut;
		AddToList(toPrint,answer->msgList);
		releasePrintColor(answer);

		answer->status = OK;
		communicate(ch,answer);

		rc = pthread_mutex_unlock(&game_mutex);

		return;
	}

}

void executeJoinLeague(Msg_t msg, Channel ch, User * me){
	
	Msg_s answer = createMsg_s(JOIN_LEAGUE);
	char * toPrint;
	int input = msg->data.ID;
	int rc;
	Element league, elemTeam;
	Team team;
	
	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);
		communicate(ch,answer);
		return;
	}

	rc = pthread_mutex_lock(&game_mutex);
	

	if(input > gameAux->leagues->NumEl){

		printRedColor(answer);
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);

		rc = pthread_mutex_unlock(&game_mutex);

		communicate(ch,answer);
		return;
	}
	
	FOR_EACH(league, gameAux->leagues){
		
		/* Encontre la liga */
		if(((League)league->data)->ID == input){
			
			/* si ya esta activa */
			if(((League)league->data)->status == ACTIVE){
				printRedColor(answer);
				toPrint = alreadyActive;
				AddToList(toPrint,answer->msgList);
				answer->status = ERROR;
				releasePrintColor(answer);

				rc = pthread_mutex_unlock(&game_mutex);

				communicate(ch,answer);
				return;
			}
			
			/* si esta llena */
			if(((League)league->data)->teams->NumEl == CANT_TEAMS){
				
				printRedColor(answer);
				toPrint = leagueFull;
				AddToList(toPrint,answer->msgList);
				answer->status = ERROR;
				releasePrintColor(answer);

				rc = pthread_mutex_unlock(&game_mutex);

				communicate(ch,answer);
				return;
			}
			
			/* Si ya estoy en la liga */
			FOR_EACH(elemTeam, ((League)league->data)->teams){
				if(strcmp(((Team)elemTeam->data)->owner, (*me)->user) == 0){
					printRedColor(answer);
					toPrint = alreadyBelongToLeague;
					AddToList(toPrint,answer->msgList);
					answer->status = ERROR;
					releasePrintColor(answer);

					rc = pthread_mutex_unlock(&game_mutex);

					communicate(ch,answer);
					return;
				}
			}
			
			team = (Team)malloc(sizeof(team_t));
			if(team == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}
			
			(gameAux->cantTeams)++;
			
			team->ID = gameAux->cantTeams;
			team->owner = (*me)->user;
			team->points = 0;
			team->cantPlayers = 0;
			team->players = (List) malloc(sizeof(llist));
			if(team->players == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}
			
			CreateList(team->players);
			
			AddToList(team, ((League)league->data)->teams);
			
			/* Modifico en mi user los ids de mis ligas y la cant */
			int * id = malloc(sizeof(int));
			(*id) = input;
			AddToList(id, (*me)->leaguesIDs);
			((*me)->leagues)++;

			
			printGreenColor(answer);
			toPrint = successfulJoin;
			AddToList(toPrint,answer->msgList);
			releasePrintColor(answer);

			rc = pthread_mutex_unlock(&game_mutex);

			answer->status = OK;
			communicate(ch,answer);
			return;
			
		}
	}
}

void executeCreateLeague(Msg_t msg, Channel ch, User * me){
	
	Msg_s answer = createMsg_s(CREATE_LEAGUE);
	char * toPrint;
	int name = msg->data.name;
	Player player;
	Element elemPlayer;
	
	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);
		communicate(ch,answer);
		return;
	}
	
	List players = loadPlayers();
	
	/* malloqueo la liga */
	League league = (League)malloc(sizeof(league_t));
	if(league == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	league->status = INACTIVE;
	league->availablePlayers = 0;
	league->cantDraft = 0;
	league->answer = FALSE;
	
	/* malloqueo para el nombre y lo inicializo */
	league->name = malloc(strlen(name)+1);
		if(league->name == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);	
	}
	strcpy(league->name,name);
	
	/* malloqueo la lista de jugadores disponibles */
	league->availablePlayers = (List) malloc(sizeof(llist));
	if(league->availablePlayers == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	CreateList(league->availablePlayers);
	
	/* malloqueo la lista de teams */
	league->teams = (List) malloc(sizeof(llist));
	if(league->teams == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	CreateList(league->teams);
	
	/* malloqueo la lista de trades */
	league->trades = (List) malloc(sizeof(llist));
	if(league->trades == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	CreateList(league->trades);
	
	int rc;

	rc = pthread_mutex_lock(&game_mutex);
	
	league->ID = (gameAux->leagues->NumEl) + 1;
	
	FOR_EACH(elemPlayer, players){
		
		player = (Player)malloc(sizeof(player_t));
		if(player == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		player->name = (char*)elemPlayer->data;
		player->points = 0;
		
		AddToList(player, league->availablePlayers);
				
	}
	
	
	AddToList(league, gameAux->leagues);
	
	printGreenColor(answer);
	toPrint = successfulCreation;
	AddToList(toPrint,answer->msgList);
	releasePrintColor(answer);

	answer->status = OK;
	communicate(ch,answer);

	rc = pthread_mutex_unlock(&game_mutex);

	return;
	
}

void executeDraft(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s(DRAFT);
	char * toPrint;
	int input = msg->data.ID;
	int rc;
	Element elemLeague, elemID;
	int flag;

	rc = pthread_mutex_lock(&game_mutex);

	if(*me == NULL)
	{
		printRedColor(answer);
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);

		rc = pthread_mutex_unlock(&game_mutex);
		communicate(ch, answer);
		return;
	}

	/* Liga no existente */
	if(input > gameAux->leagues->NumEl){
		printRedColor(answer);
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
		releasePrintColor(answer);

		rc = pthread_mutex_unlock(&game_mutex);

		communicate(ch,answer);
		return;
	}

	FOR_EACH(elemLeague, gameAux->leagues){

		/* En elemLeague cargo la liga que corresponde */
		if(((League)elemLeague->data)->ID == input){


			/* Caso que yo no pertenezca a la league */
			flag = 0;
			FOR_EACH(elemID, (*me)->leaguesIDs){
				if(*((int*)elemID->data) == ((League)elemLeague->data)->ID){
					flag = 1;
					break;
				}
			}
			if(flag == 0){
				printRedColor(answer);
				toPrint = noTeamInLeague;
				AddToList(toPrint,answer->msgList);
				answer->status = ERROR;
				releasePrintColor(answer);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = ERROR;
				communicate(ch,answer);
				return;
			}


			/* Caso de liga que ya se drafteo */
			if(((League)elemLeague->data)->status == ACTIVE){
				printRedColor(answer);
				toPrint = alreadyDrafted;
				AddToList(toPrint,answer->msgList);
				answer->status = ERROR;
				releasePrintColor(answer);

				rc = pthread_mutex_unlock(&game_mutex);

				communicate(ch,answer);
				return;
			}

			/* No hay teams suficientes para draftear */
			if(((League)elemLeague->data)->teams->NumEl < CANT_TEAMS){
				printRedColor(answer);
				toPrint = notEnoughTeams;
				AddToList(toPrint,answer->msgList);
				answer->status = ERROR;
				releasePrintColor(answer);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = ERROR;
				communicate(ch,answer);
				return;
			}

			if(((League)elemLeague->data)->status == INACTIVE){

				((League)elemLeague->data)->cantDraft++;

				printGreenColor(answer);
				AddToList(draftInSuccessful, answer->msgList);
				answer->status = OK;
				releasePrintColor(answer);
				communicate(ch, answer);


				if(((League)elemLeague->data)->cantDraft == CANT_TEAMS){

					/* Creo el thread coordinador */
					pthread_t coordinatorThread;
					int iRet;
					iRet = pthread_create(&coordinatorThread, NULL, coordinator_thread, (void*)((League)elemLeague->data));
					if (iRet){
						printf("ERROR; return code from pthread_create() is %d\n", iRet);
						exit(EXIT_FAILURE);
					}
					answer = createMsg_s(DRAFT_START);
					answer->status = OK;
					AddToList(draftStarting, answer->msgList);
					communicate(ch, answer);
					((League)elemLeague->data)->status = DRAFTING;
					rc = pthread_mutex_unlock(&game_mutex);
					makeDraft((League)elemLeague->data, ch, me);

				}else{
					//esperar mensaje logout o draft out
					while(((League)elemLeague->data)->cantDraft < CANT_TEAMS)
					{
						rc = pthread_mutex_unlock(&game_mutex);
						Msg_t incoming = rcvmessage(ch);
						if(incoming != NULL)
						{
							answer = createMsg_s(incoming->type);
							if(incoming->type != DRAFT_OUT || incoming->type != LOGOUT)
							{
								answer->status = !OK;
								AddElemToList(invalidCommand, answer->msgList);
								communicate(ch, answer);
							}
							else
							{
								rc = pthread_mutex_lock(&game_mutex);
								((League)elemLeague->data)->cantDraft--;
								rc = pthread_mutex_unlock(&game_mutex);
								answer->status = OK;
								AddElemToList(draftOutSuccessful, answer->msgList);
								communicate(ch, answer);
								return;
							}
						}

						sleep(1);
						rc = pthread_mutex_lock(&game_mutex);
					}
					answer = createMsg_s(DRAFT_START);
					answer->status = OK;
					AddToList(draftStarting, answer->msgList);
					communicate(ch, answer);
					makeDraft((League)elemLeague->data, ch, me);
					return;

				}
			}else if(((League)elemLeague->data)->status == DRAFTING){
				rc = pthread_mutex_unlock(&game_mutex);
				makeDraft((League)elemLeague->data, ch, me);
				return;
			}

		}//IF ES EL ID DE LA LEAGUE
	}//FOR_EACH
}

void makeDraft(League league,Channel ch, User * me)
{

	Msg_t fromClient;
	char * player;
	Element elemPlayer,elemTeam;
	Msg_s toClient;
	int turnFlag = FALSE;
	int rc = pthread_mutex_lock(&game_mutex);
	while(league->status == DRAFTING)
	{
		if(turnFlag == FALSE && strcmp(league->turn, (*me)->user) == 0 )
		{
			toClient = createMsg_s(DRAFT_TURN);
			toClient->status = OK;
			AddToList(isYourTurn, toClient->msgList);
			communicate(ch, toClient);
			turnFlag = TRUE;
		}
		rc = pthread_mutex_unlock(&game_mutex);

		fromClient = rcvmessage(ch);

		if(fromClient != NULL)
		{
			if(fromClient->type == DRAFT_OUT)
			{
				//league->cantDraft--;
				toClient = createMsg_s(DRAFT_OUT);
				AddToList(draftOutSuccessful, toClient->msgList);
				communicate(ch, toClient);
				return;
			}
			if(fromClient->type == LOGOUT)
			{
				executeLogout(fromClient,ch,me);
				return;
			}

			/* Mi turno*/
			if(fromClient->type == CHOOSE)
			{

				int rc = pthread_mutex_lock(&game_mutex);
				if(strcmp(league->turn, (*me)->user) == 0)
				{
					player = fromClient->data.name;

					FOR_EACH(elemPlayer, league->availablePlayers)
					{
						/* Encontre el jugador */
						if(strcmp(player,((Player)elemPlayer->data)->name) == 0)
						{
							/* Saco el jugador de availablePlayers */
							Remove(elemPlayer, league->availablePlayers);

							/* Busco en que team ponerlo */
							FOR_EACH(elemTeam, league->teams)
							{
								if(strcmp(((Team)elemTeam->data)->owner,(*me)->user) == 0)
								{
									break;
								}
							}

							AddElemToList(elemPlayer,((Team)elemTeam->data)->players);

							/* Seteo en la liga la variable que indica que respondi */
							league->answer = TRUE;
							toClient = createMsg_s(CHOOSE);
							toClient->status = OK;
							AddToList(playerChooseSuccessful, toClient->msgList);
							communicate(ch, toClient);
						}
					}

					if(league->answer == TRUE)
					{
						Remove(elemPlayer, league->availablePlayers);
					}
					turnFlag = FALSE;
				}
				rc = pthread_mutex_unlock(&game_mutex);
			}
			else
			{
				toClient = createMsg_s(CHOOSE);
				toClient->status = !OK;
				AddToList(cannotChoose, toClient->msgList);
				communicate(ch, toClient);
			}
		}

		sleep(1);
	}
	toClient = createMsg_s(DRAFT_END);
	toClient->status = OK;
	AddToList(draftEnded, toClient->msgList);
	communicate(ch, toClient);
}


void * coordinator_thread(void * data)
{
	League l = (League)data;
	l->turn = ((Team)l->teams->pFirst->data)->owner;
	int repeatFlag = FALSE;
	int rc = pthread_mutex_lock(&game_mutex);

	while(l->status == DRAFTING)
	{
		int time = 0;
		l->answer = FALSE;
		while(time < TIME_OUT)
		{
			if(l->answer == TRUE)
			{
				changeTurn(l, &repeatFlag);
				break;
			}
			time++;
			rc = pthread_mutex_unlock(&game_mutex);
			sleep(1);
			rc = pthread_mutex_lock(&game_mutex);
		}
		if(time == TIME_OUT && l->answer == FALSE)
		{
			autoAsign(l, &repeatFlag);
		}
		checkStatus(l);
	}
	rc = pthread_mutex_unlock(&game_mutex);
	return NULL;
}

void changeTurn(League l, int * repeatFlag)
{
	Element e;

	FOR_EACH(e, l->teams)
	{
		if(strcmp(((Team)e->data)->owner, l->turn) == 0)
		{
			break;
		}
	}

	if(e == NULL)
	{
		printf("Unexpected error! User not found while drafting.\n");
		exit(EXIT_FAILURE);
	}

	if((e->next == NULL || e->prev == NULL) && *repeatFlag == FALSE)
	{
		*repeatFlag = TRUE;
		return;
	}

	if(e->next == NULL)
	{
		l->turn = ((Team)l->teams->pFirst->data)->owner;
		return;
	}

	l->turn = ((Team)e->data)->owner;
	return;
}

void autoAsign(League l, int * repeatFlag)
{
	Element e;

	FOR_EACH(e, l->teams)
	{
		if(strcmp(l->turn, ((Team)e->data)->owner) == 0)
		{
			break;
		}
	}

	int n = (rand() % l->availablePlayers->NumEl);
	int i;

	Element p = l->availablePlayers->pFirst;

	for(i = 0; i <= n; i++)
	{
		p = p->next;
		if(p == NULL)
		{
			printf("Unexpected error! Available Players list changed");
			exit(EXIT_FAILURE);
		}
	}

	AddElemToList(p,((Team)e->data)->players);

	changeTurn(l, repeatFlag);
	return;
}

void checkStatus(League l)
{
	Element e;
	FOR_EACH(e, l->teams)
	{
		if(l->teams->NumEl < CANT_PLAYERS)
		{
			return;
		}
	}

	l->status = ACTIVE;
	return;
}
