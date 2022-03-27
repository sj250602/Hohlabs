#include "labs/fiber_scheduler.h"

//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked li[sts, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//

void factorial(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int *arg){
	addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
	addr_t& f_stack    = *pf_stack;
	int& ret           = *pret;
	bool& done         = *pdone;
	int& limit         = *arg;
    
    if(limit<=0){
        ret = 1;
    }else{
	for(int i=0;i<limit;i++){

		int extra = i-1; 	
		int factans = 0;
		factorial(pmain_stack,pf_stack,&factans,pdone,&extra);
		ret += factans+1;
		done = false;
		stack_saverestore(f_stack, main_stack);
	}
    }
}


void G_fun(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int *arg){
	//do nothing
	addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
	addr_t& f_stack    = *pf_stack;
	int& ret           = *pret;
	bool& done         = *pdone;
	int& limit         = *arg;
	
	if(limit<=0){
		ret += 1;
	}else{
		int extra = limit-1; 	
		int factans = 0;
	       factorial(pmain_stack,pf_stack,&factans,pdone,&extra);
	       ret += factans+1;
	       done = false;
		stack_saverestore(f_stack, main_stack);
	       
	}
	for(;;) {
		done = true;
		stack_saverestore(f_stack, main_stack);
	}
}


void H_fun(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int *arg){
	addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
	addr_t& f_stack    = *pf_stack;
	int& ret           = *pret;
	bool& done         = *pdone;
	int& limit         = *arg;

	int i;
	int j;
	int k;

	for(i=1;i<=limit;i++){
		for(j=1;j<=limit;j++){
			for(k=1;k<limit;k++){
				ret=((i+j+k) *17)%1000000007;
				done=false; 
				stack_saverestore(f_stack,main_stack);
			}
		}
	}
	for(;;){
		//ret=0;
		done=true; 
		stack_saverestore(f_stack,main_stack);
	}
}



void shell_step_fiber_scheduler(shellstate_t& shellstate, addr_t& main_stack, preempt_t& preempt, addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size, dev_lapic_t& lapic){
  	
    //insert your code here
	size_t stack_size = arrays_size/stackptrs_size;
	
	if(shellstate.G){
	
		shellstate.G_invoke++;
		
		int x=-1;
		int hello = 0;
		for(;hello<5;hello++){
			if(!shellstate.runnable[hello]){
				x = hello;
				break;
			}
		}	
		
		shellstate.g_h[x] = 'G';
		shellstate.array_arg[x] = shellstate.GArg;
		shellstate.runnable[x] = true;
		shellstate.G = false;
		
		stack_init5(stackptrs[x + 1], arrays + (x + 1) * stack_size, stack_size ,&G_fun, &stackptrs[0], &stackptrs[x + 1], &shellstate.sret[x], &shellstate.sdone[x], &shellstate.array_arg[x]);
		
	}else if(shellstate.H){
		shellstate.H_invoke++;
		
		int x=-1;
		int hello = 0;
		for(;hello<5;hello++){
			if(!shellstate.runnable[hello]){
				x = hello;
				break;
			}
		}	
		
		shellstate.g_h[x] = 'H';
		shellstate.array_arg[x] = shellstate.HArg;
		shellstate.runnable[x] = true;
		shellstate.H = false;
		
		stack_init5(stackptrs[x + 1], arrays + (x + 1) * stack_size, stack_size ,&H_fun, &stackptrs[0], &stackptrs[x + 1], &shellstate.sret[x], &shellstate.sdone[x], &shellstate.array_arg[x]);
			
	}
	
	if(shellstate.runnable[shellstate.buffer_pos]){
		lapic.reset_timer_count(100);
            preempt.yielding = 0;
            stack_saverestore(stackptrs[0], stackptrs[shellstate.buffer_pos + 1]);
            lapic.reset_timer_count(0);
		
		
		if(shellstate.sdone[shellstate.buffer_pos]){
		
			shellstate.sdone[shellstate.buffer_pos]  = false;
			shellstate.runnable[shellstate.buffer_pos] = false;
			if(shellstate.g_h[shellstate.buffer_pos]=='G'){
				shellstate.g_h[shellstate.buffer_pos] = ' ';
				shellstate.G_invoke--;
				/*for(int i=0;i<50;i++){
            shellstate.result[i] =' ';
        }*/
				shellstate.result[0] = 'g';
				shellstate.result[1] = '_';
			}else{
				shellstate.g_h[shellstate.buffer_pos] = ' ';
				shellstate.H_invoke--;
				/*for(int i=0;i<50;i++){
            shellstate.result[i] =' ';
        }*/
				shellstate.result[0] = 'h';
				shellstate.result[1] = '_';
			}
			
			int arh = shellstate.array_arg[shellstate.buffer_pos];
			int ex = arh;
			int len1 = 0;
			while(arh!=0){
			   len1++;
			   arh/=10;
			}
			
			shellstate.cur_line +=1;
			int factans = shellstate.sret[shellstate.buffer_pos];
			int extra = factans;
			int len = 0;
			while(factans!=0){
			 len++;
			 factans/=10;
			}
			len1+=2;
			len+=len1+1;
			int k = len;
			for(;k<50;k++){
				shellstate.result[k] = ' ';
			}
			int m = len1;
			while(ex!=0){
			   shellstate.result[--len1]=char(ex%10 + 48);
			   ex/=10;
			}
			
			shellstate.result[m+1] = ':';
			
			while(extra!=0){
			 shellstate.result[--len]=char(extra%10 + 48);
			 extra/=10;
			}
		}
	}
	
	
	shellstate.buffer_pos = (shellstate.buffer_pos + 1)%5;
	
}





