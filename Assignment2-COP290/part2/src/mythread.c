// Make custom thread library
#include "mythread.h"
#include "list.h"
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
    ctx.uc_stack.ss_sp = malloc(STACK_SIZE);
    ctx.uc_stack.ss_size = STACK_SIZE;
    // uc_link to main context
    ctx.uc_link = &main_ctx;
    // make context
    makecontext(&ctx, func, 1, arg);

    // add to list
    list_add(l, ctx);
}

// mythread_join function
void mythread_join(){
    // add a loop
    while(!list_is_empty(l)){
        ucontext_t temp = list_remove(l);
        swapcontext(&main_ctx, &temp);
    }
}




