#include "../include/proc.h"

extern Task_t processes[MAX_PROCESSES];

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

//TODO: add to .h

Task_t * remove_from_q(TaskQueue_t * q, Task_t * t)
{
	if(q == NULL || t == NULL)
	{
		return NULL;
	}

	Task_t * t2 = q->head;

	while(t != NULL && t2->pid != t->pid)
	{
		t2 = t2->next;
	}

	if(t == NULL)
	{
		return NULL;
	}

	if(q->size == 1)
	{
		q->head = NULL;
		q->tail = NULL;
		q->size--;
		return t;
	}

	if(q->head == t2)
	{
		q->head = t2->next;
		t2->next->prev = NULL;
		t2->next = NULL;
		q->size--;
		return t;
	}

	if(q->tail == t2)
	{
		q->tail = t2->prev;
		t2->prev->next = NULL;
		t2->prev = NULL;
		q->size--;
		return t;
	}

	t2->prev->next = t2->next;
	t2->next->prev = t2->prev;
	q->size--;
	return t;

}
