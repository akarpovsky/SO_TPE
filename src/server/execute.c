#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define OK 0
#define ERROR 1
#define WAIT 2
#define ACTIVE 1
#define INACTIVE 0

#include "../utils/LinkedList.h"
#include "../includes/structs.h"
#include "../includes/message.h"
#include "../IO/io.h"
#include "../includes/execute.h"

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
//					executeDraft(msg,ch);
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

			toPrint = malloc(strlen("The user already exists.") + 1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}	
			strcpy(toPrint,"The user already exists.");
			AddToList(toPrint,answer->msgList);
			answer->status = ERROR;

			rc = pthread_mutex_unlock(&game_mutex);

			communicate(ch, answer);
			return;

		}
	}
	/* Creo nueva estructura user */
	User newUser = (User) malloc(sizeof(user));
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
		
	toPrint = malloc(strlen("Successful registration.")+1);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	strcpy(toPrint,"Successful registration.");
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

	int rc,dim;
	Element elem;

	rc = pthread_mutex_lock(&game_mutex);

	if( (*me) == NULL){

		/* Me fijo si ya estoy conectado */
		FOR_EACH(elem, gameAux->loggedUsers){

			/* Caso: El usuario ya esta conectado */
			if(strcmp((char *)elem->data,user) == 0){

				toPrint = malloc(strlen("You are already logged.")+1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}	
				strcpy(toPrint,"You are already logged.");
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
						toPrint = malloc(strlen("Welcome! You are logged in.") + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}	
				strcpy(toPrint,"Welcome! You are logged in.");
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

		toPrint = malloc(strlen("Incorrect user or password.") + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"Incorrect user or password.");
		AddToList(toPrint,answer->msgList);
		answer->status = ERROR;

	}else{
		/* Ya hay alguien loggeado */
		dim = strlen("There is already someone logged.");
		toPrint = malloc(dim + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"There is already someone logged.");
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
		toPrint = malloc(strlen("There is no league created.") + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"There is no league created.");
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
		toPrint = malloc(strlen("There is no team created.") + 1);
		if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"There is no team created.");
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

		dim = strlen("In League:");
		toPrint = malloc(dim + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"In League:");
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
					dim = strlen("In League:");
					toPrint = malloc(dim + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					strcpy(toPrint,"In League:");
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
		dim = strlen("You don't have any thread.");
		toPrint = malloc(dim + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"You don't have any thread.");
		AddToList(toPrint,answer->msgList);
	}
	
	}else{
		toPrint = malloc(strlen("You have to be logged.") + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"You have to be logged.");
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
			dim = strlen("Free players:");
			toPrint = malloc(dim + 1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}
			strcpy(toPrint,"Free players:");
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

				dim = strlen("ID:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"ID:");
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

				dim = strlen("Points:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Points:");
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

	toPrint = malloc(strlen("Incorrect ID") + 1);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(toPrint,"Incorrect ID");
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
				dim = strlen("In League:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"In League:");
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
				dim = strlen("Owner:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Owner:");
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
				dim = strlen("Points:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Points:");
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
				dim = strlen("Players:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Players:");
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

	toPrint = malloc(strlen("Incorrect ID") + 1);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(toPrint,"Incorrect ID");
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

		toPrint = malloc(strlen("Incorrect ID") + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"Incorrect ID");
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
				dim = strlen("User from:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"User from:");
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
				dim = strlen("User to:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"User to:");
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
				dim = strlen("Player from:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Player from:");
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
				dim = strlen("Player to:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Player to:");
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
		toPrint = malloc(strlen("You have to be logged.") + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"You have to be logged.");
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
	Trade trade = (Trade)malloc(sizeof(trade));
	if(trade == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}

	rc = pthread_mutex_lock(&game_mutex);
	
	/* Si el ID es incorrecto */
	if(teamID > gameAux->cantTeams){
		toPrint = malloc(strlen("Incorrect ID") + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"Incorrect ID");
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
					toPrint = malloc(strlen("The league is inactive.") + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					strcpy(toPrint,"The league is inactive.");
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
					toPrint = malloc(strlen("You don't have a team in the same league.") + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					strcpy(toPrint,"You don't have a team in the same league.");
					AddToList(toPrint,answer->msgList);
					rc = pthread_mutex_unlock(&game_mutex);
					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}
				
				/* Si a el jugador que quiero adquirir lo tiene el otro equipo */
				flag = 0;
				FOR_EACH(elemPlayer, ((Team)elemTeam->data)->players){
					printf("Comparando a %s con SUS jugadores %s\n", ((Player)elemPlayer->data)->name, ((Player)elemPlayer->data)->name);

					if(strcmp(((Player)elemPlayer->data)->name,to) == 0){
						flag = 1;
						break;
					}
				}
				if(flag == 0){
					toPrint = malloc(strlen("The other team doesn't have that player.") + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					strcpy(toPrint,"The team other doesn't have that player.");
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
					toPrint = malloc(strlen("Your team doesn't have that player.") + 1);
					if(toPrint == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					strcpy(toPrint,"Your team doesn't have that player.");
					AddToList(toPrint,answer->msgList);

					rc = pthread_mutex_unlock(&game_mutex);

					answer->status = ERROR;
					communicate(ch,answer);
					return;
				}
				
				/* CREO EL TRADE */
				trade->ID = ++(gameAux->cantTrades);
				trade->state = WAIT;
				
				trade->playerFrom = (char*) malloc(strlen(from)+200);
				if(trade->playerFrom == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(trade->playerFrom,from);
				printf("%s\n",trade->playerFrom);
				
				trade->playerTo = (char*) malloc(strlen(to)+1);
				if(trade->playerTo == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(trade->playerTo,to);
				
				trade->userFrom = (char*) malloc(strlen(((*me)->user)+1));
				if(trade->userFrom == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(trade->userFrom, (*me)->user);

				trade->userTo = (char*) malloc(strlen(((Team)elemTeam->data)->owner)+1);
				if(trade->userTo == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(trade->userTo,((Team)elemTeam->data)->owner);
				
				AddToList(trade,((League)elemLeague->data)->trades);

				
				toPrint = malloc(strlen("Trade created successfully.") + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Trade created successfully.");
				AddToList(toPrint,answer->msgList);

				rc = pthread_mutex_unlock(&game_mutex);

				answer->status = OK;
				communicate(ch,answer);
				return;
				
			}
		}		
	}
}
	
	

