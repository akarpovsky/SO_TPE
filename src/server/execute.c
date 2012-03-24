#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
# define OK 0
# define ERROR 1

#include "../utils/LinkedList.h"
#include "../includes/structs.h"
#include "../includes/message.h"
#include "../IO/io.h"
#include "../includes/fifo_s.h"


Game gameAux = NULL;
User me;


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

void register_c(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * user = (msg->data).login_t.user;
	char * pass = (msg->data).login_t.pass;
	char * toPrint;

	int i;
	Element elem;

	FOR_EACH(elem, gameAux->users){

		/* Caso: El usuario ya existe */
		if(strcmp(((User)(elem->data))->user,user) == 0){

			toPrint = malloc(strlen("The user already exists") + 1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}	
			strcpy(toPrint,"The user already exists");
			AddToList(toPrint,answer->msgList);
			answer->status = ERROR;

			//comunicate(ch, answer);
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
		
	toPrint = malloc(strlen("Successful registration")+1);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	strcpy(toPrint,"Successful registration");
	AddToList(toPrint,answer->msgList);
	answer->status = OK;

//	comunicate(ch,answer);

	return;

}


void executeLogin(Msg_t msg, Channel ch){

 	Msg_s answer = createMsg_s();
	char * user = (msg->data).login_t.user;
	char * pass = (msg->data).login_t.pass;
	char * toPrint;
	char * usuario;

	int i;
	Element elem;

	/* Me fijo si ya estoy conectado */
	FOR_EACH(elem, gameAux->loggedUsers){

		/* Caso: El usuario ya esta conectado */
		if(strcmp((char *)elem->data,user) == 0){

			toPrint = malloc(strlen("You are already logged")+1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}	
			strcpy(toPrint,"You are already logged");
			AddToList(toPrint,answer->msgList);
			answer->status = ERROR;

		//	comunicate(ch, answer);
			return;

		}
	}

	/* Me fijo si el usuario existe y lo loggeo*/
	FOR_EACH(elem, gameAux->users){

		/* Caso: El usuario existe */
		if(strcmp(((User)(elem->data))->user,user) == 0 &&
		 			strcmp(((User)(elem->data))->pass,pass) == 0){

			toPrint = malloc(strlen("Welcome! You are logged in") + 1);
			if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
			}	
			strcpy(toPrint,"Welcome! You are logged in");
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

			me = (User)elem->data;
			//comunicate(ch, answer);
			return;

		}
	}

	/* Usuario o contraseña incorrectos */

	toPrint = malloc(strlen("User or Password incorrects") + 1);
	if(toPrint == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	strcpy(toPrint,"User or Password incorrects");
	AddToList(toPrint,answer->msgList);
	answer->status = ERROR;
//	comunicate(ch, answer);
	return;

}

void executeListLeagues(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;

	if(gameAux->leagues->NumEl == 0){
		toPrint = malloc(strlen("There is no league created") + 1);
		if(toPrint == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"There is no league created");
		AddToList(toPrint,answer->msgList);
		answer->status = OK;
	//	comunicate(ch, answer);
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

	answer->status = OK;
	//comunicate(ch,answer);
	return;

}

void executeListTeams(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;

	if(gameAux->cantTeams == 0){
		toPrint = malloc(strlen("There is no team created") + 1);
		if(toPrint == NULL){
				perror("Insufficient memory\n");
				exit(EXIT_FAILURE);
		}
		strcpy(toPrint,"There is no team created");
		AddToList(toPrint,answer->msgList);
		answer->status = OK;
	//	comunicate(ch, answer);
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
	//comunicate(ch,answer);
	return;

}

void executeListTrades(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;

	int dim;
	Element elem;
	Element elemTrade;

	FOR_EACH(elem, gameAux->leagues){
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


		FOR_EACH(elemTrade, ((League)elem->data)->trades){

			if(strcmp(me->user,((Trade)elemTrade->data)->userFrom) == 0 ||
				strcmp(me->user,((Trade)elemTrade->data)->userTo) == 0){
				
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

	answer->status = OK;
//	comunicate(ch,answer);
	return;

}

void executeLeagueShow(Msg_t msg, Channel ch){

	Msg_s answer = createMsg_s();
	char * toPrint;
	int input = msg->data.show_t.ID;

	int dim;
	Element elem;
	Element elemItem;

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

			answer->status = OK;
		//	comunicate(ch,answer);
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
	answer->status = ERROR;
//	comunicate(ch,answer);
	return;

}

void executeTeamShow(Msg_t msg, Channel ch){
	
	Msg_s answer = createMsg_s();
	char * toPrint;
	int input = msg->data.show_t.ID;

	int dim;
	Element elemLeague;
	Element elemTeam;
	Element elemPlayer;

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

				answer->status = ERROR;
			//	comunicate(ch,answer);
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
	answer->status = ERROR;
//	comunicate(ch,answer);
	return;
}

void executeTradeShow(Msg_t msg, Channel ch){
	
	Msg_s answer = createMsg_s();
	char * toPrint;
	int input = msg->data.show_t.ID;

	int dim;
	Element elemLeague;
	Element elemTrade;

	FOR_EACH(elemLeague, gameAux->leagues){

		FOR_EACH(elemTrade, ((League)elemLeague->data)->trades){

			if(((Trade)elemTrade->data)->ID == input){

				/* Frase User from */
				dim = strlen("User from:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"User from:");
				AddToList(toPrint,answer->msgList);

				/* UserFrom */
				dim = strlen(((Trade)elemTrade->data)->userFrom);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->userFrom);
				AddToList(toPrint,answer->msgList);

				/* Frase User to */
				dim = strlen("User to:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"User to:");
				AddToList(toPrint,answer->msgList);

				/* UserTo */
				dim = strlen(((Trade)elemTrade->data)->userTo);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->userTo);
				AddToList(toPrint,answer->msgList);

				/* Frase Player from */
				dim = strlen("Player from:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Player from:");
				AddToList(toPrint,answer->msgList);

				/* PlayerFrom */
				dim = strlen(((Trade)elemTrade->data)->playerFrom);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->playerFrom);
				AddToList(toPrint,answer->msgList);

				/* Frase Player to */
				dim = strlen("Player to:");
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,"Player to:");
				AddToList(toPrint,answer->msgList);

				/* playerTo */
				dim = strlen(((Trade)elemTrade->data)->playerTo);
				toPrint = malloc(dim + 1);
				if(toPrint == NULL){
					perror("Insufficient memory\n");
					exit(EXIT_FAILURE);
				}
				strcpy(toPrint,((Trade)elemTrade->data)->playerTo);
				AddToList(toPrint,answer->msgList);
			}

		}
	}

}


int main(void){

	gameAux = loadGame();

	return 0;
}
