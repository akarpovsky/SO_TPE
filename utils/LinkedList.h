#if !defined( LinkedList_h )
#define LinkedList_h
#endif

#define FOR_EACH(item_ptr, list) \
     for (ListEl = myList.pFirst; ListEl != NULL; ListEl=ListEl->next)



//Generic list lelement

typedef struct lelement{
    struct lelement *  next;
    struct lelement *  prev;
    void * data;
	} lelement;

typedef lelement * Element;

//Generic List Structure
typedef struct llist {
    int NumEl;   //Number of lelement * s in list
    lelement *  pFirst;  //Ptr to first lelement *  in list
    lelement *  pLast;   //Ptr to last lelement *  in list
} llist;

typedef llist * List;

void CreateList(List list);
Element AddToList(void *item, List list);