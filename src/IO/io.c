#include "./io.h"

/*
* function loadUser
*
* Alloc memory for a user_t and Loads a specific user
* fiven the path of the saved one.
*
* @path: Path of the saved user file 
*/
User loadUser(char * path){
	

	FILE * file;
	int * id;
	

	User auxUser; // Will store the loaded user
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int dim, num, auxNum;
	
	auxUser = malloc(sizeof(user_t));
	
	if(auxUser == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	
	file = fopen(path,"r");
	
	if(file == NULL){
		printf("<LOG - io.c>\n\tFile not found.\n<end>\n");
		exit(EXIT_FAILURE);
	}

	/* Load username */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->user  = (char *) malloc(dim);
	if(auxUser->user == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	aux[dim-1] = 0;
	strcpy(auxUser->user,aux);

	/* Load password */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxUser->pass  = (char *)  malloc(dim);
	if(auxUser->pass == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	aux[dim-1] = 0;
	strcpy(auxUser->pass,aux);

	
	/* Load number of Leagues */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxUser->leagues = num;
	
	auxUser->leaguesIDs = (List) malloc(sizeof(llist));
	if(auxUser->leaguesIDs == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	
	CreateList(auxUser->leaguesIDs);
	
	int i;
	for(i = 0; i < num; i++){
		fgets(aux,BUFFER_SIZE,file);
		auxNum = atoi(aux);
		id = malloc(sizeof(int));
		*id = auxNum;
		AddToList(id,auxUser->leaguesIDs);
	}
	
	 printUser(auxUser);
	
	 fclose(file);

	return auxUser;
}

void printUser(User user){
	printf("\n-------- USUARIO ---------\n\n");
	printf("\tNombre: %s\n", user->user);
	printf("\tPassword: %s\n", user->pass);
	printf("\tCant. ligas: %d\n", user->leagues);
	printf("\n--------------------------\n\n");
}



/*
* function loadTrade
*
* Allocates memory for a trade and Loads a specific trade
* given the path of the saved one.
*
* @path: Path of the saved trade file 
*/

Trade loadTrade(char * path){

	FILE * file;
	
	Trade auxTrade; // Will store the loaded trade
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int dim, num;
		
	auxTrade = malloc(sizeof(trade_t));
	
	if(auxTrade == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	
	file = fopen(path,"r");
	
	if(file == NULL){
		perror("File not found\n");
		exit(EXIT_FAILURE);
	}
	
	/* Load id */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTrade->ID = num;

	/* Load state */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTrade->state = num;
	
	/* Load userFrom */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->userFrom  = (char *) malloc(dim);
	if(auxTrade->userFrom == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	aux[dim-1] = 0;
	strcpy(auxTrade->userFrom,aux);

	
	/* Load userTo */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->userTo  = (char *) malloc(dim);
	if(auxTrade->userTo == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	aux[dim-1] = 0;
	strcpy(auxTrade->userTo,aux);

	
	/* Load playerFrom */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->playerFrom  = (char *) malloc(dim);
	if(auxTrade->playerFrom == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	aux[dim-1] = 0;
	strcpy(auxTrade->playerFrom,aux);

	
	/* Load playerTo */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTrade->playerTo  = (char *) malloc(dim);
	if(auxTrade->playerTo == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	aux[dim-1] = 0;
	strcpy(auxTrade->playerTo,aux);

	fclose(file);
	
	return auxTrade;
}


/*
* function loadTeam
*
* Alloc memory for a team and Loads a specific team
* given the path of the saved one.
*
* @path: Path of the saved trade file 
*/
Team loadTeam(char * path){
	
	FILE * file;

	Team auxTeam; // Will store the loaded team
	Player auxPlayer;
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int dim, num, i;
	
	auxTeam = malloc(sizeof(team_t));
	
	if(auxTeam == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	
	file = fopen(path,"r");
	
	if(file == NULL){
		perror("File not found\n");
		exit(EXIT_FAILURE);
	}

	/* Load id */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTeam->ID = num;
	
	/* Load owner */
	fgets(aux,BUFFER_SIZE,file);
	dim = strlen(aux);
	auxTeam->owner  = (char *) malloc(dim);
	if(auxTeam->owner == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	aux[dim-1] = 0;
	strcpy(auxTeam->owner,aux);

	

	/* Load points */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	auxTeam->points = num;

	/* Load cantPlayers */
	fgets(aux,BUFFER_SIZE,file);
	num = atoi(aux);
	if(num > MAX_PLAYERS){
		printf("<LOG - io.c>\n\tInvalid file, too much players\n<end>\n");
		exit(EXIT_FAILURE);
	}
	
	auxTeam->cantPlayers = num;


	/* Load players */

	auxTeam->players = (List) malloc(sizeof(llist));
	if(auxTeam->players == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}

	CreateList(auxTeam->players);

	for(i = 0; i < auxTeam->cantPlayers; i++){
		auxPlayer = malloc(sizeof(player_t));
		if(auxPlayer == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		fgets(aux,BUFFER_SIZE,file);
		dim = strlen(aux);
		auxPlayer->name  = (char *) malloc(dim);
		if(auxPlayer->name == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
		aux[dim-1] = 0;
		strcpy(auxPlayer->name,aux);


		/* Load points */
		fgets(aux,BUFFER_SIZE,file);
		num = atoi(aux);
		auxPlayer->points = num;	
		
		AddToList(auxPlayer,auxTeam->players);
	}

	fclose(file);

	return auxTeam;
}


/*
* function dismissWrongFiles
*
* Dismiss "." and ".." files
*
* @entry: Dirent entry 
*/

int dismissWrongFiles(struct dirent * entry){
	if (entry->d_name[0] == '.'){
			return 0;			
		}
	else
		return 1;
}

/*
*
* function fileType
*
* Returns the type (regular file or folder);
*
* @path: Path to the file
* @file: File/folder name
*/

int fileType(char * filePath){
	int status;
	struct stat st_buf;

	// Get the status of the file system object.

	status = stat(filePath, &st_buf);
	if (status != 0) {
		perror("<LOG - io.c>\n\tError loading stat.\n<end>\n");
		return -1;
	}
	
	
	if (S_ISREG (st_buf.st_mode)) {
		return FILE_TYPE;
	}
	
	if (S_ISDIR (st_buf.st_mode)) {
		return DIR_TYPE;
	}
	
	return UNKNOWN_TYPE;
}

char * createFilePath(char * path, char * file){
	char * filePath = (char *) calloc(BUFFER_SIZE, sizeof(char));
	
	strcat(filePath, path);
	strcat(filePath, file);
	
	return filePath;
	
}

List loadUsers(char * path){

	printf("<LOG - io.c> Comeinzo la carga de usuarios ... <end>\n");

	
	List usersList = (List) malloc(sizeof(llist));
	CreateList(usersList);

    DIR * d;
    char * dir_name = path;
    struct dirent * entry;

    /* Open the current directory. */
    d = opendir(dir_name);

    if(!d) {
        printf("Cannot open directory '%s'.\n",dir_name);
        exit(EXIT_FAILURE);
    }

    while(1){
        entry = readdir(d);
        
		if (!entry) {
            break;
        }
		else
		{
			if(dismissWrongFiles(entry)){
				char * filePath = createFilePath(path, entry->d_name);
				int type = fileType(filePath);
				if( type == FILE_TYPE){
					printf("<LOG - io.c> Cargando usuario: %s <end>\n", entry->d_name);
					
					AddToList((void *) loadUser(filePath), usersList);
										
				}	
				else if(type == DIR_TYPE)
					printf ("Carpeta: %s\n", entry->d_name);
				
			}
        }	
    }
    /* Close the directory. */
    if (closedir(d)) {
        perror("Could not close directory\n");
        exit (EXIT_FAILURE);
    }

	printf("<LOG - io.c> Fin de la carga de usuarios - Usuarios cargados: %d <end>\n", usersList->NumEl);
	

	return usersList;
}

int isExt(char * nameFile, char * ext){
	
	int dim = strlen(nameFile);
	int i;
	
	for(i = strlen(ext); i >= 0; i--, dim--){
		if(nameFile[dim] != ext[i]){
			return 0;
		}
	}
	
	return 1;
}

int isTeamsFolder(char * nameFile){

	return strcmp("teams",nameFile) == 0;
}

int isTradesFolder(char * nameFile){

	return strcmp("teams",nameFile) == 0;
}

List loadTeams(char * path){
	
	
		List teamsList = (List) malloc(sizeof(llist));
		if(teamsList == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		CreateList(teamsList);

	    DIR * d;
	    char * dir_name = path;
	    struct dirent * entry;

	    /* Open the current directory. */
	    d = opendir(dir_name);

	    if(!d) {
	        printf("Cannot open teams directory '%s'\n",dir_name);
	        exit(EXIT_FAILURE);
	    }

	    while(1){
	        entry = readdir(d);

			if (!entry) {
	            break;
	        }else{
				if(dismissWrongFiles(entry)){
					char * filePath = createFilePath(path, entry->d_name);
					int type = fileType(filePath);
					if( type == FILE_TYPE && isExt(entry->d_name,"team")){
						AddToList((void *) loadTeam(filePath), teamsList);
					}	

				}
	        }	
	    }
	    /* Close the directory. */
	    if (closedir(d)) {
	        perror("Could not close directory\n");
	        exit (EXIT_FAILURE);
	    }

		return teamsList;
}

List loadTrades(char * path){
	
	
		List tradesList = (List) malloc(sizeof(llist));
		if(tradesList == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		CreateList(tradesList);

	    DIR * d;
	    char * dir_name = path;
	    struct dirent * entry;

	    /* Open the current directory. */
	    d = opendir(dir_name);

	    if(!d) {
	        printf("Cannot open trades directory '%s'\n",dir_name);
	        exit(EXIT_FAILURE);
	    }

	    while(1){
	        entry = readdir(d);

			if (!entry) {
	            break;
	        }else{
				if(dismissWrongFiles(entry)){
					char * filePath = createFilePath(path, entry->d_name);
					int type = fileType(filePath);
					if( type == FILE_TYPE && isExt(entry->d_name,"trade")){
						AddToList((void *) loadTrade(filePath), tradesList);
					}	

				}
	        }	
	    }
	    /* Close the directory. */
	    if (closedir(d)) {
	        perror("Could not close directory\n");
	        exit (EXIT_FAILURE);
	    }

		return tradesList;
}

League loadLeague(char * path){

	DIR * d = NULL;
	char * dir_name = path;
	struct dirent * entry;
	int num,i,dim;
	Player auxPlayer = NULL;

	League auxLeague; // Will store the loaded team
	auxLeague = malloc(sizeof(league_t));
	
	if(auxLeague == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}	
	
	/* Open the current directory. */
	d = opendir(dir_name);
	
	if(!d){
		printf("Cannot open directory '%s'\n",dir_name);
		exit(EXIT_FAILURE);
	}
	while(1){
		entry = readdir(d);
		
		if(!entry){
			break;
		}else{
			if(dismissWrongFiles(entry)){
				char * filePath = createFilePath(path,entry->d_name);
				int type = fileType(filePath);
				/* si es el archivo league */
				if(type == FILE_TYPE && isExt(entry->d_name,"league")){
					FILE * file;
					char aux[BUFFER_SIZE]; // For reading the file line by line
					file = fopen(filePath,"r");

					if(file == NULL){
						perror("File not found\n");
						exit(EXIT_FAILURE);
					}
					/* Load ID */
					fgets(aux,BUFFER_SIZE,file);
					num = atoi(aux);
					auxLeague->ID = num;

					/* Load name */
					fgets(aux,BUFFER_SIZE,file);
					dim = strlen(aux);
					auxLeague->name  = (char *) calloc(dim, sizeof(char));
					if(auxLeague->name == NULL){
						printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
						exit(EXIT_FAILURE);
					}
					aux[dim-1] = 0;
					strcpy(auxLeague->name,aux);

					/* Load status */
					fgets(aux,BUFFER_SIZE,file);
					num = atoi(aux);
					auxLeague->status = num;
					
					/* Load cantDraft, turn, answer */
					auxLeague->cantDraft = 0;
					auxLeague->turn = NULL;
					auxLeague->answer = 0;

					/* Load available players */
					fgets(aux,BUFFER_SIZE,file);
					num = atoi(aux);
					auxLeague->cantAvailablePlayers = num;


					auxLeague->availablePlayers = (List)malloc(sizeof(llist));
					if(auxLeague == NULL){
						perror("Insufficient memory\n");
						exit(EXIT_FAILURE);
					}
					CreateList(auxLeague->availablePlayers);
					
					for(i = 0; i < auxLeague->cantAvailablePlayers; i++){
						auxPlayer = calloc(1, sizeof(player_t));

						if(auxPlayer == NULL){
							perror("Insufficient memory\n");
							exit(EXIT_FAILURE);
						}

						/* Load Player Name */ 
						fgets(aux,BUFFER_SIZE,file);
						dim = strlen(aux);
						auxPlayer->name  = (char *) calloc(dim, sizeof(char));
						if(auxPlayer->name == NULL){
							printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
							exit(EXIT_FAILURE);
						}
						aux[dim-1] = 0;
						strcpy(auxPlayer->name,aux);

						/* Load Player points */ 
						fgets(aux,BUFFER_SIZE,file);
						num = atoi(aux);
						auxPlayer->points = num;

						AddToList(auxPlayer,auxLeague->availablePlayers);

					} 

					fclose(file);

				}else if(type == DIR_TYPE && isExt(entry->d_name,"teams")){
					filePath = createFilePath(filePath,"/");
					auxLeague->teams = loadTeams(filePath);
					
				}else if(type == DIR_TYPE && isExt(entry->d_name,"trades")){
					filePath = createFilePath(filePath,"/");
					auxLeague->trades = loadTrades(filePath);
					
				}
			}
		}
	}
	
	if (closedir(d)) {
        perror("Could not close directory\n");
        exit (EXIT_FAILURE);
    }
	
	return auxLeague;
}

List loadLeagues(char * path){	
	
		List leaguesList = (List) malloc(sizeof(llist));
		if(leaguesList == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}

		CreateList(leaguesList);

	    DIR * d;
	    char * dir_name = path;
	    struct dirent * entry;

	    /* Open the current directory. */
	    d = opendir(dir_name);

	    if(!d) {
	        printf("Cannot open directory '%s'\n",dir_name);
	        exit(EXIT_FAILURE);
	    }

	    while(1){
	        entry = readdir(d);

			if (!entry) {
	            break;
	        }else{
				if(dismissWrongFiles(entry)){
					char * filePath = createFilePath(path, entry->d_name);
					int type = fileType(filePath);
					if( type == DIR_TYPE){
						filePath = createFilePath(filePath,"/");
						printf("<LOG - io.c> Cargando liga: %s  <end>\n", filePath);
						AddToList((void *) loadLeague(filePath), leaguesList);
					}
				}
	        }	
	    }
	    /* Close the directory. */
	    if (closedir(d)) {
	        perror("Could not close directory\n");
	        exit (EXIT_FAILURE);
	    }
		return leaguesList;
}



/*
* function objToFile
*
* Given an object type (i.e. "User"), builds
* the string that will be saved in the hard drive
* for persisting that kind of object.
*
* @path: Path of the saved user file 
*/
char * objToFile(int OBJ_TYPE, void * obj){
	User auxUser;
	char * obj_to_save = NULL;

	switch(OBJ_TYPE){
		case USER_OBJ:
			return userToString((User) obj);
			auxUser = (User) obj;
			printf("%s\n\n", auxUser->user);
		break;
		case LEAGUE_OBJ:
			return leagueToString((League) obj);
		break;
		
		case TRADE_OBJ:
			return tradeToString((Trade) obj);
		break;
		
		case TEAM_OBJ:
			return teamToString((Team) obj);
		break;
		default:
			printf("<LOG - io.c>\n\tUnrecognized data type.\n<end>\n");
			return NULL;
		break;
	}

	return obj_to_save;
		
	return NULL;
}

// char * userToString(User usr)
// {
// 	char * str;
// 	int strSize;
// 	int userSize = strlen(usr->user)+1;
// 	int passSize = strlen(usr->pass)+1;

// 	int intMaxCharSize = DIGIT_COUNT(INT_MAX) + 1;

// 	Element e;

// 	if((e = (Element) malloc(sizeof(lelement))) == NULL)
// 	{
// 		perror("userToString: not enough memory");
// 		exit(1);
// 	}

// 	char * leaguesIDstr;

// 	if((leaguesIDstr = (char*)malloc(sizeof(char)*intMaxCharSize*usr->leagues)) == NULL)
// 	{
// 		perror("userToString: not enough memory");
// 		exit(1);
// 	}

// 	int i = 0;

// 	FOR_EACH(e, usr->leaguesIDs){
// 		itoa(*((int*)e->data), leaguesIDstr+i);
// 		i += strlen(leaguesIDstr+i);
// 		leaguesIDstr[i] = '\n';
// 		i++;
// 	}

// 	leaguesIDstr[i-1] = '\0';

// 	int leaguesQtySize = ((int)log10(usr->leagues))+1;
// 	strSize = passSize + userSize + leaguesQtySize + i * sizeof(char);

// 	if((str = (char*)malloc(strSize)) == NULL)
// 	{
// 		perror("userToString: not enought memory");
// 		exit(1);
// 	}

// 	strcpy(str, usr->user);
// 	str[userSize-1] = '\n';
// 	strcpy(str+userSize, usr->pass);
// 	str[userSize+passSize-1] = '\n';
// 	itoa(usr->leagues, str+userSize+passSize);
// 	str[userSize+passSize+leaguesQtySize-1] = '\n';
// 	strcpy(str+userSize+passSize+leaguesQtySize, leaguesIDstr);

// 	free(leaguesIDstr);

// 	return str;

// }



char * userToString(User usr)
{
	char * str = NULL;

	int intMaxCharSize = DIGIT_COUNT(INT_MAX) + 1;


	char * leaguesIDstr;

	if((leaguesIDstr = (char *)malloc(sizeof(char)*intMaxCharSize*usr->leagues)) == NULL)
	{
		perror("userToString: not enough memory");
		exit(1);
	}


	if(usr->leaguesIDs->NumEl != 0){
		int i = 0;

		Element e;

		if((e = (Element) malloc(sizeof(lelement))) == NULL)
		{
			perror("userToString: not enough memory");
			exit(1);
		}

		FOR_EACH(e, usr->leaguesIDs){
			itoa(*((int*)e->data), leaguesIDstr+i);
			i += strlen(leaguesIDstr+i);
			leaguesIDstr[i] = '\n';
			i++;
		}

		leaguesIDstr[i-1] = '\0';

		asprintf(&str, "%s\n%s\n%d\n%s", usr->user, usr->pass, usr->leagues, leaguesIDstr);

		free(leaguesIDstr);
	}else{
		asprintf(&str, "%s\n%s\n%d\n", usr->user, usr->pass, usr->leagues);		
	}

	return str;

}

char * leagueToString(League lg){

	char * str;

	Element e;

	asprintf(&str, "%d\n%s\n%d\n%d", lg->ID, lg->name, lg->status, lg->cantAvailablePlayers);

	FOR_EACH(e, lg->availablePlayers){

		asprintf(&str, "%s\n%s\n%d", str, ((Player)e->data)->name, ((Player)e->data)->points);

	}

	return str;

}

// char * leagueToString(League lg){

// 	char * str;
// 	int nameSize = strlen(lg->name)+1;
// 	int idSize = DIGIT_COUNT(lg->ID)+1;
// 	int statusSize = DIGIT_COUNT(lg->status)+1;
// 	int qtyAvSize = DIGIT_COUNT(lg->cantAvailablePlayers)+1;
// 	int avPlyrsSize = 0;

// 	int * plyrsNamesSizes = malloc(sizeof(int)*lg->cantAvailablePlayers);
// 	int * plyrsPtsSizes = malloc(sizeof(int)*lg->cantAvailablePlayers);

// 	int i = 0;

// 	if(plyrsNamesSizes == NULL || plyrsPtsSizes == NULL){
// 		perror("leagueToString: not enough memory");
// 		exit(1);
// 	}

// 	Element e;

// 	FOR_EACH(e, lg->availablePlayers){
// 		avPlyrsSize += (plyrsNamesSizes[i] = strlen(((Player)e->data)->name)+1) + (plyrsPtsSizes[i] = DIGIT_COUNT(((Player)e->data)->points)+1);
// 		i++;
// 	}

// 	str = malloc(nameSize+idSize+statusSize+qtyAvSize+avPlyrsSize);

// 	if(str == NULL)	{
// 		perror("leagueToString: not enough memory");
// 		exit(1);
// 	}

// 	char * straux = str;
// 	itoa(lg->ID, straux);
// 	straux[idSize-1] = '\n';
// 	straux+=idSize;
// 	strcpy(straux, lg->name);
// 	straux[nameSize-1] = '\n';
// 	straux += nameSize;
// 	itoa(lg->status, straux);
// 	straux[statusSize-1] = '\n';
// 	straux += statusSize;
// 	itoa(lg->cantAvailablePlayers, straux);
// 	straux[qtyAvSize-1] = '\n';
// 	straux += qtyAvSize;

// 	i = 0;

// 	FOR_EACH(e, lg->availablePlayers){
// 		strcpy(straux, ((Player)e->data)->name);
// 		straux[plyrsNamesSizes[i]-1] = '\n';
// 		straux += plyrsNamesSizes[i];
// 		itoa(((Player)e->data)->points, straux);
// 		straux[plyrsPtsSizes[i]-1] = '\n';
// 		straux += plyrsPtsSizes[i];

// 		i++;
// 	}

// 	straux[-1] = '\0';

// 	free(plyrsNamesSizes);
// 		free(plyrsPtsSizes);

// 	return str;

// }

// char * tradeToString(Trade tr)
// {
// 	char * str;
// 	int usrFSize = strlen(tr->userFrom)+1;
// 	int usrTSize = strlen(tr->userTo)+1;
// 	int plyFSize = strlen(tr->playerFrom)+1;
// 	int plyTSize = strlen(tr->playerTo)+1;

// 	char * straux = str = malloc(sizeof(char)*(usrFSize+usrTSize+plyFSize+plyTSize+DIGIT_COUNT(tr->ID)+1+DIGIT_COUNT(tr->state)+1));

// 	if(str == NULL){
// 		perror("tradeToString: not enough memory");
// 		exit(1);
// 	}

// 	itoa(tr->ID, straux);
// 	straux[DIGIT_COUNT(tr->ID)] = '\n';
// 	straux += DIGIT_COUNT(tr->ID)+1;
// 	itoa(tr->state, straux);
// 	straux[DIGIT_COUNT(tr->state)] = '\n';
// 	straux += DIGIT_COUNT(tr->state)+1;
// 	strcpy(straux, tr->userFrom);
// 	straux[usrFSize-1] = '\n';
// 	straux += usrFSize;
// 	strcpy(straux,tr->userTo);
// 	straux[usrTSize-1] = '\n';
// 	strcpy(straux, tr->playerFrom);
// 	straux[plyFSize-1] = '\n';
// 	straux += plyFSize;
// 	strcpy(straux, tr->playerTo);

// 	return str;
// }

char * tradeToString(Trade tr)
{
	char * str;
	asprintf(&str, "%d\n%d\n%s\n%s\n%s\n%s", tr->ID, tr->state, tr->userFrom, tr->userTo, tr->playerFrom, tr->playerTo);
	return str;
}

// char * teamToString(Team t)
// {
// 	char * str;
// 	int ownerSize = strlen(t->owner)+1;
// 	int * plyrsNamesSizes = malloc(sizeof(int)*t->cantPlayers);
// 	int * plyrsPtsSizes = malloc(sizeof(int)*t->cantPlayers);
// 	int avPlyrsSize = 0;
// 	int i;
// 	Element e;

// 	FOR_EACH(e, t->players){
// 		avPlyrsSize += (plyrsNamesSizes[i] = strlen(((Player)e->data)->name)+1) + (plyrsPtsSizes[i] = DIGIT_COUNT(((Player)e->data)->points)+1);
// 		i++;
// 	}

// 	char * straux = str = malloc(ownerSize + avPlyrsSize + DIGIT_COUNT(t->ID)+1 + DIGIT_COUNT(t->points)+1 + DIGIT_COUNT(t->cantPlayers)+1);

// 	if(str == NULL)
// 	{
// 		perror("teamToString: not enough memory");
// 		exit(1);
// 	}

// 	itoa(t->ID, straux);
// 	straux[DIGIT_COUNT(t->ID)] = '\n';
// 	straux += DIGIT_COUNT(t->ID)+1;
// 	strcpy(straux, t->owner);
// 	straux[ownerSize-1] = '\n';
// 	straux += ownerSize;
// 	itoa(t->points, straux);
// 	straux[DIGIT_COUNT(t->points)] = '\n';
// 	straux += DIGIT_COUNT(t->points)+1;
// 	itoa(straux, t->cantPlayers);
// 	straux[DIGIT_COUNT(t->cantPlayers)] = '\n';
// 	straux += DIGIT_COUNT(t->cantPlayers)+1;

// 	i = 0;

// 	FOR_EACH(e, t->players){
// 		strcpy(straux, ((Player)e->data)->name);
// 		straux[plyrsNamesSizes[i]-1] = '\n';
// 		straux += plyrsNamesSizes[i];
// 		itoa(((Player)e->data)->points, straux);
// 		straux[plyrsPtsSizes[i]-1] = '\n';
// 		straux += plyrsPtsSizes[i];

// 		i++;
// 	}

// 	straux[-1] = '\0';

// 	free(plyrsNamesSizes);
// 	free(plyrsPtsSizes);

// 	return str;
// }

char * teamToString(Team t)
{
	char * str;

	Element e;
	asprintf(&str, "%d\n%s\n%d\n%d", t->ID, t->owner, t->points, t->cantPlayers);

	FOR_EACH(e, t->players){

		asprintf(&str, "%s\n%s\n%d", str, ((Player)e->data)->name, ((Player)e->data)->points);

	}

	return str;
}

Game loadGame(void){

	Game game = (Game) malloc(sizeof(game_t));
	if(game == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}

	game->users = loadUsers("./res/users/");
	game->leagues = loadLeagues("./res/leagues/");
	game->loggedUsers = (List) malloc(sizeof(llist));
	if(game->loggedUsers == NULL){
		perror("Insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	CreateList(game->loggedUsers);
	
	game->cantTrades = 0;
	game->cantTeams = 0;
	

		
	Element league_ptr;
	FOR_EACH(league_ptr, game->leagues){
	
		game->cantTeams += ((League)league_ptr->data)->teams->NumEl;
		game->cantTrades += ((League)league_ptr->data)->trades->NumEl;		
	}
	
	return game;
}

void checkMatchesDir(void)
{
	DIR * matches_dp;
	DIR * new_matches_dp;
	DIR * dump_matches_dp;
	int creatFlag = FALSE;

	if((matches_dp = opendir(MATCHES_DIR)) == NULL)
	{
		creatFlag = TRUE;
		if(mkdir(MATCHES_DIR, 0777) == -1)
		{
			perror("Could not create matches directory");
			exit(EXIT_FAILURE);
		}
		if((matches_dp = opendir(MATCHES_DIR)) == NULL)
		{
			perror("Critical failure opening matches directory");
			exit(EXIT_FAILURE);
		}
	}
	if(creatFlag || (new_matches_dp = opendir(MATCHES_NEW)) == NULL)
	{
		if(mkdir(MATCHES_NEW, 0777) == -1)
		{
			perror("Could not create new matches directory");
			exit(EXIT_FAILURE);
		}
		if((new_matches_dp = opendir(MATCHES_NEW)) == NULL)
		{
			perror("Critical failure opening new matches directory");
			exit(EXIT_FAILURE);
		}
	}
	if(creatFlag || (dump_matches_dp = opendir(MATCHES_DUMP)) == NULL)
	{
		if(mkdir(MATCHES_DUMP, 0777) == -1)
		{
			perror("Could not create dump matches directory");
			exit(EXIT_FAILURE);
		}
		if((dump_matches_dp = opendir(MATCHES_DUMP)) == NULL)
		{
			perror("Critical failure opening dump matches directory");
			exit(EXIT_FAILURE);
		}
	}

	struct dirent * d;
	List l;
	FILE * fp;

	while((d = readdir(new_matches_dp)) != NULL)
	{
		if(d->d_ino != 0)
		{
			char * f = malloc(strlen(MATCHES_NEW)+strlen(d->d_name)+1);
			strcpy(f, MATCHES_NEW);
			strcat(f, d->d_name);
			if(fileType(f) == FILE_TYPE)
			{
				if((fp = fopen(f,"r")) == NULL)
				{
					printf("Impossible to read file: %s\n", f);
					fclose(fp);
					free(f);
					continue;
				}
				if((l = loadMatch(fp)) != NULL){
					updatePlayers(l);
					fclose(fp);
					Element e;
					FOR_EACH(e, l)
					{
						free(((Player)e->data)->name);
						free(e->data);
					}
					e = l->pFirst;
					Element oe;
					oe = e->next;
					while(e != NULL)
					{
						free(e);
						e = oe;
						oe = ((e != NULL)?e->next:NULL);
					}
					dumpMatch(f);
				}
				free(f);
			}
		}
	}
}

List loadMatch(FILE * file){
	
	List ret = (List) malloc(sizeof(llist));
	if(ret == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	CreateList(ret);
	
	Player auxPlayer = NULL;
	
	char aux[BUFFER_SIZE]; // For reading the file line by line
	int cantP,i,dim;
	
	/* Cargo la cantidad de jugadores del archivo */
	fgets(aux,BUFFER_SIZE,file);
	cantP = atoi(aux);
	
	for(i = 0; i < cantP; i++){
	
		auxPlayer = calloc(1, sizeof(player_t));
		if(auxPlayer == NULL){
			perror("Insufficient memory\n");
			exit(EXIT_FAILURE);
		}
	
		/* Load Player Name */ 
		fgets(aux,BUFFER_SIZE,file);
		dim = strlen(aux);
		auxPlayer->name  = (char *) calloc(dim, sizeof(char));
		if(auxPlayer->name == NULL){
			printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
			exit(EXIT_FAILURE);
		}
		aux[dim-1] = 0;
		strcpy(auxPlayer->name,aux);

		/* Load Player points */ 
		fgets(aux,BUFFER_SIZE,file);
		int num = atoi(aux);
		auxPlayer->points = num;

		AddToList(auxPlayer, ret);
		
	}
	
	return ret;
}

void checkMatches(void)
{
	int in_fd = inotify_init();
	char stream[INOTIFY_BUFFER_SIZE];
	char * path;
	char filename[FILENAME_MAX];
	struct inotify_event * ine;
	int closeFlag = FALSE;
	List l;
	FILE * fp;

	if(in_fd == -1)
	{
		perror("Inotify could not be initialized");
		exit(EXIT_FAILURE);
	}

	int in_wd = inotify_add_watch(in_fd, MATCHES_NEW, IN_MOVED_TO | IN_CREATE);

	do
	{
		int nread;
		void * auxStream = stream;
		if((nread = read(in_fd, stream, INOTIFY_BUFFER_SIZE)) != -1)
		{
			int i;
			ine = NULL;
			for(i = 0; i <= nread; i++)
			{
				auxStream += ((i == 0)?0:(sizeof(struct inotify_event) + ine->len));
				ine = auxStream;
				strcpy(filename, ine->name);
				path = malloc(strlen(MATCHES_NEW)+ine->len+1);
				strcpy(path, MATCHES_NEW);
				strcat(path, filename);
				if(fileType(path) == FILE_TYPE)
				{
					if((fp = fopen(path,"r")) == NULL)
					{
						printf("Impossible to read file: %s\n", filename);
						fclose(fp);

						if((l = loadMatch(fp)) != NULL){
							updatePlayers(l);
							fclose(fp);
							Element e;
							FOR_EACH(e, l)
							{
								free(((Player)e->data)->name);
								free(e->data);
							}
							e = l->pFirst;
							Element oe;
							oe = e->next;
							while(e != NULL)
							{
								free(e);
								e = oe;
								oe = ((e != NULL)?e->next:NULL);
							}
							dumpMatch(path);

						}
					}
					free(path);
				}
			}

		}
	}while(closeFlag != TRUE);
}

void updatePlayers(List l){

       Element p,league,team,player;
       int rc;

       rc = pthread_mutex_lock(&game_mutex);

       FOR_EACH(p, l){

               FOR_EACH(league, gameAux->leagues){

                       /* Cargo points en availablePlayers de una league */
                       FOR_EACH(player, ((League)league->data)->availablePlayers){

                               if(strcmp(((Player)p->data)->name,((Player)player->data)->name) == 0){
                                       ((Player)player->data)->points += ((Player)p->data)->points;
                               }
                       }

                       /* Cargo points en cada player de un team y en el team */
                       FOR_EACH(team, ((League)league->data)->teams){

                               FOR_EACH(player, ((Team)team->data)->players){

                                       if(strcmp(((Player)p->data)->name,((Player)player->data)->name) == 0){
                                               ((Player)player->data)->points += ((Player)p->data)->points;
                                               ((Team)team->data)->points += ((Player)p->data)->points;
                                       }
                               }
                       }
               }
       }

       rc = pthread_mutex_unlock(&game_mutex);
}

void dumpMatch(char * path){

	char string[200];
	
	sprintf("mv %s %s",string,path,MATCHES_DUMP);
	
	//exec(string);
	
}

List loadPlayers(void){

	FILE * file;

	List players;
	int cantPlayers,i,dim;
	file = fopen("./res/players.players","r");
	char aux[BUFFER_SIZE]; // For reading the file line by line
	char * player;

	if(file == NULL){
		printf("<LOG - io.c>\n\tFile not found.\n<end>\n");
		exit(EXIT_FAILURE);
	}

	players = (List) malloc(sizeof(llist));
	if(players == NULL){
		printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}
	CreateList(players);

	/* Load cantPlayers */
	fgets(aux,BUFFER_SIZE,file);
	cantPlayers = atoi(aux);

	for(i = 0; i < cantPlayers; i++){
		/* Load username */
		fgets(aux,BUFFER_SIZE,file);
		dim = strlen(aux);
		player = (char *) malloc(dim);
		if(player == NULL){
			printf("<LOG - io.c>\n\tInsufficient memory.\n<end>\n");
			exit(EXIT_FAILURE);
		}
		aux[dim-1] = 0;
		strcpy(player,aux);
		
		AddToList(player, players);
	}

	fclose(file);

	return players;
}

void saveGame(void){


	/* We dont want inconsistent data! Lock the mutex */
	int rc;
	rc = pthread_mutex_lock(&game_mutex);
	printf("Saving game ... \n");

	// loadUsers("./res/users/");
	// loadLeagues("./res/leagues/");
	
	char * filePath;

	/* Salvo usuarios */
	Element user_ptr;
	FOR_EACH(user_ptr, gameAux->users){
		asprintf(&filePath, "./res/users/%s.user", ((User) user_ptr->data)->user);
		saveUser(filePath, ((User) user_ptr->data));
	}


	/* Salvo ligas */
	Element league_ptr;
	FOR_EACH(league_ptr, gameAux->leagues){
	
	}
	
	/* Game data can continue to be used */
	rc = pthread_mutex_unlock(&game_mutex);

	printf("Finished saving game ... \n");
	return ;
}

void saveUser(char * path, User usr){

	FILE * userFILE;
	int * id;
	
	userFILE = fopen(path,"w+");
	
	if(userFILE == NULL){
		perror("User file could not be created.");
		exit(EXIT_FAILURE);
	}

	char * usertBytestrem = objToFile(USER_OBJ, usr);
	fprintf(userFILE, "%s", usertBytestrem);
		
	fclose(userFILE);

}

