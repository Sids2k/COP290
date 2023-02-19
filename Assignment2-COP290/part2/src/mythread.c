// Make custom thread library
#define _XOPEN_SOURCE
#include <stdio.h>
#include "../include/hm.h"
#include "../include/list.h"
#include "../include/mythread.h"
#include <ucontext.h>
#define MEM 64000

static ucontext_t main_ctx;
struct listentry* curr_le;

struct list *l;

void mythread_init() {
	l = list_new();
}

// mythread_create function
ucontext_t* mythread_create(void func(void*), void* arg){
    // set up context
    getcontext(&main_ctx);
    // printf("Inside mythread_create");
    ucontext_t* ctx = (ucontext_t*) malloc(sizeof(ucontext_t));
    // static ucontext_t ctx;
    // add to list
    list_add(l, ctx);
    getcontext(ctx);
    (*ctx).uc_stack.ss_sp = malloc(MEM);
    (*ctx).uc_stack.ss_size = MEM;
    // uc_link to main context
    (*ctx).uc_link = &main_ctx;
    makecontext(ctx, func, 1, arg);

    return ctx;
}

struct listentry* get_next(struct list* l, struct listentry* e)
{
    if(e->next != NULL)
        return e->next;
    else
        return l->head;
}

// mythread_join function
void mythread_join(){
    // get the head
    // struct listentry* le = l->head;
    curr_le = l->head;

    // check if list is empty
    while (!is_empty(l)){
        // get the context
        ucontext_t* temp = (ucontext_t*) curr_le->data;
        // curr_le = le;
        // swap context to 
        swapcontext(&main_ctx, temp);
        // remove the element
        struct listentry* to_rem = curr_le;
        curr_le = get_next(l, curr_le);
        list_rm(l, to_rem);
    }
}

// mythread_yield function
void mythread_yield(){

    // see what the current context is
    ucontext_t* curr_ctx = (ucontext_t*) curr_le->data;

    // get the next element
    struct listentry* next_le = get_next(l, curr_le);

    // get the next context
    ucontext_t* next_ctx = (ucontext_t*) next_le->data;

    curr_le = next_le;

    // swap context
    swapcontext(curr_ctx, next_ctx);
}


struct lock* lock_new()   // return an initialized lock object
{
    struct lock* lk = (struct lock*) malloc(sizeof(struct lock));
    lk->ctx = NULL;
    return lk;
}

void lock_acquire(struct lock* lk)   // Set lock. Yield if lock is acquired by some other thread.
{
    while (lk->ctx != NULL)
        mythread_yield();
    lk->ctx = (ucontext_t*) curr_le->data;
}

int lock_release(struct lock* lk)   // Release lock
{
    if (lk->ctx == (ucontext_t*) curr_le->data)
    {
        lk->ctx = NULL;
        return 1;
    }
    else
        return 0;
}
