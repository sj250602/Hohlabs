#include "labs/fiber.h"


void f(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int *arg){
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




void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, preempt_t& preempt, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize, dev_lapic_t& lapic){

  //insert your code here
  if (shellstate.fstart == 'd' && shellstate.ftask){

	    stack_init5(f_stack, f_array, f_arraysize, &f, &main_stack, &f_stack, &shellstate.fret, &shellstate.fdone, &shellstate.farg);
      // Change the state
            shellstate.fret = 1;
            shellstate.ftask = false;
            shellstate.fstart = 's';
    } else if (shellstate.fstart == 's') {
        if (!shellstate.fdone) {
        
            lapic.reset_timer_count(100);
            preempt.yielding = 0;
            stack_saverestore(main_stack,f_stack);
            lapic.reset_timer_count(0);
            
        } else {
        
            shellstate.fstart = 'r';
            
        }
    } else if (shellstate.fstart == 'r') {
    
        shellstate.fdone  = false;
        shellstate.fstart = 'd';
	//shellstate.cur_line +=1;
	int factans = shellstate.fret;
	int extra = factans;
	int len = 0;
	while(factans!=0){
	 len++;
	 factans/=10;
	}
	int k = len;
	for(;k<50;k++){
		shellstate.result[k] = ' ';
	}
	while(extra!=0){
	 shellstate.result[--len]=char(extra%10 + 48);
	 extra/=10;
	}
    }

}
