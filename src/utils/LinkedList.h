#if !defined( LinkedList_h )
#define LinkedList_h



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

#define FOR_EACH(item_ptr, myList) \
     for (item_ptr = myList->pFirst; item_ptr != NULL; item_ptr=item_ptr->next)

void CreateList(List list);
Element AddToList(void *item, List list);
void Remove(Element elem, List list);

#endif
