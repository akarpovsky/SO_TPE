#ifndef _MEMORYM_H_
#define _MEMORYM_H_

#include "scheduler.h"

#ifndef _NULL_
#define _NULL_
#define NULL (void*) 0
#endif /*_NULL_*/
#define MAX_HEADER_SIZE 128 /* Tamaño del header */
#define MAX_PAGE_SIZE 4096 /* 4K */
#define PADDING 32 /* Tamaño minimo que se puede allocar */
#define NOT_FOUND 0
#define FOUND 1
#define NO_MEM 0
#define OK 1
#define FREE_PAGE -1 // pid que no pertenece a ningun proceso
#define GET_HEADER(i) \
            ((Element)((pages->pFirst) + i*sizeof(element)))->headerEl
#define GET_PAGE(i) \
            ((pages->first_page) + (i * MAX_PAGE_SIZE))

#define CEILING_POS(X) (((X)-(int)(X)) > 0 ? (int)((X)+1) : (int)(X))
#define CEILING_NEG(X) (((X)-(int)(X)) < 0 ? (int)((X)-1) : (int)(X))
#define CEILING(X) ( ((X) > 0) ? CEILING_POS(X) : CEILING_NEG(X) )



// FALTA FIRST_PAGE Y MAX_PAGES que estan en la estructura llist

typedef struct mem_header{
	
	char header[MAX_HEADER_SIZE];
	unsigned char blocks_cont; /*= MAX_HEADER_SIZE; cantidad minima contigua de bloques, al principio 128 */
	int pid;
}mem_header;

typedef struct element{
    struct element *  next;
    struct element *  prev;
    mem_header headerEl;
	} element;

typedef element * Element;

//Generic List Structure
typedef struct llist {
    int NumEl;   //Number of lelement * s in list
    element *  pFirst;  //Ptr to first lelement *  in list
    element *  pLast;   //Ptr to last lelement *  in list
    void * first_page;
    int max_pages;
} llist;

typedef llist * List;

#define FOR_EACH(item_ptr, myList) \
     for (item_ptr = myList->pFirst; item_ptr != NULL; item_ptr=item_ptr->next)

void createHeadersList(void * inicio, void * fin);
int addHeader(List list);
void initHeader(mem_header * h);
void * malloc(int size);
int getBlocks(int size);
int cantMaxBlocks(char* header);
int abs(int num);
int getUsed(mem_header * page_header);
int getFreeSpace(mem_header* header);
void * calloc(int size);
void free(void* p);
int getPageIndex (void* p);
int getPageIndexEFICIENTE(void * p);
void changePagePID(int pid, void * pointer);
void * getFreePage();

#endif /*_MEMORYM_H_*/
