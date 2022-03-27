#include "labs/coroutine.h"

void factor(int n,int& cnt){
    if(n==0){
        return ;
    }
    for(int i=0;i<n;i++){
        cnt++;
        factor(n-1,cnt);
    }
}

void f(coroutine_t* pf_coro,f_t* pf_locals,bool* pdone,int *arg){
	coroutine_t& f_coro = *pf_coro; // boilerplate: to ease the transition from existing code
	
	bool& done          = *pdone;

	int& i              = pf_locals->i;
	int& j              = pf_locals->j;
	int& k              = pf_locals->k;
	int& ret            = pf_locals->cret;
	int &limit          = *arg;

	h_begin(f_coro);

	for(i=1;i<=limit;i++){
		for(j=1;j<=limit;j++){
			for(k=1; k<limit;k++){
	  			ret=((i+j+k) *17)%1000000007;
	  			done=false;
	  			h_yield(f_coro);
			}
		}
	}

	//ret=0;
	done=true;
	h_end(f_coro);
}


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){

    //insert your code here
     
   if (shellstate.cstart == 'd' && shellstate.ctask==true){
	    coroutine_reset(f_coro);
	    f_locals.i = 0;
	    f_locals.j = 0;
	    shellstate.ctask = false;
	    shellstate.cstart = 's';
	    f_locals.k = 0;
	    f_locals.cret = 0;
	   
    } else if (shellstate.cstart == 's'&&shellstate.cdone==false) {
      	f(&f_coro, &f_locals,&shellstate.cdone,&shellstate.carg);
    } else if (shellstate.cstart == 's'&&shellstate.cdone==true) {
       	shellstate.cstart = 'r';  
    } else if (shellstate.cstart == 'r') {
    
        shellstate.cdone  = false;
        shellstate.cstart = 'd';
	shellstate.cur_line +=1;
	
	int factans = f_locals.cret;
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
