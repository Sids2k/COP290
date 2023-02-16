#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define MEM 64000
#include <stdio.h>
#include <ucontext.h>
#include <stdbool.h>

static ucontext_t ctx, ctx1;

static void conjecture(int len, void* options, int sz, void fn(void*)){
	int i = -1;
	// printf("%d\n", i);
	getcontext(&ctx); // Save main context
	i++;
	if (i == len) {
		return;
	}
	getcontext(&ctx1); // Save temp context
	ctx1.uc_stack.ss_sp = malloc(MEM); 
	ctx1.uc_stack.ss_size = MEM;
	ctx1.uc_link = &ctx; 
	makecontext(&ctx1, fn, 1, options + i * sz); // temp context calls fn
	setcontext(&ctx1); // start the temp context
} // Create context and start traversal

void assert(bool b){
	if(!b) {
		printf("Assertion failed\n");
		setcontext(&ctx); // Restore context if condition fails
	}
} // Restore context if condition fails

bool is_prime(int x) {
	for(int i = 2; i <= x/2; i ++) {
		if(x % i == 0)
			return false;
	}
	return true;
}

bool is_lt_40(int x) {
	return x < 40;
}

int nested(int i) {
	assert(!is_prime(i));
	return i * i;
}

void app(void* c) {
	// printf("In app");
	int* i = (int*)c;
	// printf("Trying %d\n", *i);
	assert(is_lt_40(*i));
	int x = nested(*i);
	printf("%d\n", x);
}

int main(void) {
	int mynums[] = {25, 39, 11, 23, 42, 41, 52, 22};
	// printf("Trying");
	// We have to ensure that conjecture lives in the bottom of the call stack. 
	// If the conjecture frame is popped, we will never be able to rollback to it.
	conjecture(8, (void*) mynums, sizeof(int), &app);
}


// main references: https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-makecontext-modify-user-context#rmctxt and https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-getcontext-get-user-context#rgcntx