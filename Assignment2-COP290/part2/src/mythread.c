// Make custom thread library
#include "mythread.h"
#include "list.h"
#define MEM 64000
#include <hm.h>
#include <list.h>

struct ucontext_t main_ctx;
struct list *l;

void mythread_init() {
	l = list_new();
}

// mythread_create function
void* mythread_create(void func(void*), void* arg){
    // set up context
    ucontext_t ctx;
    getcontext(&ctx);
    ctx.uc_stack.ss_sp = malloc(MEM);
    ctx.uc_stack.ss_size = MEM;
    // uc_link to main context
    ctx.uc_link = &main_ctx;
    // make context
    makecontext(&ctx, func, 1, arg);

    // add to list
    list_add(l, &ctx);
}

// mythread_join function
void mythread_join(){
    // add a loop
    while(!is_empty(l)){
        // get element from list
        struct listentry* le = l->head;
        ucontext_t temp = *(ucontext_t*) le->data;
        // remove from list
        list_rm(l, &temp);
        swapcontext(&main_ctx, &temp);
    }
}




