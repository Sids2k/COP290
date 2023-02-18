#include "../include/mythread.h"
#include "../include/list.h"
#include "../include/hm.h"

void list_rm(struct list* l, struct listentry* e)    // Remove an item from the list
{
    if(e->prev != NULL)
        e->prev->next = e->next;
    else
        l->head = e->next;
    if(e->next != NULL)
        e->next->prev = e->prev;
    else
        l->tail = e->prev;
    free(e);
}

struct listentry* list_add(struct list* l, void* data)  // Add an item to the list
{
    struct listentry* e = malloc(sizeof(struct listentry));
    e->data = data;
    e->next = NULL;
    e->prev = l->tail;
    if(l->tail != NULL)
        l->tail->next = e;
    else
        l->head = e;
    l->tail = e;
    return e;
}


struct list* list_new()  // Return an initialized list
{
    struct list* l = malloc(sizeof(struct list));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

int is_empty(struct list* l)  // Check if list is empty or not
{
    return l->head == NULL;
}