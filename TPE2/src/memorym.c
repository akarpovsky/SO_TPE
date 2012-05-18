#include "../include/memorym.h"

List pages;


void createHeadersList(void * inicio, void * fin)
{
	// CANTIDAD DE PAGINAS DISPONIBLES
	int cantPages = ((int)fin - (int)inicio) / MAX_PAGE_SIZE;
	// CANTIDAD DE PAGINAS QUE VOY A NECESITAR PARA LOS HEADERS
	int cantHeadPages = (((cantPages * sizeof(element)) + sizeof(llist)) / MAX_PAGE_SIZE) + 1 ;
	int i;

	//inicio va a ser el la direccion de la primera pagina que tengo que prender
	void * dir = inicio;
	int pag = 0;

	
	// Ahora tengo que prender las paginas que voy a necesitar para los headers
	for( i = 0; i < cantHeadPages; i++){

		dir = (void*)(i * MAX_PAGE_SIZE);

		// Calculo que pagina es en memoria y la prendo
		pag = ((int)dir) / MAX_PAGE_SIZE;

		presentPageNumber(pag);
	}


	((List)inicio)->NumEl = 0;
	((List)inicio)->pFirst = NULL;
	((List)inicio)->pLast = NULL;

	// Seteo en la lista la direccion de la primera pagina destinada a mallocs
	((List)inicio)->first_page = inicio + MAX_PAGE_SIZE * cantHeadPages;
	((List)inicio)->max_pages = cantPages - cantHeadPages;

	int a = addHeader((List)inicio);

//	int b = addHeader((List)inicio);
	pages = ((List)inicio);

}

int addHeader(List list){

	if(list->NumEl >= list->max_pages){
		return NO_MEM;
	}

	if(list->NumEl == 0){

		list->pFirst = (void*) (list + sizeof(llist));
		list->pLast = list->pFirst;

	}else{
		list->pLast = list->pLast + sizeof(element);
	}

	(list->NumEl)++;

	initHeader(&(list->pLast->headerEl));
	return OK;

}

void initHeader(mem_header * h){
	int i;
	if (h!=NULL && h->header!=NULL){
		for (i=0; i<MAX_HEADER_SIZE;i++)
			h->header[i]=0;
		h->blocks_cont = MAX_HEADER_SIZE;

	}
	h->pid = FREE_PAGE;
	
}

void * malloc(int size){

	void * ans = NULL;
	int i=0;
	int j=0;
	int k=0; /* Indice de la menor posicion posible */
	int aux;
	int aux2;
	int flag = NOT_FOUND;
	mem_header * actual;
	int bloques = getBlocks(size);

	if(size > 0 && size <= MAX_PAGE_SIZE){

		/* Itero sobre el pages para ver si cuento con un segmento de memorio del tamaño solicitado */	
		for (i = 0; i < pages->NumEl && flag == NOT_FOUND; i++){
			if(GET_HEADER(i).blocks_cont >= bloques &&
				(GET_HEADER(i).pid == FREE_PAGE || GET_HEADER(i).pid == getpid())){

				actual = &(GET_HEADER(i)); /* Header */
				for(j = 0, k = -1; (j < MAX_HEADER_SIZE) && (actual->header[j] =! bloques)
					&& (actual->header[j] != 0); j++){
					/* Busca el menor bloque en el que yo entre */
					if( actual->header[j] >= bloques){
						if( k == -1 ){
							k = j;
						}else{	
							k = (actual->header[j] < actual->header[k] ? j : k);
						}	
					}
				}
				// Si sale porque entra justo en un bloque
				if (actual->header[j] == bloques){
					actual->header[j] *= -1;
				}

				// Si salio porque encontro un cero
				else if( j >= MAX_HEADER_SIZE ||  actual->header[j] == 0){
					
					if(k!=-1){
						aux = actual->header[k] - bloques;
					
						actual->header[k] = bloques*-1;
					
						j = k++;
						while(actual->header[k] != 0 && k < MAX_HEADER_SIZE){
							aux2 = actual->header[k];
							actual->header[k] = aux;
							aux = aux2;
							k++;
						}if(actual->header[k] == 0){
							actual->header[k] = aux;
						}
					}
					else
						actual->header[j]= bloques*-1;
				}
				// ans = el sector de memoria es "[i][j]" 
				
				flag = FOUND;
				// Marco a la pagina con el pid del proceso
				actual->pid = getpid();

				// TODO
				// AGREGAR LA PAGINA I A LA LISTA DE PAGINAS DEL PROCESO SI NO EXISTE

				//corregirlo contando cuantos bloques recorrio
				for(k=0, ans=pages->first_page + (i * MAX_PAGE_SIZE); k<j ; k++){
					ans += abs(actual->header[k])*PADDING;
				}
				//CALCULAR BLOQUES DISPONIBLES
				actual->blocks_cont=cantMaxBlocks(actual->header);

			} 
		}


		if(flag == NOT_FOUND && pages->NumEl < pages->max_pages){

			addHeader(pages);
			actual = &(GET_HEADER(i));
			actual->header[0]= bloques*-1;
			actual->pid = getpid();
			actual->blocks_cont=cantMaxBlocks(actual->header);
			
			ans = GET_PAGE(i);

		}

	}

	return ans;

}


/* Segun un size, la cantidad minima de bloques necesitada */
int getBlocks(int size){
	int aux = 0;
	if (size >=0)
	 aux=size/PADDING + (size % PADDING == 0 ? 0 : 1);
	return aux; 
}

int cantMaxBlocks(char* header){
	int i;
	int ret = 0;
	int aux = MAX_HEADER_SIZE;
	
	for(i = 0; header[i] != 0; i++){
		aux = aux - abs(header[i]);		
//		kprintf("%d-%d-%d\n",i, header[i], aux);

		if( header[i] > 0 && ret < header[i]){
			ret = header[i];
		}
	}
	
	if(ret < aux){
		ret = aux;
	}	
	return ret;
}

int abs(int num){
	return (num>=0?num:num*-1);
}

// Retorna la cantidad de bytes usados en una pagina
int getUsed(mem_header * page_header){
		
	int sum = 0;
	int i;
	if (page_header!=NULL){
		for (i = 0; i < MAX_HEADER_SIZE && page_header -> header[i] != 0; i++){
			sum += ((page_header -> header)[i] > 0 ? 0 : (int)((page_header -> header)[i]));
		}
	}
	return sum * PADDING * (-1);
}

// Retorna la cantidad de bytes libres en una pagina
int getFreeSpace(mem_header* header){
	
	return MAX_PAGE_SIZE - getUsed(header);
}

void * calloc(int size){

	int i;
	char * p = (char *) malloc(size);
	int bloques = getBlocks(size);

	if(p != NULL){
		for (i = 0; i < bloques * PADDING; i++){
			p[i] = 0;
		}
	}
	return p;
}

void free(void* p){
	
	int i;
	int j;
	int k;
	int dif;
	int aux;
	mem_header* actual;

	/* Calculo a que pagina debo moverme*/
	i = getPageIndex(p);
	if(i != -1){

		actual = &(GET_HEADER(i));

		aux = p - (void*)(GET_PAGE(i));
	
		if( aux % PADDING == 0){
			aux /= PADDING;
			for(j = 0, k = 0; j < MAX_HEADER_SIZE && aux > 0; j++){
			
				aux = aux - abs(actual->header[j]);
			
				if(aux >=0){
					k++;
				}
			}
			if(aux == 0 && actual->header[k] < 0){
		
				actual->header[k] = actual->header[k] * -1;
				//aca busco si mi antecesor es un espacio libre
				if (k>0 && actual->header[k-1]>0){
					k--;
				}
				dif=k;
				while(k< MAX_HEADER_SIZE && actual->header[k]!=0  && dif<MAX_HEADER_SIZE){
				
					if(actual->header[k]>0 && (dif+1<MAX_HEADER_SIZE) && actual->header[dif+1]>0){
						dif++;
						actual->header[k]+=actual->header[dif];
						if (dif+1<MAX_HEADER_SIZE && actual->header[dif+1]>0){
								actual->header[k]+=actual->header[dif++];
						}				
					}
					k++;
					dif++;
					if (dif<MAX_HEADER_SIZE){
						actual->header[k]= actual->header[dif];
					}
					if (dif-k>1){
						actual->header[k]= actual->header[dif-1];
					}
				}		
			}
			for (j=MAX_HEADER_SIZE-1; j>=0 && actual->header[j]>0; j--){
					actual->header[j]=0;
			}
			actual->blocks_cont=cantMaxBlocks(actual->header);
			if(actual->blocks_cont == MAX_HEADER_SIZE){
				actual->pid = FREE_PAGE;
				//TODO ELIMINAR LA PAGINA DE LA LISTA DE PAGINAS DEL PROCESO
			}
		}
	}
	
	return;
}

int getPageIndex (void* p){
	int i;
	int found = 0;
	
	for (i = 0; i < pages->NumEl && !found; i++){

		found = (p >= (void*)GET_PAGE(i) && (p < ((void*)GET_PAGE(i) + MAX_PAGE_SIZE))) ? 1 : 0;
	}
	return (found ? (i - 1) : -1);
}

// PROBARLO
int getPageIndexEFICIENTE(void * p){

	if( p > pages->first_page && p < (void *)(((int)pages->first_page) + (pages->NumEl * MAX_PAGE_SIZE))){
		void * pointer = (void *) (pages->first_page - p);
		return (int)pointer / MAX_PAGE_SIZE;

	}

	return -1;

}

void changePagePID(int pid, void * pointer){

	int page = getPageIndex(pointer);
	mem_header* h = &GET_HEADER(page);
	h->pid = pid;

}


void * getFreePage(){

	return malloc(MAX_PAGE_SIZE);
}










