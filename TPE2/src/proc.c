#include "../include/proc.h"

Task_t *pop(TaskQueue_t * q){
	if(q == NULL){
		return NULL;
	}

	if(q->size == 0){
		return NULL;
	}

	Task_t * ret;
	ret = q->head;
	q->head = q->head->next;
	ret->prev = ret->next = q->head->prev = NULL;
	q->size--;
	if(q->size == 0){
		q->head = q->tail = NULL;
	}
	return ret;
}

bool empty(TaskQueue_t * q){
	return (q->size == 0);
}

void add_to_queue(TaskQueue_t * q, Task_t * t){
	if(q == NULL || t == NULL){
		return;
	}

	if(q->size == 0){
		q->head = q->tail = t;
		t->next = t->prev = NULL;
		q->size++;
		return;
	}

	q->tail->next = t;
	t->prev = q->tail;
	t->next = NULL;
	q->tail = t;
	q->size++;
	return;

}

int null_process(int argc, char **argv){
	_Sti();
	do
	{
		//putc('n');
	}while(true);

	return 0;
}



