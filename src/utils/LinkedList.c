#include <stdio.h>
#include <stdlib.h>

#include "./LinkedList.h"

void CreateList(List list)
{
    list->NumEl = 0;
    list->pFirst = NULL;
    list->pLast = NULL;
}

Element AddToList(void *item, List list)
{
    //check inputs
  //  assert(item!=NULL); assert(list!=NULL);
    if(item == NULL || list == NULL){
		printf("<LOG - LinkedList.c>\n\tNull pointer.\n<end>\n");
		exit(EXIT_FAILURE);
	}

	//Create generic element to hold item ptr
    Element NewEl;
    NewEl = (Element)malloc(sizeof(lelement));  //create generic element

	if(NewEl == NULL){
		printf("<LOG - LinkedList.c>\n\tInsufficient memory.\n<end>\n");
		exit(EXIT_FAILURE);
	}

    list->NumEl = list->NumEl + 1;
    NewEl->data = item;
    if (list->NumEl == 1)
    {
      list->pFirst = NewEl;
      NewEl->prev = NULL;
      NewEl->next = NULL;
    }
    else
    {
      NewEl->prev = list->pLast;
      NewEl->next = NULL;
      list->pLast->next = NewEl;
    }
    list->pLast = NewEl;
    return NewEl;
}

void AddElemToList(Element NewEl, List list){
	
	list->NumEl = list->NumEl + 1;
    if (list->NumEl == 1)
    {
      list->pFirst = NewEl;
      NewEl->prev = NULL;
      NewEl->next = NULL;
    }
    else
    {
      NewEl->prev = list->pLast;
      NewEl->next = NULL;
      list->pLast->next = NewEl;
    }
    list->pLast = NewEl;
    return;
	
}


void Remove(Element elem, List list){
	
	Element item;
	int flag;

	FOR_EACH(item, list){
		if(item == elem){
			flag = 1;
			break;
		}
	}
	
	if(flag == 0){
		return;
	}
		
	/* Soy el primero */
	if(list->pFirst == elem){
		list->pFirst = elem->next;
		elem->next->prev = NULL;
	}
	/* Soy el ultimo */
	if(list->pLast == elem){
		list->pLast = elem->prev;
		elem->prev->next = NULL;
	}
	if(elem->prev != NULL && elem->next != NULL){
		elem->prev->next = elem->next;
		elem->next->prev = elem->prev;
	}
	
	elem->prev = NULL;
	elem->next = NULL;
	
	return;
}