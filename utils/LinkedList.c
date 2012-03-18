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
    
	//Create generic element to hold item ptr
    Element NewEl;
    NewEl = (Element)malloc(sizeof(NewEl));  //create generic element
 
 	//  assert(NewEl != NULL);
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

int main(void){
	
	List myList = (List *) malloc(sizeof(llist));
	CreateList(myList);
	AddToList((void *) 5, myList);
	printf("%d\n", myList->pFirst);

	return 0;
}