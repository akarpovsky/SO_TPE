#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define OK 0
#define ERROR 1
#define WAIT 2

#define INACTIVE 0
#define DRAFT 2
#define ACTIVE 3

#include "../utils/LinkedList.h"
#include "../includes/structs.h"
#include "../includes/message.h"
#include "../IO/io.h"
#include "../includes/execute.h"
#include "../includes/toPrint.h"

#ifdef fifo
	#include "../includes/fifo_s.h"
#endif
#ifdef sockets
	#include "../includes/socket_s.h"
#endif
#ifdef msgqueue
	#include "../includes/mq_s.h"
#endif
#ifdef shmm
	#include "../includes/shmm_s.h"
#endif

#define DEFINE_VARIABLES
#include "../includes/global.h"


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

void * createMsg_s(void){

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
//					executeTradeWithdraw(msg,ch);
					break;
					
		case TRADE_ACCEPT:
//					executeTradeAccept(msg,ch);
					break;
					
		case TRADE_NEGOTIATE:
//					executeTradeNegotiate(msg,ch);
					break;
					
		case LOGOUT:
//					executeLogout(msg,ch,me);
					break;
					
		case JOIN_LEAGUE:
//					executeJoinLeague(msg,ch);
					break;
					
		case CREATE_LEAGUE:
//					executeCreateLeague(msg,ch);
					break;
	
		case DRAFT:
//					executeDraft(msg,ch,me);
					break;		
									
		case DRAFT_OUT:
//					executeDraftOut(msg,ch);
					break;		
	}
	
}

void executeRegister(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * user = (msg->data).login_t.user;
	char * pass = (msg->data).login_t.pass;
	char * toPrint;

	int rc;
	Element elem;

	rc = pthread_mutex_lock(&game_mutex);

	FOR_EACH(elem, gameAux->users){

		/* Caso: El usuario ya existe */
		if(strcmp(((User)(elem->data))->user,user) == 0){

			toPrint = userExists;
			AddToList(toPrint,answer->msgList);
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
		
	toPrint = successfulReg;
	AddToList(toPrint,answer->msgList);
	answer->status = OK;

	communicate(ch,answer);

	rc = pthread_mutex_unlock(&game_mutex);

	return;

}

void executeLogin(Msg_t msg, Channel ch, User * me){

 	Msg_s answer = createMsg_s();
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

				toPrint = logged;
				AddToList(toPrint,answer->msgList);
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
				
				toPrint = welcome;
				AddToList(toPrint,answer->msgList);
				
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

		toPrint = incorrectPass;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;

	}else{
		/* Ya hay alguien loggeado */
		toPrint = alreadyLogged;
		AddToList(toPrint,answer->msgList);
	}	
	rc = pthread_mutex_unlock(&game_mutex);

	communicate(ch, answer);
	return;

}

void executeListLeagues(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;
	int rc;

	rc = pthread_mutex_lock(&game_mutex);

	if(gameAux->leagues->NumEl == 0){
		toPrint = noLeague;
		AddToList(toPrint,answer->msgList);
		answer->status = OK;

		rc = pthread_mutex_unlock(&game_mutex);

		communicate(ch, answer);
		return;
	}

	int dim;
	Element elem;

	FOR_EACH(elem, gameAux->leagues){

		/* ID */
		dim = floor(log10(((League)elem->data)->ID));
		toPrint = malloc(dim + 1);
		if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}
		itoa(((League)elem->data)->ID,toPrint);
		AddToList(toPrint,answer->msgList);

		/* Nombre */
		dim = strlen(((League)(elem->data))->name);
		toPrint = malloc(dim + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,((League)(elem->data))->name);
		AddToList(toPrint,answer->msgList);

	}

	rc = pthread_mutex_unlock(&game_mutex);

	answer->status = OK;
	communicate(ch,answer);
	return;

}

void executeListTeams(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;
	int rc;

	rc = pthread_mutex_lock(&game_mutex);

	if(gameAux->cantTeams == 0){
		toPrint = noTeam;
		AddToList(toPrint,answer->msgList);
		answer->status = OK;

		rc = pthread_mutex_unlock(&game_mutex);

		communicate(ch, answer);
		return;
	}

	int dim;
	Element elem;
	Element elemTeam;

	FOR_EACH(elem, gameAux->leagues){

		toPrint = inLeague;
		AddToList(toPrint,answer->msgList);

		dim = strlen(((League)elem->data)->name);
		toPrint = malloc(dim + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,((League)elem->data)->name);
		AddToList(toPrint,answer->msgList);

		FOR_EACH(elemTeam, ((League)elem->data)->teams){
			dim = floor(log10(((Team)(elemTeam->data))->ID));
			toPrint = malloc(dim + 1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}
			itoa(((Team)(elemTeam->data))->ID,toPrint);
			AddToList(toPrint,answer->msgList);

			dim = strlen(((Team)elemTeam->data)->owner);
			toPrint = malloc(dim + 1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}
			strcpy(toPrint,((Team)elemTeam->data)->owner);
			AddToList(toPrint,answer->msgList);

		}

	}

	answer->status = OK;
	communicate(ch,answer);

	rc = pthread_mutex_unlock(&game_mutex);

	return;

}

void executeListTrades(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s();
	char * toPrint;

	int dim,rc;
	Element elem;
	Element elemTrade;

	rc = pthread_mutex_lock(&game_mutex);
	
	if(*me != NULL){
	
	FOR_EACH(elem, gameAux->leagues){

		FOR_EACH(elemTrade, ((League)elem->data)->trades){
				if(strcmp((*me)->user,((Trade)elemTrade->data)->userFrom) == 0 ||
					strcmp((*me)->user,((Trade)elemTrade->data)->userTo) == 0){
				
					/* Agrego la frase "In League" */
					toPrint = inLeague;
					AddToList(toPrint,answer->msgList);

					/* Agrego el nombre de la League */
					dim = strlen(((League)elem->data)->name);
					toPrint = malloc(dim + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					strcpy(toPrint,((League)elem->data)->name);
					AddToList(toPrint,answer->msgList);
					
					dim = floor(log10(((Trade)elemTrade->data)->ID));
					toPrint = malloc(dim + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					itoa(((Team)elemTrade->data)->ID,toPrint);
					AddToList(toPrint,answer->msgList);
				}
		}
	}
	
	if(answer->msgList->NumEl == 0){
		toPrint = noTrade;
		AddToList(toPrint,answer->msgList);
	}
	
	}else{
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = OK;
	}

	rc = pthread_mutex_unlock(&game_mutex);

	answer->status = OK;
	communicate(ch,answer);
	return;

}

void executeLeagueShow(Msg_t msg, Channel ch){


	
	Msg_s answer = createMsg_s();
	char * toPrint;
	int input = msg->data.show_t.ID;

	int dim,rc;
	Element elem;
	Element elemItem;

	rc = pthread_mutex_lock(&game_mutex);

	FOR_EACH(elem, gameAux->leagues){

		if(((League)(elem->data))->ID == input){
		
			/* Imprimo el nombre de la league */
			dim = strlen(((League)elem->data)->name);
			toPrint = malloc(dim + 1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}
			strcpy(toPrint,((League)elem->data)->name);
			AddToList(toPrint,answer->msgList);

			/* Jugadores sin draftear */
			toPrint = freePlayers;
			AddToList(toPrint,answer->msgList);

			FOR_EACH(elemItem, ((League)elem->data)->availablePlayers){

				/* Nombre jugador */
				dim = strlen(((Player)elemItem->data)->name);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Player)elemItem->data)->name);
				AddToList(toPrint,answer->msgList);

				/* Puntaje */

				dim = floor(log10(((Player)elemItem->data)->points));
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				itoa(((Player)elemItem->data)->points,toPrint);
				AddToList(toPrint,answer->msgList);

			}

			FOR_EACH(elemItem, ((League)elem->data)->teams){

				/* Nombre del equipo */
				dim = strlen(((Team)elemItem->data)->owner);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Team)elemItem->data)->owner);
				AddToList(toPrint,answer->msgList);

				toPrint = IDString;
				AddToList(toPrint,answer->msgList);

				/* ID */
				dim = floor(log10(((Team)(elemItem->data))->ID));
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				itoa(((Team)elemItem->data)->ID,toPrint);
				AddToList(toPrint,answer->msgList);

				toPrint = pointsString;
				AddToList(toPrint,answer->msgList);

				dim = floor(log10(((Team)elemItem->data)->points));
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				itoa(((Team)elemItem->data)->points,toPrint);
				AddToList(toPrint,answer->msgList);

			}

			rc = pthread_mutex_unlock(&game_mutex);

			answer->status = OK;
			communicate(ch,answer);
			return;

		}

	}

	toPrint = incorrectID;
	AddToList(toPrint,answer->msgList);

	rc = pthread_mutex_unlock(&game_mutex);

	answer->status = ERROR;
	communicate(ch,answer);
	return;

}

void executeTeamShow(Msg_t msg, Channel ch){
	
	Msg_s answer = createMsg_s();
	char * toPrint;
	int input = msg->data.show_t.ID;

	int dim,rc;
	Element elemLeague;
	Element elemTeam;
	Element elemPlayer;

	rc = pthread_mutex_lock(&game_mutex);

	FOR_EACH(elemLeague, gameAux->leagues){

		FOR_EACH(elemTeam, ((League)elemLeague->data)->teams){

			if(((Team)elemTeam->data)->ID == input){

				/* Frase In League */
				toPrint = inLeague;
				AddToList(toPrint,answer->msgList);

				/* Agrego el nombre de la League */
				dim = strlen(((League)elemLeague->data)->name);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((League)elemLeague->data)->name);
				AddToList(toPrint,answer->msgList);

				/* Frase owner */
				toPrint = ownerString;
				AddToList(toPrint,answer->msgList);

				/* Owner */
				dim = strlen(((Team)elemTeam->data)->owner);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Team)elemTeam->data)->owner);
				AddToList(toPrint,answer->msgList);

				/* Frase Points*/
				toPrint = pointsString;
				AddToList(toPrint,answer->msgList);

				/* Points */
				dim = floor(log10(((Team)elemTeam->data)->points));	
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				itoa(((Team)elemTeam->data)->points,toPrint);
				AddToList(toPrint,answer->msgList);

				/* Frase Players */
				toPrint = playersString;
				AddToList(toPrint,answer->msgList);

				/* Player */
				FOR_EACH(elemPlayer, ((Team)elemTeam->data)->players){

					/* Nombre del jugador */
					dim = strlen(((Player)elemPlayer->data)->name);
					toPrint = malloc(dim + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					strcpy(toPrint,((Player)elemPlayer->data)->name);
					AddToList(toPrint,answer->msgList);

					/* Points */
					dim = floor(log10(((Player)elemPlayer->data)->points));	
					toPrint = malloc(dim + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					itoa(((Player)elemPlayer->data)->points,toPrint);
					AddToList(toPrint,answer->msgList);

				}

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = ERROR;
				communicate(ch,answer);
				return;
			} 
		}
	}

	toPrint = incorrectID;
	AddToList(toPrint,answer->msgList);

	rc = pthread_mutex_unlock(&game_mutex);

	answer->status = ERROR;
	communicate(ch,answer);
	return;
}

void executeTradeShow(Msg_t msg, Channel ch){
	
	Msg_s answer = createMsg_s();
	char * toPrint;
	int input = msg->data.show_t.ID;

	int dim,rc;
	Element elemLeague;
	Element elemTrade;

	rc = pthread_mutex_lock(&game_mutex);
	
	printf("CANTINDAD DE TRADES %d\n",gameAux->cantTrades );
	
	if(input > gameAux->cantTrades){

		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		
		rc = pthread_mutex_unlock(&game_mutex);

		answer->status = ERROR;
		communicate(ch,answer);
		return;
		
	}

	FOR_EACH(elemLeague, gameAux->leagues){

		FOR_EACH(elemTrade, ((League)elemLeague->data)->trades){

			if(((Trade)elemTrade->data)->ID == input){
				printf("LLEGUEEEEE  111\n");
				
				/* Frase User from */
				toPrint = userF;
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  222\n");
				
				/* UserFrom */
				dim = strlen(((Trade)elemTrade->data)->userFrom);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->userFrom);
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  333\n");
				
				/* Frase User to */
				toPrint = userT;
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  444\n");
				
				/* UserTo */
				dim = strlen(((Trade)elemTrade->data)->userTo);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->userTo);
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  555\n");
				
				/* Frase Player from */
				toPrint = playerF;
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  666\n");
				
				/* PlayerFrom */
				dim = strlen(((Trade)elemTrade->data)->playerFrom);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->playerFrom);
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  777\n");
				
				/* Frase Player to */
				toPrint = playerT;
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  888\n");
				
				/* playerTo */
				dim = strlen(((Trade)elemTrade->data)->playerTo);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->playerTo);
				AddToList(toPrint,answer->msgList);
				printf("LLEGUEEEEE  999\n");
				
				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = OK;
				communicate(ch,answer);
				return;
			}
		}
	}
}

void executeTrade(Msg_t msg, Channel ch, User * me){
	
	Msg_s answer = createMsg_s();
	char * toPrint;
	
	if((*me) == NULL){
		/* Si no esta loggeado el usuario */
		toPrint = noLogged;
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;
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
		toPrint = incorrectID;
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
					toPrint = inactiveLeague;
					AddToList(toPrint,answer->msgList);

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
					toPrint = noTeamInLeague;
					AddToList(toPrint,answer->msgList);
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
					toPrint = noPlayerTo;
					AddToList(toPrint,answer->msgList);

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
					toPrint = noPlayerFrom;
					AddToList(toPrint,answer->msgList);

					rc = pthread_mutex_unlock(&game_mutex);

					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}
				
				/* CREO EL TRADE */
				t->ID = ++(gameAux->cantTrades);
				t->state = WAIT;
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

				
				toPrint = successfulTrade;
				AddToList(toPrint,answer->msgList);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = OK;
				communicate(ch,answer);
				return;
				
			}
		}		
	}
}

void executeDraft(Msg_t msg, Channel ch, User * me){

	Msg_s answer = createMsg_s();
	char * toPrint;
	int input = msg->data.ID;
	int rc;
	Element elemLeague, elemID;
	int flag;
	
	rc = pthread_mutex_lock(&game_mutex);
	
	/* Liga no existente */
	if(input > gameAux->leagues->NumEl){
		toPrint = incorrectID;
		AddToList(toPrint,answer->msgList);
		
		rc = pthread_mutex_unlock(&game_mutex);

		answer->status = ERROR;
		communicate(ch,answer);
		return;
	}
	
	FOR_EACH(elemLeague, gameAux->leagues){
		
		/* En elemLeague cargo la liga que corresponde */
		if(((League)elemLeague->data)->ID == input){
			
			
			/* Caso que yo no pertenezca a la league */
			flag = 0;
			FOR_EACH(elemID, (*me)->leaguesIDs){
				if(((int*)elemID->data) == ((League)elemLeague->data)->ID){
					flag = 1;
					break;
				}							
			}
			if(flag == 0){
				toPrint = noTeamInLeague;
				AddToList(toPrint,answer->msgList);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = ERROR;
				communicate(ch,answer);
				return;
			}
			
			
			/* Caso de liga que ya se drafteo */
			if(((League)elemLeague->data)->status == ACTIVE){
				toPrint = alreadyDrafted;
				AddToList(toPrint,answer->msgList);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = ERROR;
				communicate(ch,answer);
				return;
			}
			
			/* No hay teams suficientes para draftear */
			if(((League)elemLeague->data)->teams->NumEl < CANT_TEAMS){
				toPrint = notEnoughTeams;
				AddToList(toPrint,answer->msgList);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = ERROR;
				communicate(ch,answer);
				return;
			}
			
			if(((League)->elemLeague->data)->status == INACTIVE){
				
				((League)->elemLeague->data)->cantDraft++;
				
				if(((League)->elemLeague->data)->cantDraft == CANT_TEAMS){
					
					/* Creo el thread coordinador */
					pthread_t coordinatorThread;
					int iRet;
					iRet = pthread_create(&coordinatorThread, NULL, coordinator_thread, NULL);
					if (iRet){
						printf("ERROR; return code from pthread_create() is %d\n", iRet);
						exit(EXIT_FAILURE);
					}
					
					makeDraft((League)elemLeague->data, ch, me);
					
				}else{
					//esperar mensaje logout o draft out
				}
			}else{
				makeDraft((League)elemLeague->data, ch, me);
			}
			
		}//IF ES EL ID DE LA LEAGUE
	}//FOR_EACH
}

void makeDraft(League league,Channel ch, User * me){
	
	Msg_t fromClient;
	char * player;
	Element elemPlayer,elemTeam;
	int flag;
	
	while(league->status == DRAFT){
		
		fromClient = IPClisten(ch);

		/* Mi turno*/
		if(strcmp(league->turn, (*me)->user) == 0){
			
			if(fromClient->type == DRAFT_OUT){
				league->cantDraft--;
				return;
			}
			if(fromClient->type == LOGOUT){
				league->cantDraft--;
				executeLogout(fromClient,ch,me);
				
				return;
			}
			
			if(fromClient->type = CHOOSE){
				
				player = fromClient->data->name;
				flag = 0;
				
				FOR_EACH(elemPlayer, league->availablePlayers){
					/* Encontre el jugador */
					if(strcmp(player,((Player)elemPlayer->data)->name) == 0){						
						/* Saco el jugador de availablePlayers */
						Remove(elemPlayer, league->availablePlayers);
						
						/* Busco en que team ponerlo */
						FOR_EACH(elemTeam, league->teams){
							if(strcmp(((Team)elemTema->data)->name,(*me)->user) == 0){
								break;
							}
						}
					
						AddElemToList(elemPlayer,((Team)elemTeam->data)->players);
						
						/* Seteo en la liga la variable que indica que respondi */
						league->answer = OK;
						return;
					}
				}

			}
			
		}
		
		
	}
	
	
}
	
	

